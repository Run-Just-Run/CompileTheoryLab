#include "Scanner.h"

namespace Scanner
{

    std::unordered_set<std::string> Scanner::Reserved_Words = std::unordered_set<std::string>
            ({
                "int",      "float",    "const",
                "main",     "void",     "break",
                "return",   "if",       "else",
                "for",      "while",    "do",
                            "continue"
            });

    CHAR_TYPE char2CharType(char source)
    {
        if      ((source <= 'Z' && source >= 'A')||(source <= 'z' && source >= 'a'))        return CHAR_TYPE::ALPHA;
        else if (source >= '0'&& source <= '9')                                             return CHAR_TYPE::DIGIT;
        else if (source == '\n'|| source == '\r')                                           return CHAR_TYPE::NEWLINE;
        else if (source == '\t'|| source == ' '|| source == '\012'|| source == '\b')        return CHAR_TYPE::WHITE_SPACE_CHAR;
        else if (source == '_')                                                             return CHAR_TYPE::UNDERSCORE;
        else {
            SCANNER_CORE_ERROR("Unexpected Input '{0}' In Line: {1}, char: {2}",
                               Scanner::getScanner().getChar(),Scanner::getScanner().getLine(),Scanner::getScanner().getCharCount());
            return CHAR_TYPE::UNKNOWN_TYPE;
        }
    }

    Scanner::Scanner()
    {
        m_CharCount = 0;
        m_Line = 1;
        m_Char = ' ';
        m_Text="";
    }

    Scanner::~Scanner()
    {
        if(m_file_in!= nullptr) delete m_file_in;
        if(m_file_out!= nullptr)delete m_file_out;
    }

    void Scanner::Reset()
    {
        m_CharCount = 0;
        m_Line = 1;
        m_Char = ' ';
        m_Text="";
        m_State=STATE::STATE_EMPTY;
    }

    void Scanner::Run()
    {
        if(m_file_in==nullptr||m_file_out==nullptr)
            SCANNER_CORE_ERROR("Unspecified IO file!");
        m_State=STATE::STATE_BEGIN;
        while(m_file_in->read(&m_Char,1)&&m_Char!=EOF)
        {
            CHAR_TYPE c= char2CharType(m_Char);
            m_CharCount++;

            switch (m_State) {
                case STATE::STATE_BEGIN:
                {
                    m_Text="";
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE)                                              {m_State=STATE::STATE_IDENT;m_Text+=m_Char;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_IDENT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                    m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE||c==CHAR_TYPE::DIGIT)                         {m_State=STATE::STATE_IDENT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_EMPTY:
                {
                    break;
                }
            }
            if(c==CHAR_TYPE::NEWLINE)m_Line++;
        }
        if(m_Text!="")OutputInfo();
        Reset();
    }

    void Scanner::OutputInfo()
    {
        SCANNER_CORE_INFO("Line: {0}", m_Line);
        if(m_State == STATE::STATE_IDENT) {
            if(Reserved_Words.find(m_Text)!=Reserved_Words.end()) {
                reportResult(2);
            }
            else
            {
                reportResult(1);
            }
        }
    }

    void Scanner::reportError()
    {
        SCANNER_CORE_ERROR("Unexpected Input '{0}' In Line: {1}, char: {2}",getChar(),getLine(),getCharCount());
        m_State = STATE::STATE_EMPTY;
        Reset();
    }

    void Scanner::reportResult(int categoryCode)
    {
        SCANNER_CORE_INFO("CharBegin: {0}, CharEnd: {1}", m_CharCount - m_Text.size(), m_CharCount);
        SCANNER_CORE_INFO("({0} , {1})", categoryCode, m_Text);
        (*m_file_out)<<"("<<categoryCode<<" , "+m_Text+")"<<std::endl;
    }
}
