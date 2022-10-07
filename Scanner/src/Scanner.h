#pragma once
#include <filesystem>
#include<fstream>
#include <unordered_set>
#include "Log.h"

namespace Scanner {

    enum class CHAR_TYPE:uint8_t
    {
        ALPHA = 1,
        DIGIT,
        NEWLINE,
        WHITE_SPACE_CHAR,
        UNDERSCORE,
        DOT,
        SIGN,
        UNKNOWN_TYPE
    };

    class Scanner {
    public:
        Scanner(const Scanner&)=delete;
        Scanner(const Scanner&&)=delete;
        ~Scanner();

        enum class STATE:uint8_t
        {
            STATE_BEGIN,
            STATE_IDENT,
            STATE_PREFIX_ZERO,
            STATE_INT_DEC,
            STATE_INT_OCT,
            STATE_INT_HEX_PRE,
            STATE_INT_HEX,
            STATE_DEC_DOT,
            STATE_DEC_DOT_BEGIN,
            STATE_HEX_DOT,
            STATE_HEX_FLOAT_PRE,
            STATE_HEX_FLOAT,
            STATE_HEX_FLOAT_SIGN,
            STATE_HEX_FLOAT_READY,
            STATE_DEC_FLOAT,
            STATE_DEC_FLOAT_PRE,
            STATE_DEC_FLOAT_SIGN,
            STATE_COMMENT_LINE,
            STATE_COMMENT_PARAGRAPH,
            STATE_TO_COMMENT,
            STATE_END_COMMENT,
            STATE_EMPTY
        };

        static Scanner& getScanner()
        {
            static Scanner m_Scanner;
            return m_Scanner;
        }

        void Reset();

        inline char getChar()const {return m_Char;}
        inline int  getCharCount()const {return m_CharCount;}
        inline int  getLine()const {return m_Line;}
        inline void setIn(std::string inSourse)
        {
            if(m_file_in!= nullptr)
                m_file_in->close();
            else {
                m_file_in = new std::fstream();
            }
            m_file_in->open(inSourse, std::ios::in);
            if(!(m_file_in->is_open()))SCANNER_CORE_ERROR("Source File Not Found!");
        }
        inline void setOut(std::string outFile)
        {
            if(m_file_out!= nullptr)
                m_file_out->close();
            else {
                m_file_out = new std::fstream();
            }
            m_file_out->open(outFile, std::ios::out);
            if(!(m_file_out->is_open()))SCANNER_CORE_ERROR("Out File Open Error!");
        }

        CHAR_TYPE char2CharType(char source);

        void Run();

        void OutputInfo();

        void reportError();
        void reportResult(int categoryCode, std::string text ="");

    private:
        Scanner();

        int m_Line;
        int m_CharCount;
        char m_Char;
        std::string m_Text;

        STATE m_State=STATE::STATE_EMPTY;

        std::fstream* m_file_in=nullptr;
        std::fstream* m_file_out=nullptr;

        static std::unordered_set<std::string>Reserved_Words;
    };
}



