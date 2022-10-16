#include "Parser.h"
namespace Parser
{
    bool Parser::Run() {
        position=0;
        m_Token=token_buffer[0];
        while(position!=token_buffer.size()-1) {
            ParseS();
        }
        return true;
    }

    void Parser::ParseS() {
        errorFlag=0;
        if(m_Token.first==1||m_Token.first==3||m_Token.second=="("||m_Token.second=="+"||m_Token.second=="-")
            ParseADD();
        else
            errorHandle();
    }

    void Parser::ParseADD() {
        if(errorFlag)return;
        if(m_Token.first==1||m_Token.first==3||m_Token.second=="("||m_Token.second=="+"||m_Token.second=="-") {
            ParseMUL();
            ParseADD_();
        }
        else
            errorHandle();
    }

    void Parser::ParseMUL() {
        if(errorFlag)return;
        if(m_Token.first==1||m_Token.first==3||m_Token.second=="("||m_Token.second=="+"||m_Token.second=="-") {
            ParseUE();
            ParseMUL_();
        }
        else
            errorHandle();
    }

    void Parser::ParsePE() {
        if(errorFlag)return;
        if(m_Token.second=="(") {
            matchToken({-1,"("});
            ParseADD();
            matchToken({-1,")"});
        }
        else if(m_Token.first==1)
            ParseLV();
        else if(m_Token.first==3)
            ParseNUM();
        else
            errorHandle();
    }

    void Parser::ParseADD_() {
        if(errorFlag)return;
        if(m_Token.second=="+"||m_Token.second=="-")
        {
            matchToken({-1,m_Token.second});
            ParseMUL();
            ParseADD_();
        }
        else if(m_Token.second=="#"||m_Token.second==")"||m_Token.second=="]"||m_Token.second==",")
        {}
        else if(m_Token.first==1||m_Token.first==3||m_Token.second=="("||m_Token.second=="+"||m_Token.second=="-"){}
        else
            errorHandle();
    }

    void Parser::ParseMUL_() {
        if(errorFlag)return;
        if(m_Token.second=="*"||m_Token.second=="/"||m_Token.second=="%")
        {
            matchToken({-1,m_Token.second});
            ParseUE();
            ParseMUL_();
        }
        else if(m_Token.second=="+"||m_Token.second=="-"||m_Token.second=="#"||m_Token.second==")"||m_Token.second=="]"||m_Token.second==",")
        {}
        else if(m_Token.first==1||m_Token.first==3||m_Token.second=="("||m_Token.second=="+"||m_Token.second=="-"){}
        else
            errorHandle();
    }

    void Parser::ParseUE() {
        if(errorFlag)return;
        if(m_Token.first==1)
        {
            matchToken({1,""});
            ParseUE_();
        }
        else if(m_Token.second=="+"||m_Token.second=="-")
        {
            ParseUO_C();
            ParseUE();
        }
        else if(m_Token.second=="("||m_Token.first==3)
        {
            ParsePE();
        }
        else
        {
            errorHandle();
        }
    }

    void Parser::ParseNUM() {
        if(errorFlag)return;
        if(m_Token.first==3) {
            matchToken({3, ""});
        }
        else
            errorHandle();

    }

    void Parser::ParseLV() {
        if(errorFlag)return;
        if(m_Token.first==1) {
            matchToken({1, ""});
            ParseLV_();
        }
        else
            errorHandle();
    }

    void Parser::ParseFP() {
        if(errorFlag)return;
        if(m_Token.first==1||m_Token.first==3||m_Token.second=="("||m_Token.second=="+"||m_Token.second=="-") {
            ParseADD();
            ParseFP_();
        }
        else
            errorHandle();
    }

    void Parser::ParseUO_C() {
        if(errorFlag)return;
        if(m_Token.second=="+")
        {
            matchToken({-1,"+"});
        }
        else if(m_Token.second=="-")
        {
            matchToken({-1,"-"});
        }
        else
            errorHandle();
    }

    void Parser::matchToken(std::pair<int, std::string> token) {
        if(errorFlag)return;
        std::cout<<m_Token.second<<std::endl;
        if(token.first!=-1)
        {
            if(token.first == m_Token.first)position++;
            else errorHandle();
        }
        else
        {
            if(token.second == m_Token.second)position++;
            else errorHandle();
        }
        if(position== token_buffer.size())
                return;
        m_Token=token_buffer[position];
    }

    void Parser::ParseLV_() {
        if(errorFlag)return;
        if(m_Token.second=="[")
        {
            matchToken({-1,"["});
            ParseADD();
            matchToken({-1,"]"});
        }
        else if(m_Token.second=="%"||m_Token.second=="/"||m_Token.second=="*"||m_Token.second=="+"||m_Token.second=="-"||m_Token.second=="#")
        {}
        else
            errorHandle();
    }

    void Parser::ParseFP_() {
        if(errorFlag)return;
        if(m_Token.second==",")
        {
            matchToken({-1,","});
            ParseADD();
            ParseFP_();
        }
        else if(m_Token.second==")")
        {}
        else
            errorHandle();
    }

    void Parser::ParseUE_() {
        if(errorFlag)return;
        if(m_Token.second=="+"||m_Token.second=="-"||m_Token.second=="*"||m_Token.second=="/"||m_Token.second=="%"||m_Token.second==","||m_Token.second==")")
        {}
        else if(m_Token.second=="(")
        {
            matchToken({-1,"("});
            ParseFP();
            matchToken({-1,")"});
        }
        else if(m_Token.second=="[")
        {
            matchToken({-1,"["});
            ParseADD();
            matchToken({-1,"]"});
        }
        else
            errorHandle();
    }

    void Parser::errorHandle() {
        //std::cout<<pos_buffer.size()<<" "<<position<<std::endl;
        CORE_ERROR("Line:"+ std::to_string(pos_buffer[position][0])+" CharBegin:"+ std::to_string(pos_buffer[position][1])+" CharEnd:"+
        std::to_string(pos_buffer[position][2]));
        CORE_ERROR("Syntax Error! "+m_Token.second+" is illegal here!");
        while(position<token_buffer.size()&&m_Token.first!=1&&m_Token.first!=3&&m_Token.second!="("&&m_Token.second!="+"&&m_Token.second!="-")
        {
            position++;
            m_Token=token_buffer[position];
        }
        errorFlag=1;
    }

}