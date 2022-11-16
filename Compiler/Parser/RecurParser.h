#pragma once
//#define TIMETEST
#include <iostream>
#include<fstream>
#include <unordered_set>
#include <stack>
#include "../../Log.h"
#include "ASTNode.h"
#include "Parser.h"

namespace PARSER {

    class RecurParser:public Parser{
    public:
        RecurParser(const RecurParser&)=delete;
        RecurParser(const RecurParser&&)=delete;
        static RecurParser& getParser()
        {
            static RecurParser m_Parser;
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
        inline ASTNode* getAST(){
            //std::cout<<root<<std::endl;
            return root;
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
        void ParseCD();
        void ParseCD_();
        void ParseCD__();
        void ParseCD___();
        void ParseIV();
        void ParseIV_();
        void ParseVD();
        void ParseCOND();
        void ParseRE();
        void ParseRE_();

        void ParseLOE();
        void ParseLOE_();
        void ParseLAE();
        void ParseLAE_();
        void ParseEE();
        void ParseEE_();
        void ParseEQ();
        void ParseUE_C();

        void errorHandle();
    private:
        RecurParser(): Parser(){}
        std::vector<std::pair<int,std::string>>token_buffer;
        std::vector<std::vector<int>>pos_buffer;
        std::pair<int,std::string>m_Token;
        int position;
        bool errorFlag=0;
        bool finalCheck=1;
        int urgency=0;

        bool inADD=0;
        ASTNode*root;
        std::stack<ASTNode*>AST_stack;

        bool constSituation = 0;
    private:
        void matchToken(std::pair<int,std::string>token);
        void creatTree(std::pair<int,std::string>token);
    };

    void PrintTree(ASTNode*);
    bool Prior(std::string top, std::string now);
    void Reverse_Polish(std::vector<std::pair<int, std::string>>token_buffer);

}

