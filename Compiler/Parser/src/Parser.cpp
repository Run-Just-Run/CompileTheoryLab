#include "LRParser.h"
#include "RecurParser.h"
#include "Parser.h"

namespace PARSER{
    MODE Parser::m_Mode = MODE::RECURPARSER;
    Parser* Parser::m_Parser = nullptr;

    Parser& Parser::getParser() {

        switch (m_Mode) {
            case MODE::LRPARSER:{
                m_Parser = &LRParser::getParser();
                return LRParser::getParser();
                break;
            }
            case MODE::RECURPARSER:{
                m_Parser = &RecurParser::getParser();
                return RecurParser::getParser();
                break;
            }
            default:{
                CORE_ASSERT(true,"unknown parser mode");
                return *m_Parser;
            }
        }
    }
}

