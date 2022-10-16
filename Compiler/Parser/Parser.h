#pragma once
//#define TIMETEST
#include <iostream>
#include <filesystem>
#include<fstream>
#include <unordered_set>
#include "../Log.h"

namespace Parser {
    class Parser {
    public:
        Parser(const Parser&)=delete;
        Parser(const Parser&&)=delete;
        static Parser& getParser()
        {
            static Parser m_Parser;
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

        void ParseS();
        void ParseADD();
        void ParseMUL();
        void ParsePE();
        void ParseADD_();
        void ParseMUL_();
        void ParseUE();
        void ParseNUM();
        void ParseLV();
        void ParseFP();
        void ParseUO_C();
        void ParseLV_();
        void ParseFP_();
        void ParseUE_();

        void errorHandle();
    private:
        Parser(){}
        std::vector<std::pair<int,std::string>>token_buffer;
        std::vector<std::vector<int>>pos_buffer;
        std::pair<int,std::string>m_Token;
        int position;
        bool errorFlag=0;
    private:
        void matchToken(std::pair<int,std::string>token);
    };
}

