#pragma once

#include <stack>
#include "Parser.h"

namespace PARSER {

    enum class VTYPE:uint8_t {
        P_SIGN=0      ,
        M_SIGN        , IDENT        , L_PARENTHESES , L_PARENTHESES_,
        R_PARENTHESES , NUM          ,
        R_PARENTHESES_, END          , COMMA          , EMPTY
    };

    enum class TTYPE:uint8_t {
        S_=11         , S             , ME            , UE            ,
        PE            , IDENT         , FP            , UO            , LV          , EMPTY
    };

    class LRParser:public Parser{
    public:
        LRParser(const LRParser&)=delete;
        LRParser(const LRParser&&)=delete;

        static LRParser& getParser()
        {
            static LRParser m_Parser;
            return m_Parser;
        }

        inline void setTokenBuffer(const std::vector<std::pair<int,std::string>>&v)
        {
            token_buffer=v;
            if (token_buffer.back().second!="#")
                token_buffer.push_back({5,"#"});
        }

        inline void setPositionBuffer(const std::vector<std::vector<int>>&v)
        {
            pos_buffer=v;
        }

        bool Run();
    private:
        bool getNextToken();
        VTYPE foreseenNextToken();

        void execute();

        void errorHandle();

        void outSigns(int count);
        void outStatus(int count);
    private:
        LRParser(): Parser(){}
        std::vector<std::pair<int,std::string>>token_buffer;

        std::vector<std::vector<int>>pos_buffer;
        std::pair<int,std::string>m_Token;

        std::stack<int>status;
        std::stack<uint8_t>signs;

        VTYPE type;
        TTYPE t_Type=TTYPE::EMPTY;

        int m_Pos = -1;
        bool errorFlag = 0;
    };

}


