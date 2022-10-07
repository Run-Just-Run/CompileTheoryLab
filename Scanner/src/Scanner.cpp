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
        else if (source == '.')                                                             return CHAR_TYPE::DOT;
        else if (source == '+'||source=='-')                                                return CHAR_TYPE::SIGN;
        else {
            SCANNER_CORE_ERROR("Unexpected Input '{0}' In Line: {1}, char: {2}",Scanner::getScanner().getChar(),Scanner::getScanner().getLine(),Scanner::getScanner().getCharCount());
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
            //SCANNER_CORE_INFO(m_Char);
            CHAR_TYPE c= char2CharType(m_Char);
            m_CharCount++;

            switch (m_State) {
                case STATE::STATE_BEGIN:
                {
                    m_Text="";
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {if(m_Text!="")OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE)                                              {m_State=STATE::STATE_IDENT;m_Text+=m_Char;}
                    else if (c==CHAR_TYPE::DIGIT&&m_Char!='0')                                                           {m_State=STATE::STATE_INT_DEC;m_Text+=m_Char;}
                    else if (m_Char=='0')                                                                                {m_State=STATE::STATE_PREFIX_ZERO;m_Text+=m_Char;}
                    else if (c==CHAR_TYPE::DOT)                                                                          {m_State=STATE::STATE_DEC_DOT_BEGIN;m_Text+=m_Char;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_DOT_BEGIN:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if(c==CHAR_TYPE::DIGIT)                                                                              {m_State=STATE::STATE_DEC_FLOAT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_PREFIX_ZERO:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='x'||m_Char=='X')                                                                   {m_State=STATE::STATE_INT_HEX;}
                    else if (m_Char<='7'&&m_Char>='0')                                                                   {m_State=STATE::STATE_INT_OCT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_INT_HEX_PRE:
                {
                    m_Text+=m_Char;

                    if      ((c==CHAR_TYPE::DIGIT&&m_Char!='0')||(m_Char<='f'&&m_Char>='a')||(m_Char<='F'&&m_Char>='A')) {m_State=STATE::STATE_INT_HEX;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_INT_HEX:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::DIGIT||(m_Char<='f'&&m_Char>='a')||(m_Char<='F'&&m_Char>='A'))                {m_State=STATE::STATE_INT_HEX;}
                    else if (m_Char=='p'||m_Char=='P')                                                                   {m_State=STATE::STATE_HEX_FLOAT_PRE;}
                    else if (c==CHAR_TYPE::DOT)                                                                          {m_State=STATE::STATE_HEX_DOT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_HEX_FLOAT_PRE:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_HEX_FLOAT_READY;}
                    else if (m_Char=='+'||m_Char=='-')                                                                   {m_State=STATE::STATE_HEX_FLOAT_SIGN;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_HEX_FLOAT_SIGN:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_HEX_FLOAT_READY;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_HEX_FLOAT_READY:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_HEX_FLOAT_READY;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_HEX_DOT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::DIGIT||(m_Char<='f'&&m_Char>='a')||(m_Char<='F'&&m_Char>='A'))                {m_State=STATE::STATE_HEX_FLOAT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_HEX_FLOAT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::DIGIT||(m_Char<='f'&&m_Char>='a')||(m_Char<='F'&&m_Char>='A'))                {m_State=STATE::STATE_HEX_FLOAT;}
                    else if (m_Char=='p'||m_Char=='P')                                                                   {m_State=STATE::STATE_HEX_FLOAT_PRE;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_INT_OCT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char<='7'&&m_Char>='0')                                                                   {m_State=STATE::STATE_INT_HEX;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_INT_DEC:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_INT_DEC;}
                    else if (c==CHAR_TYPE::DOT)                                                                          {m_State=STATE::STATE_DEC_DOT;}
                    else if (m_Char=='e'||m_Char=='E')                                                                   {m_State=STATE::STATE_DEC_FLOAT_PRE;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_DOT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_DEC_FLOAT;}
                    else if (m_Char=='e'||m_Char=='E')                                                                   {m_State=STATE::STATE_DEC_FLOAT_PRE;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_FLOAT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_DEC_FLOAT;}
                    else if (m_Char=='e'||m_Char=='E')                                                                   {m_State=STATE::STATE_DEC_FLOAT_PRE;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_FLOAT_PRE:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if(c==CHAR_TYPE::DIGIT)                                                                              {m_State=STATE::STATE_DEC_FLOAT;}
                    else if(m_Char=='-'||m_Char=='+')                                                                    {m_State=STATE::STATE_DEC_FLOAT_SIGN;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_FLOAT_SIGN:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE)
                        m_Text+=m_Char;
                    if(c==CHAR_TYPE::DIGIT)                                                                              {m_State=STATE::STATE_DEC_FLOAT;}
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
        if(m_State == STATE::STATE_IDENT) {
            if(Reserved_Words.find(m_Text)!=Reserved_Words.end()) {
                reportResult(2);
            }
            else
            {
                reportResult(1);
            }
        }
        else if(m_State == STATE::STATE_INT_DEC  ||m_State == STATE::STATE_INT_HEX||
                m_State == STATE::STATE_INT_OCT  ||m_State == STATE::STATE_PREFIX_ZERO||
                m_State == STATE::STATE_DEC_DOT  ||m_State == STATE::STATE_DEC_FLOAT||
                m_State == STATE::STATE_HEX_FLOAT_READY){
                reportResult(3);
        }
    }

    void Scanner::reportError()
    {
        SCANNER_CORE_INFO("Line: {0}", m_Line);
        SCANNER_CORE_ERROR("Unexpected Input '{0}' In Line: {1}, char: {2}",getChar(),getLine(),getCharCount());
        m_State = STATE::STATE_EMPTY;
        Reset();
    }

    void Scanner::reportResult(int categoryCode)
    {
        SCANNER_CORE_INFO("Line: {0}", m_Line);
        SCANNER_CORE_INFO("CharBegin: {0}, CharEnd: {1}", m_CharCount - m_Text.size(), m_CharCount-1);
        SCANNER_CORE_INFO("({0} , {1})", categoryCode, m_Text);
        (*m_file_out)<<"("<<categoryCode<<" , "+m_Text+")"<<std::endl;
    }
}
