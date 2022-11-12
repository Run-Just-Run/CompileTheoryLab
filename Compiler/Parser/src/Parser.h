#pragma once
#include <iostream>
#include "../../log.h"
#include "../../Core.h"

namespace PARSER
{
    enum class MODE:uint8_t {
        LRPARSER,
        RECURPARSER
    };

    class Parser{
    public:
        Parser(){}
        Parser(const Parser&)=delete;
        Parser(const Parser&&)=delete;

        static Parser& getParser();

        static inline void setMode(MODE mode)
        {
            m_Mode = mode;
        }



        virtual inline void setTokenBuffer(const std::vector<std::pair<int,std::string>>&v) =0 ;
        virtual inline void setPositionBuffer(const std::vector<std::vector<int>>&v) =0 ;
        virtual bool Run() =0 ;

    private:
        static MODE m_Mode;
        static Parser*m_Parser;
    };
}
