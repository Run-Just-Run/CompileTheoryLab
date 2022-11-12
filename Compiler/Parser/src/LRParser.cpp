#include "LRParser.h"

namespace PARSER {

    VTYPE token2vtype(std::pair<int, std::string> token) {
        CORE_ASSERT(0 <= token.first <= 6, "NOT A TOKEN!");

        if (token.first == 1) return VTYPE::IDENT;
        else if (token.first == 3||token.first==7) return VTYPE::NUM;
        else if (token.second == "(") return VTYPE::L_PARENTHESES;
        else if (token.second == ")") return VTYPE::R_PARENTHESES;
        else if (token.second == "[") return VTYPE::L_PARENTHESES_;
        else if (token.second == "]") return VTYPE::R_PARENTHESES_;
        else if (token.second == "+" || token.second == "-") return VTYPE::P_SIGN;
        else if (token.second == "*" || token.second == "/" || token.second == "%") return VTYPE::M_SIGN;
        else if (token.second == "#") return VTYPE::END;
        else if (token.second == ",") return VTYPE::COMMA;
        CORE_ERROR("UNKNOWN TOKEN!" + token.second);
        return VTYPE::EMPTY;
    }

    void printStack(std::stack<int> s) {
        while (!s.empty()) {
            std::cout << s.top() << " ";
            s.pop();
        }
        std::cout << std::endl;
    }

    bool LRParser::Run() {
        Compiler::Log::Init("Parser");
        m_Pos=-1;
        t_Type=TTYPE::EMPTY;
        while (!status.empty())status.pop();
        status.push(0);
        while (!signs.empty())signs.pop();
        m_Token = token_buffer[0];
        type = token2vtype(m_Token);

        while (getNextToken()) {
            execute();
        }
        return !errorFlag;
    }

    void LRParser::execute() {

        type = token2vtype(m_Token);

        while (true) {
            //printStack(status);
            switch (status.top()) {
                case 0: {
                    if (t_Type == TTYPE::S) {
                        status.push(1);
                        signs.push((int) TTYPE::S);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::ME) {
                        signs.push((int) TTYPE::ME);
                        status.push(14);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(10);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 1: {
                    if (type == VTYPE::END) {
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(2);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::NUM || type == VTYPE::L_PARENTHESES || type == VTYPE::IDENT) {
                        while (!signs.empty())signs.pop();
                        while (!status.empty())status.pop();
                        status.push(0);
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 2: {
                    if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::ME) {
                        signs.push((int) TTYPE::ME);
                        status.push(3);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(10);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    }
                    break;
                }
                case 3: {
                    CORE_INFO(m_Token.second);
                    if (type == VTYPE::END || type == VTYPE::P_SIGN || type == VTYPE::R_PARENTHESES ||
                        type == VTYPE::R_PARENTHESES_ || type == VTYPE::COMMA || type == VTYPE::NUM ||
                        type == VTYPE::L_PARENTHESES || type == VTYPE::IDENT) {
                        outSigns(3);
                        signs.push((int) TTYPE::S);
                        outStatus(3);
                        t_Type = TTYPE::S;
                    } else if (type == VTYPE::M_SIGN) {
                        status.push(4);
                        signs.push((int) VTYPE::M_SIGN);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 4: {
                    if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(15);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 5: {
                    outStatus(1);
                    outSigns(1);
                    signs.push((int) TTYPE::UO);
                    t_Type = TTYPE::UO;
                    break;
                }
                case 6: {
                    outSigns(1);
                    outStatus(1);
                    signs.push((int) TTYPE::PE);
                    t_Type = TTYPE::PE;
                    break;
                }
                case 7: {
                    if (t_Type == TTYPE::S) {
                        status.push(16);
                        signs.push((int) TTYPE::S);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::ME) {
                        signs.push((int) TTYPE::ME);
                        status.push(14);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(10);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 8: {
                    if (type == VTYPE::END || type == VTYPE::P_SIGN || type == VTYPE::M_SIGN ||
                        type == VTYPE::R_PARENTHESES || type == VTYPE::R_PARENTHESES_ || type == VTYPE::COMMA) {
                        outSigns(1);
                        outStatus(1);
                        signs.push((int) TTYPE::LV);
                        t_Type = TTYPE::LV;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(20);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES_) {
                        status.push(18);
                        signs.push((int) VTYPE::L_PARENTHESES_);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 9: {
                    outStatus(1);
                    outSigns(1);
                    signs.push((int) TTYPE::UE);
                    t_Type = TTYPE::UE;
                    break;
                }
                case 10: {
                    outStatus(1);
                    outSigns(1);
                    signs.push((int) TTYPE::ME);
                    t_Type = TTYPE::ME;
                    break;
                }
                case 11: {
                    if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(13);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 12: {
                    outStatus(1);
                    outSigns(1);
                    signs.push((int) TTYPE::PE);
                    t_Type = TTYPE::PE;
                    break;
                }
                case 13: {
                    outStatus(2);
                    outSigns(2);
                    signs.push((int) TTYPE::UE);
                    t_Type = TTYPE::UE;
                    break;
                }
                case 14: {
                    if (type == VTYPE::END || type == VTYPE::P_SIGN || type == VTYPE::R_PARENTHESES ||
                        type == VTYPE::R_PARENTHESES_ || type == VTYPE::COMMA) {
                        outStatus(1);
                        outSigns(1);
                        signs.push((int) TTYPE::S);
                        t_Type = TTYPE::S;
                    } else if (type == VTYPE::M_SIGN) {
                        status.push(4);
                        signs.push((int) VTYPE::M_SIGN);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 15: {
                    outStatus(3);
                    outSigns(3);
                    signs.push((int) TTYPE::ME);
                    t_Type = TTYPE::ME;
                    break;
                }
                case 16: {
                    if (type == VTYPE::R_PARENTHESES) {
                        status.push(17);
                        signs.push((int) VTYPE::R_PARENTHESES);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(2);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 17: {
                    outStatus(3);
                    outSigns(3);
                    signs.push((int) TTYPE::PE);
                    t_Type = TTYPE::PE;
                    break;
                }
                case 18: {
                    if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::ME) {
                        signs.push((int) TTYPE::ME);
                        status.push(14);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(10);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::S) {
                        signs.push((int) TTYPE::S);
                        status.push(19);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 19: {
                    if (type == VTYPE::R_PARENTHESES_) {
                        status.push(26);
                        signs.push((int) VTYPE::R_PARENTHESES_);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(2);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                }
                case 20: {
                    if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::ME) {
                        signs.push((int) TTYPE::ME);
                        status.push(14);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(10);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::S) {
                        status.push(21);
                        signs.push((int) TTYPE::S);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::FP) {
                        status.push(24);
                        signs.push((int) TTYPE::FP);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::R_PARENTHESES) {
                        status.push(27);
                        signs.push((int) VTYPE::R_PARENTHESES);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 21: {
                    if (type == VTYPE::R_PARENTHESES) {
                        outStatus(1);
                        outSigns(1);
                        signs.push((int) TTYPE::FP);
                        t_Type = TTYPE::FP;
                        break;
                    } else if (type == VTYPE::COMMA) {
                        status.push(22);
                        signs.push((int) VTYPE::COMMA);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                }
                case 22: {
                    if (t_Type == TTYPE::LV) {
                        signs.push((int) TTYPE::LV);
                        status.push(12);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::ME) {
                        signs.push((int) TTYPE::ME);
                        status.push(3);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UO) {
                        signs.push((int) TTYPE::UO);
                        status.push(11);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::UE) {
                        signs.push((int) TTYPE::UE);
                        status.push(10);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::PE) {
                        signs.push((int) TTYPE::PE);
                        status.push(9);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::S) {
                        status.push(21);
                        signs.push((int) TTYPE::S);
                        t_Type = TTYPE::EMPTY;
                    } else if (t_Type == TTYPE::FP) {
                        status.push(23);
                        signs.push((int) TTYPE::FP);
                        t_Type = TTYPE::EMPTY;
                    } else if (type == VTYPE::NUM) {
                        status.push(6);
                        signs.push((int) VTYPE::NUM);
                        return;
                    } else if (type == VTYPE::P_SIGN) {
                        status.push(5);
                        signs.push((int) VTYPE::P_SIGN);
                        return;
                    } else if (type == VTYPE::L_PARENTHESES) {
                        status.push(7);
                        signs.push((int) VTYPE::L_PARENTHESES);
                        return;
                    } else if (type == VTYPE::IDENT) {
                        status.push(8);
                        signs.push((int) VTYPE::IDENT);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                    break;
                }
                case 23: {
                    outStatus(3);
                    outSigns(3);
                    signs.push((int) (int) TTYPE::FP);
                    t_Type = TTYPE::FP;
                    break;
                }
                case 24: {
                    if (type == VTYPE::R_PARENTHESES) {
                        status.push(25);
                        signs.push((int) VTYPE::R_PARENTHESES);
                        return;
                    } else {
                        errorHandle();
                        return;
                    }
                }
                case 25: {
                    outStatus(4);
                    outSigns(4);
                    signs.push((int) (int) TTYPE::UE);
                    t_Type = TTYPE::UE;
                    break;
                }
                case 26: {
                    outStatus(4);
                    outSigns(4);
                    signs.push((int) (int) TTYPE::LV);
                    t_Type = TTYPE::LV;
                    break;
                }
                case 27: {
                    outStatus(3);
                    outSigns(3);
                    signs.push((int) (int) TTYPE::UE);
                    t_Type = TTYPE::UE;
                    break;
                }
            }
        }
    }

    bool LRParser::getNextToken() {
        if (m_Pos >= (int) token_buffer.size() - 1)
            return false;

        m_Token = token_buffer[++m_Pos];
        //CORE_INFO(m_Token.second);
        return true;
    }

    VTYPE LRParser::foreseenNextToken() {
        if (m_Pos >= token_buffer.size() - 1)
            return VTYPE::EMPTY;

        return token2vtype(token_buffer[m_Pos + 1]);
    }

    void LRParser::errorHandle() {
        errorFlag = true;
        CORE_ERROR("Line:" + std::to_string(pos_buffer[m_Pos][0]) + " CharBegin:" +
                   std::to_string(pos_buffer[m_Pos][1]) + " CharEnd:" +
                   std::to_string(pos_buffer[m_Pos][2]));
        CORE_ERROR("{0} is illegal here!", m_Token.second);
        while (foreseenNextToken() != VTYPE::NUM && foreseenNextToken() != VTYPE::IDENT
               && foreseenNextToken() != VTYPE::P_SIGN && foreseenNextToken() != VTYPE::L_PARENTHESES
               && getNextToken()) {}

        status = std::stack<int>();
        status.push(0);
        signs = std::stack<uint8_t>();
    }

    void LRParser::outSigns(int count) {
        for (int i = 0; i < count; i++)signs.pop();
    }

    void LRParser::outStatus(int count) {
        for (int i = 0; i < count; i++)status.pop();
    }
}