#include <queue>
#include "Parser.h"

namespace Parser {
    bool Parser::Run() {
        position = 0;
        m_Token = token_buffer[0];
        root = new ASTNode("");
        AST_stack.push(root);
        while (position < token_buffer.size() - 1) {
            while (AST_stack.top() != root && AST_stack.size() >= 1)AST_stack.pop();
            errorFlag = 0;
            ParseS();
        }
        return finalCheck;
    }

    void Parser::ParseS() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-")
            ParseADD();
        else
            errorHandle();
    }

    void Parser::ParseADD() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-") {
            ParseMUL();
            ParseADD_();
        } else
            errorHandle();
    }

    void Parser::ParseMUL() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-") {
            ParseUE();
            ParseMUL_();
        } else
            errorHandle();
    }

    void Parser::ParsePE() {
        if (errorFlag)return;
        if (m_Token.second == "(") {
            matchToken({-1, "("});
            urgency++;
            ParseADD();
            matchToken({-1, ")"});
            urgency--;
        } else if (m_Token.first == 1)
            ParseLV();
        else if (m_Token.first == 3)
            ParseNUM();
        else
            errorHandle();
    }

    void Parser::ParseADD_() {
        if (errorFlag)return;
        if (m_Token.second == "+" || m_Token.second == "-") {
            creatTree({-1, m_Token.second});
            matchToken({-1, m_Token.second});
            ParseMUL();
            ParseADD_();
            std::string s = AST_stack.top()->getContent();
            if (s == "+" || s == "-")
                AST_stack.pop();
        } else if (m_Token.second == "#" || m_Token.second == ")" || m_Token.second == "]" || m_Token.second == ",") {}
        else if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
                 m_Token.second == "-") {}
        else
            errorHandle();
    }

    void Parser::ParseMUL_() {
        if (errorFlag)return;
        if (m_Token.second == "*" || m_Token.second == "/" || m_Token.second == "%") {
            creatTree({-1, m_Token.second});
            matchToken({-1, m_Token.second});
            ParseUE();
            ParseMUL_();
            std::string s = AST_stack.top()->getContent();
            if (s == "*" || s == "/" || s == "%")
                AST_stack.pop();
        } else if (m_Token.second == "+" || m_Token.second == "-" || m_Token.second == "#" || m_Token.second == ")" ||
                   m_Token.second == "]" || m_Token.second == ",") {}
        else if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
                 m_Token.second == "-") {}
        else
            errorHandle();
    }

    void Parser::ParseUE() {
        if (errorFlag)return;
        if (m_Token.first == 1) {
            creatTree({1, ""});
            matchToken({1, ""});
            ParseUE_();
        } else if (m_Token.second == "+" || m_Token.second == "-") {
            ParseUO_C();
            ParseUE();
            //AST_stack.pop();
        } else if (m_Token.second == "(" || m_Token.first == 3) {
            ParsePE();
        } else {
            errorHandle();
        }
    }

    void Parser::ParseNUM() {
        if (errorFlag)return;
        if (m_Token.first == 3) {
            creatTree({3, ""});
            matchToken({3, ""});
        } else
            errorHandle();

    }

    void Parser::ParseLV() {
        if (errorFlag)return;
        if (m_Token.first == 1) {
            creatTree({1, ""});
            matchToken({1, ""});
            ParseLV_();
        } else
            errorHandle();
    }

    void Parser::ParseFP() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-") {
            ParseADD();
            ParseFP_();
        } else if (m_Token.second == ")") {}
        else
            errorHandle();
    }

    void Parser::ParseUO_C() {
        if (errorFlag)return;
        if (m_Token.second == "+") {
            creatTree({3, "0"});
            creatTree({-1, "+"});
            matchToken({-1, "+"});
        } else if (m_Token.second == "-") {
            creatTree({3, "0"});
            creatTree({-1, "-"});
            matchToken({-1, "-"});
        } else
            errorHandle();
    }

    void Parser::ParseLV_() {
        if (errorFlag)return;
        if (m_Token.second == "[") {
            matchToken({-1, "["});
            ParseADD();
            matchToken({-1, "]"});
        } else if (m_Token.second == "%" || m_Token.second == "/" || m_Token.second == "*" || m_Token.second == "+" ||
                   m_Token.second == "-" || m_Token.second == "#") {}
        else
            errorHandle();
    }

    void Parser::ParseFP_() {
        if (errorFlag)return;
        if (m_Token.second == ",") {
            std::string s = AST_stack.top()->getContent();
            while (!AST_stack.top()->getOpFlag() || s == "+"
                   || s == "-" || s == "*" || s == "/" || s == "%")
                AST_stack.pop();
            matchToken({-1, ","});
            ParseADD();
            ParseFP_();
        } else if (m_Token.second == ")") {
            std::string s = AST_stack.top()->getContent();
            while (!AST_stack.top()->getOpFlag() || s == "+"
            || s == "-" || s == "*" || s == "/" || s == "%")
                AST_stack.pop();}
        else
            errorHandle();
    }

    void Parser::ParseUE_() {
        if (errorFlag)return;
        if (m_Token.second == "+" || m_Token.second == "-" || m_Token.second == "*" || m_Token.second == "/" ||
            m_Token.second == "%" || m_Token.second == "," || m_Token.second == ")") {
            AST_stack.top()->setOpFlag(false);
            //AST_stack.pop();
        } else if (m_Token.second == "(") {
            matchToken({-1, "("});
            ParseFP();
            matchToken({-1, ")"});
            AST_stack.pop();
        } else if (m_Token.second == "[") {
            matchToken({-1, "["});
            ParseADD();
            matchToken({-1, "]"});
            AST_stack.pop();
        } else
            errorHandle();
    }

    void Parser::errorHandle() {
        CORE_ERROR("Line:" + std::to_string(pos_buffer[position][0]) + " CharBegin:" +
                   std::to_string(pos_buffer[position][1]) + " CharEnd:" +
                   std::to_string(pos_buffer[position][2]));
        CORE_ERROR("Syntax Error! " + m_Token.second + " is illegal here!");
        while (position < token_buffer.size() && m_Token.first != 1 && m_Token.first != 3 && m_Token.second != "(" &&
               m_Token.second != "+" && m_Token.second != "-") {
            position++;
            m_Token = token_buffer[position];
        }
        errorFlag = 1;
        finalCheck = 0;
    }

    void Parser::matchToken(std::pair<int, std::string> token) {
        if (errorFlag)return;
        //std::cout<<m_Token.second<<std::endl;
        if (token.first != -1) {
            if (token.first == m_Token.first)position++;
            else errorHandle();
        } else {
            if (token.second == m_Token.second)position++;
            else errorHandle();
        }
        if (position == token_buffer.size())
            return;
        m_Token = token_buffer[position];
    }

    void Parser::creatTree(std::pair<int, std::string> token) {
        if (errorFlag)return;
        ASTNode *temp = AST_stack.top();
        std::string s = temp->getContent();
        if (token.first != -1) {
            ASTNode *new_node = new ASTNode(token.second == "" ? m_Token.second : token.second);
            if (token.first == 1) {
                AST_stack.top()->addChild(new_node);
                new_node->setOpFlag(true);
                AST_stack.push(new_node);
                //PrintTree(root);
                //std::cout<<AST_stack.top()->getContent()<<std::endl;
            } else {
                if (s == "+" || s == "-" || s == "*" || s == "/" || s == "%") {
                    AST_stack.top()->addChild(new_node);
                    //AST_stack.push(new_node);
                } else {
                    AST_stack.top()->addChild(new_node);
                    AST_stack.push(new_node);
                    //std::cout<<new_node->getContent()<<std::endl;
                }
                //PrintTree(root);
            }
        } else {
            if (token.second == "+" || token.second == "-" || token.second == "*" || token.second == "/" ||
                token.second == "%") {
                ASTNode *new_node = new ASTNode(m_Token.second,urgency);
                new_node->setOpFlag(true);
                //std::cout<<AST_stack.top()->getContent();
                if (!temp->getOpFlag()) {
                    //std::cout<<temp->getContent()<<std::endl;
                    if (s != "" && (token.second == "+" || token.second == "-")) {
                        AST_stack.pop();
                        std::string s1 = AST_stack.top()->getContent();
                        if (AST_stack.top()->getUrgency()>urgency||(AST_stack.top()->getUrgency()==urgency&&(s1 == "*" || s1 == "/" || s1 == "%"))) {
                            ASTNode *last = AST_stack.top();
                            AST_stack.pop();
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(last);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        } else {
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->getChldren().push_back(new_node);
                            AST_stack.push(new_node);
                        }
                        //PrintTree(root);
                    } else if (s != "") {
                        AST_stack.pop();
                        std::string s1 = AST_stack.top()->getContent();
                        if(AST_stack.top()->getUrgency()<urgency||(AST_stack.top()->getUrgency()==urgency&&(s1 == "+" || s1 == "-")))
                        {
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->getChldren().push_back(new_node);
                            AST_stack.push(new_node);
                            //PrintTree(root);
                        }
                        else
                        {
                            ASTNode *last = AST_stack.top();
                            AST_stack.pop();
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(last);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                    } else {
                        ASTNode *last = AST_stack.top()->getChldren().back();
                        AST_stack.top()->getChldren().pop_back();
                        new_node->addChild(last);
                        AST_stack.top()->getChldren().push_back(new_node);
                        AST_stack.push(new_node);
                        //PrintTree(root);
                    }
                } else if ((s == "+" || s == "-" || s == "*" || s == "/" || s == "%") &&
                           temp->getChldren().size() == 2) {
                    if (s == "+" || s == "-") {
                        if (urgency<temp->getUrgency()||((urgency==temp->getUrgency())&&(token.second == "+" || token.second == "-"))) {
                            AST_stack.pop();
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        } else {
                            ASTNode *last = temp->getChldren().back();
                            temp->getChldren().pop_back();
                            new_node->addChild(last);
                            temp->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                    } else {
                        if(urgency<temp->getUrgency()) {
                            AST_stack.pop();
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                        else
                        {
                            ASTNode *last = temp->getChldren().back();
                            temp->getChldren().pop_back();
                            new_node->addChild(last);
                            temp->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                    }
                    //PrintTree(root);
                } else {
                    temp->addChild(new_node);
                    AST_stack.push(new_node);
                    //PrintTree(root);
                }
            }
        }

    }

    void Print(ASTNode *);

    void PrintTree(ASTNode *root) {
        for (const auto &c: root->getChldren()) {
            Print(c);
            std::cout << std::endl;
        }
    }


    void Print(ASTNode *root) {
        ::std::queue<ASTNode *> con;
        con.push(root);
        while (!con.empty()) {
            ASTNode *temp = con.front();
            con.pop();

            ::std::cout << temp->getContent() << " ";
            for (const auto &c: temp->getChldren()) {
                con.push(c);
            }
        }
    }

}