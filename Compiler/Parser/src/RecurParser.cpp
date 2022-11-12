#include <queue>
#include "RecurParser.h"
//#define TIMETEST



namespace PARSER {

    bool RecurParser::Run() {
        Compiler::Log::Init("Parser");
        position = 0;
        m_Token = token_buffer[0];
        root = new ASTNode("", urgency);
        root->setRoot(true);
#ifndef TIMETEST
        AST_stack.push(root);
#endif
        while (position < token_buffer.size() - 1) {
            while (!AST_stack.empty()&&AST_stack.top() != root && AST_stack.size() >= 1)AST_stack.pop();
            errorFlag = 0;
            ParseS();
        }
        return finalCheck;
    }

    void RecurParser::ParseS() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-"||m_Token.first==7) {
            ParseADD();
            matchToken({-1, ";"});
        }
        else if (m_Token.second == "const") {
            ParseCD();
            matchToken({-1,";"});
        } else if (m_Token.second == "int" || m_Token.second == "float") {
            ParseVD();
            matchToken({-1,";"});
        } else
            errorHandle();
    }

    void RecurParser::ParseRE() {
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-"||m_Token.first==7) {
            ParseADD();
            ParseRE_();
        } else
            errorHandle();
    }

    void RecurParser::ParseRE_() {
        if (m_Token.second == "<=" || m_Token.second == ">=" || m_Token.second == "<" || m_Token.second == ">") {
            matchToken({-1, m_Token.second});
            ParseADD();
            ParseRE_();
        } else if (m_Token.second == "==" || m_Token.second == "!=" || m_Token.second == "&&" ||
                   m_Token.second == "||" || m_Token.second == ";" || m_Token.second == ")") {}
        else
            errorHandle();
    }

    void RecurParser::ParseCOND() {

    }

    void RecurParser::ParseVD() {
        if (m_Token.second == "int" || m_Token.second == "float") {
            ParseCD_();
            matchToken({-1, "="});
            ParseIV();
            ParseCD___();
            AST_stack.pop();
            //matchToken({-1, ";"});
        } else
            errorHandle();
    }

    void RecurParser::ParseADD() {
        if (errorFlag)return;
        inADD = 1;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-"||m_Token.first==7) {
            ParseMUL();
            ParseADD_();
        } else
            errorHandle();
        inADD = 0;
    }

    void RecurParser::ParseMUL() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-"||m_Token.first==7) {
            ParseUE();
            ParseMUL_();
        } else
            errorHandle();
    }

    void RecurParser::ParsePE() {
        if (errorFlag)return;
        if (m_Token.second == "(") {
            matchToken({-1, "("});
            urgency++;
            ParseADD();
            matchToken({-1, ")"});
            urgency--;
        } else if (m_Token.first == 1)
            ParseLV();
        else if (m_Token.first == 3||m_Token.first==7)
            ParseNUM();
        else
            errorHandle();
    }

    void RecurParser::ParseADD_() {
        if (errorFlag)return;
        if (m_Token.second == "+" || m_Token.second == "-") {
            creatTree({-1, m_Token.second});
            matchToken({-1, m_Token.second});
            ParseMUL();
            ParseADD_();
            std::string s;
            if(!AST_stack.empty())
            s = AST_stack.top()->getContent();
            if (AST_stack.size()>=2)
                AST_stack.pop();
        } else if (m_Token.second == "#" || m_Token.second == ")" || m_Token.second == "]" || m_Token.second == "," ||
                   m_Token.second == ";") {}
        else if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
                 m_Token.second == "-"||m_Token.first==7) {}
        else
            errorHandle();
    }

    void RecurParser::ParseMUL_() {
        if (errorFlag)return;
        if (m_Token.second == "*" || m_Token.second == "/" || m_Token.second == "%") {
            creatTree({-1, m_Token.second});
            matchToken({-1, m_Token.second});
            ParseUE();
            ParseMUL_();
            std::string s;
            if(!AST_stack.empty())
            s = AST_stack.top()->getContent();
            if (AST_stack.size()>=2)
                AST_stack.pop();
        } else if (m_Token.second == "+" || m_Token.second == "-" || m_Token.second == "#" || m_Token.second == ")" ||
                   m_Token.second == "]" || m_Token.second == ",") {}
        else if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
                 m_Token.second == "-" || m_Token.second == ";"||m_Token.first==7) {}
        else
            errorHandle();
    }

    void RecurParser::ParseUE() {
        if (errorFlag)return;
        if (m_Token.first == 1) {
            creatTree({1, ""});
            matchToken({1, ""});
            ParseUE_();
        } else if (m_Token.second == "+" || m_Token.second == "-") {
            ParseUO_C();
            ParseUE();
        } else if (m_Token.second == "(" || m_Token.first == 3||m_Token.first == 7) {
            ParsePE();
        } else {
            errorHandle();
        }
    }

    void RecurParser::ParseNUM() {
        if (errorFlag)return;
        if (m_Token.first == 3) {
            creatTree({3, ""});
            matchToken({3, ""});
        }
        else if (m_Token.first==7){
            creatTree({7, ""});
            matchToken({7, ""});
        }else
            errorHandle();

    }

    void RecurParser::ParseLV() {
        if (errorFlag)return;
        if (m_Token.first == 1) {
            creatTree({1, ""});
            matchToken({1, ""});
            ParseLV_();
        } else
            errorHandle();
    }

    void RecurParser::ParseFP() {
        if (errorFlag)return;
        if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
            m_Token.second == "-"||m_Token.first==7) {
            ParseADD();
            ParseFP_();
        } else if (m_Token.second == ")") {}
        else
            errorHandle();
    }

    void RecurParser::ParseUO_C() {
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

    void RecurParser::ParseLV_() {
        if (errorFlag)return;
        if (m_Token.second == "[") {
            matchToken({-1, "["});
            ParseADD();
            matchToken({-1, "]"});
            if(!AST_stack.empty())
                AST_stack.pop();
            //CORE_INFO(AST_stack.top()->getContent());
        } else if (m_Token.second == "%" || m_Token.second == "/" || m_Token.second == "*" || m_Token.second == "+" ||
                   m_Token.second == "-" || m_Token.second == "#" || m_Token.second == ";") {
            if(!AST_stack.empty()) {
                AST_stack.top()->setOpFlag(false);
                AST_stack.top()->setFuncFlag(false);
            }
        }
        else
            errorHandle();
    }

    void RecurParser::ParseFP_() {
        if (errorFlag)return;
        if (m_Token.second == ",") {
            std::string s;
            if(!AST_stack.empty())
            s = AST_stack.top()->getContent();
            while (!AST_stack.empty()&&(!AST_stack.top()->getOpFlag() || s == "+"
                   || s == "-" || s == "*" || s == "/" || s == "%"))
                AST_stack.pop();
            matchToken({-1, ","});
            ParseADD();
            ParseFP_();
        } else if (m_Token.second == ")") {
            std::string s;
            if(!AST_stack.empty())
            s = AST_stack.top()->getContent();
            while (!AST_stack.empty()&&(!AST_stack.top()->getOpFlag() || s == "+"
                   || s == "-" || s == "*" || s == "/" || s == "%"))
                AST_stack.pop();
        } else
            errorHandle();
    }

    void RecurParser::ParseUE_() {
        if (errorFlag)return;
        if (m_Token.second == "+" || m_Token.second == "-" || m_Token.second == "*" || m_Token.second == "/" ||
            m_Token.second == "%" || m_Token.second == "," || m_Token.second == ")" || m_Token.second == ";" ||
            m_Token.second == "#" || m_Token.second == "]") {
            if(!AST_stack.empty()) {
                AST_stack.top()->setOpFlag(false);
                AST_stack.top()->setFuncFlag(false);
                if(!AST_stack.empty())AST_stack.pop();
            }
        } else if (m_Token.second == "(") {
            matchToken({-1, "("});
            ParseFP();
            matchToken({-1, ")"});
            if(!AST_stack.empty())
            AST_stack.pop();
        } else if (m_Token.second == "[") {
            AST_stack.top()->setFuncFlag(false);
            matchToken({-1, "["});
            ParseADD();
            matchToken({-1, "]"});
            if(!AST_stack.empty())
            AST_stack.pop();
            //CORE_TRACE(AST_stack.top()->getContent());
        } else
            errorHandle();
    }

    void RecurParser::errorHandle() {
        if(position== token_buffer.size())position--;
        CORE_ERROR("Line:" + std::to_string(pos_buffer[position][0]) + " CharBegin:" +
                   std::to_string(pos_buffer[position][1]) + " CharEnd:" +
                   std::to_string(pos_buffer[position][2]));
        CORE_ERROR("Syntax Error! " + m_Token.second + " is illegal here!");
        while (position < token_buffer.size() && m_Token.first != 1 && m_Token.first != 3 && m_Token.second != "(" &&
               m_Token.second != "+" && m_Token.second != "-"||m_Token.first==7) {
            position++;
            m_Token = token_buffer[position];
        }
        errorFlag = 1;
        finalCheck = 0;
    }

    void RecurParser::matchToken(std::pair<int, std::string> token) {
        if (errorFlag)return;
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
        //CORE_INFO(token.second);
    }

    void RecurParser::ParseCD() {
        if (m_Token.second == "const") {
            constSituation=1;
            creatTree({-1,"const"});
            matchToken({-1, "const"});
            ParseCD_();
            matchToken({-1, "="});
            ParseIV();
            ParseCD___();
            constSituation=0;
            AST_stack.pop();
            //matchToken({-1, ";"});
        } else
            errorHandle();
    }

    void RecurParser::ParseIV() {
        if (m_Token.second == "{") {
            matchToken({-1, "{"});
            ParseIV();
            ParseIV_();
            matchToken({-1, "}"});
        } else if (m_Token.first == 1 || m_Token.first == 3 || m_Token.second == "(" || m_Token.second == "+" ||
                   m_Token.second == "-"||m_Token.first==7) {
            ParseADD();
            //std::cout<<"here"<<std::endl;
        } else
            errorHandle();
    }

    void RecurParser::ParseIV_() {
        if (m_Token.second == ",") {
            matchToken({-1, ","});
            ParseIV();
            ParseIV_();
        } else if (m_Token.second == "}") {}
        else
            errorHandle();
    }

    void RecurParser::ParseCD_() {
        if (m_Token.second == "int") {
            creatTree({-1, "int"});
            matchToken({-1, "int"});

            creatTree({1, ""});
            matchToken({1, ""});
            ParseCD__();
            AST_stack.pop();
        } else if (m_Token.second == "float") {
            creatTree({-1, "float"});
            matchToken({-1, "float"});
            creatTree({1, ""});
            matchToken({1, ""});
            ParseCD__();
            AST_stack.pop();
        } else
            errorHandle();
    }

    void RecurParser::ParseCD__() {
        if (m_Token.second == "[") {
            matchToken({-1, "["});
            ParseADD();
            matchToken({-1, "]"});
            ParseCD__();
        } else if (m_Token.second == "=") {}
        else
            errorHandle();
    }

    void RecurParser::ParseCD___() {
        if (m_Token.second == ",") {
            AST_stack.pop();
            matchToken({-1, ","});
            while(!AST_stack.empty()&&!(AST_stack.top()->getContent()=="int"||AST_stack.top()->getContent()=="float"))
                AST_stack.pop();
            creatTree({1,""});
            matchToken({1, ""});
            ParseCD__();
            matchToken({-1, "="});
            ParseIV();
            ParseCD___();
            //AST_stack.pop();
        } else if (m_Token.second == ";") {}
        else
            errorHandle();
    }

    void RecurParser::creatTree(std::pair<int, std::string> token) {
        //CORE_WARN(token.second);
        //CORE_WARN(AST_stack.top()->getContent());

        //if (!inADD)return;
        if (errorFlag)return;
        ASTNode *temp = AST_stack.top();
        //CORE_INFO(temp->getContent());
        //CORE_INFO(token.second);
        std::string s = temp->getContent();
        if (token.first != -1) {
            ASTNode *new_node = new ASTNode(token.second == "" ? m_Token.second : token.second);
            if(token.first==3||token.first==7)new_node->setConstFlag(true);
            if(token.first==3)new_node->setInt(true);
            if(token.first==7)new_node->setFloat(true);
            if (token.first == 1) {
                if(s=="int"||s=="float")
                {
                    temp->addChild(new_node);
                    AST_stack.push(new_node);
                    if(constSituation) {
                        new_node->setConstDeclFlag(true);
                        new_node->setConstFlag(true);
                    }
                    else
                        new_node->setDecl(true);
                    AST_stack.push(new_node);
                    return ;
                }
                AST_stack.top()->addChild(new_node);
                new_node->setOpFlag(true);
                new_node->setFuncFlag(true);
                AST_stack.push(new_node);
            } else {
                if (s == "+" || s == "-" || s == "*" || s == "/" || s == "%") {
                    AST_stack.top()->addChild(new_node);
                } else {
                    AST_stack.top()->addChild(new_node);
                    AST_stack.push(new_node);
                }
            }
            PrintTree(root);
        } else if (token.second == "+" || token.second == "-" || token.second == "*" || token.second == "/" ||
                token.second == "%") {
                ASTNode *new_node = new ASTNode(m_Token.second, urgency);
                new_node->setOpFlag(true);
                if (!temp->getOpFlag()) {
                    if(temp->getConstDeclFlag()||temp->getDecl()) {
                        ASTNode *last = AST_stack.top()->getChldren().back();
                        AST_stack.top()->getChldren().pop_back();
                        new_node->addChild(last);
                        AST_stack.top()->getChldren().push_back(new_node);
                        AST_stack.push(new_node);
                        return;
                    }
                    if (s != "" && (token.second == "+" || token.second == "-")) {
                        AST_stack.pop();
                        std::string s1 = AST_stack.top()->getContent();
                        if (AST_stack.top()->getUrgency() > urgency ||
                            (AST_stack.top()->getUrgency() == urgency && (s1 == "*" || s1 == "/" || s1 == "%"))) {
                            ASTNode *last = AST_stack.top();
                            AST_stack.pop();
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(last);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        }else if(AST_stack.top()->getDecl()||AST_stack.top()->getConstDeclFlag()){
                            CORE_INFO(AST_stack.top()->getContent());
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                        else {
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->getChldren().push_back(new_node);
                            AST_stack.push(new_node);
                        }
                        //PrintTree(root);
                    } else if (s != "") {
                        AST_stack.pop();
                        std::string s1 = AST_stack.top()->getContent();

                        if (AST_stack.top()->getUrgency() < urgency ||
                            (AST_stack.top()->getUrgency() == urgency &&
                             (s1 == "+" || s1 == "-" || s1 == "" || AST_stack.top()->getFuncFlag()))) {
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->getChldren().push_back(new_node);
                            AST_stack.push(new_node);
                            //PrintTree(root);
                        }else if(AST_stack.top()->getDecl()||AST_stack.top()->getConstDeclFlag()){
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(temp);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                        else {
                            ASTNode *last = AST_stack.top();
                            AST_stack.pop();
                            AST_stack.top()->getChldren().pop_back();
                            new_node->addChild(last);
                            AST_stack.top()->addChild(new_node);
                            AST_stack.push(new_node);
                        }
                    }
                    else {
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
                        if (urgency < temp->getUrgency() ||
                            ((urgency == temp->getUrgency()) && (token.second == "+" || token.second == "-"))) {
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
                        if (urgency < temp->getUrgency()) {
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
                    }
                } else if(temp->getConstDeclFlag()||temp->getDecl()){
                    temp->addChild(new_node);
                    AST_stack.push(new_node);
                }
                else {
                    temp->addChild(new_node);
                    AST_stack.push(new_node);
                }
                PrintTree(root);
        }
        else if(token.second == "const")
        {
            while(!AST_stack.top()->getRoot())AST_stack.pop();
            ASTNode* new_node = new ASTNode("const");
            new_node->setConstDeclFlag(true);
            AST_stack.top()->addChild(new_node);
            AST_stack.push(new_node);
            PrintTree(root);
        }
        else if(token.second == "int"){
            CORE_WARN(temp->getContent());
            ASTNode* new_node = new ASTNode("int");
            new_node->setOpFlag(true);
            if(constSituation)
                new_node->setConstDeclFlag(true);
            else
                new_node->setDecl(true);
            AST_stack.top()->addChild(new_node);
            AST_stack.push(new_node);
            PrintTree(root);
        }else if(token.second == "float"){
            ASTNode* new_node = new ASTNode("float");
            new_node->setOpFlag(true);
            if(constSituation)
                new_node->setConstDeclFlag(true);
            else
                new_node->setDecl(true);
            AST_stack.top()->addChild(new_node);
            AST_stack.push(new_node);
            PrintTree(root);
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
