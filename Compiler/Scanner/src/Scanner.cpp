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

    CHAR_TYPE Scanner::char2CharType(char source)
    {
        if      ((source <= 'Z' && source >= 'A')||(source <= 'z' && source >= 'a'))        return CHAR_TYPE::ALPHA;
        else if (source >= '0'&& source <= '9')                                             return CHAR_TYPE::DIGIT;
        else if (source == '\n'|| source == '\r')                                           return CHAR_TYPE::NEWLINE;
        else if (source == '\t'|| source == ' '|| source == '\012'|| source == '\b')        return CHAR_TYPE::WHITE_SPACE_CHAR;
        else if (source == '_')                                                             return CHAR_TYPE::UNDERSCORE;
        else if (source == '.')                                                             return CHAR_TYPE::DOT;
        else if (source == '/')                                                             return CHAR_TYPE::SLASH;
        else if (source == '+'||source == '-'||
                 source == '*'||source == '['||
                 source == ']'||source == '{'||
                 source == '}'||source == '('||
                 source == ')'||source == ','||
                 source == ';')                                                             return CHAR_TYPE::SINGLE_SIGN;
        else if (source == '|'||source == '&'||
                 source == '!'||source == '>'||
                 source == '<'||source == '=')                                              return CHAR_TYPE::DOUBLE_SIGN;
        else if (m_State==STATE::STATE_COMMENT_LINE||
                 m_State==STATE::STATE_COMMENT_PARAGRAPH)                                   return CHAR_TYPE::UNKNOWN_TYPE;
        else {
            CORE_ERROR("Unexpected Input '{0}' In Line: {1}, char: {2}",Scanner::getScanner().getChar(),Scanner::getScanner().getLine(),Scanner::getScanner().getCharCount());
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
    #ifndef TIMETEST
        m_CharCount = 0;
    #endif
        m_Line = 1;
        m_Char = ' ';
        m_Text="";
        m_State=STATE::STATE_EMPTY;
    }

    void Scanner::Run()
    {
        if(m_file_in==nullptr||m_file_out==nullptr)
            CORE_ERROR("Unspecified IO file!");
        m_State=STATE::STATE_BEGIN;
        while(m_file_in->read(&m_Char,1)&&m_Char!=EOF)
        {
            //CORE_INFO(m_Char);
            CHAR_TYPE c= char2CharType(m_Char);
            m_CharCount++;

            switch (m_State) {
                case STATE::STATE_BEGIN:
                {
                    m_Text="";
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text+=m_Char;m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text+=m_Char;m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_State=STATE::STATE_WAIT_EQUAL;m_Text+=m_Char;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE)                                              {m_State=STATE::STATE_IDENT;m_Text+=m_Char;}
                    else if (c==CHAR_TYPE::DIGIT&&m_Char!='0')                                                           {m_State=STATE::STATE_INT_DEC;m_Text+=m_Char;}
                    else if (m_Char=='0')                                                                                {m_State=STATE::STATE_PREFIX_ZERO;m_Text+=m_Char;}
                    else if (c==CHAR_TYPE::DOT)                                                                          {m_State=STATE::STATE_DEC_DOT_BEGIN;m_Text+=m_Char;}
                    else if (m_Char == '/')                                                                              {m_State=STATE::STATE_TO_COMMENT;}
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
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
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
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
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
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
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
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
                    else if (m_Char<='7'&&m_Char>='0')                                                                   {m_State=STATE::STATE_INT_HEX;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_INT_DEC:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
                    else if (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_INT_DEC;}
                    else if (c==CHAR_TYPE::DOT)                                                                          {m_State=STATE::STATE_DEC_DOT;}
                    else if (m_Char=='e'||m_Char=='E')                                                                   {m_State=STATE::STATE_DEC_FLOAT_PRE;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_DOT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
                    else if (c==CHAR_TYPE::DIGIT)                                                                        {m_State=STATE::STATE_DEC_FLOAT;}
                    else if (m_Char=='e'||m_Char=='E')                                                                   {m_State=STATE::STATE_DEC_FLOAT_PRE;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_DEC_FLOAT:
                {
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                        m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
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
                    if(c!=CHAR_TYPE::WHITE_SPACE_CHAR&&c!=CHAR_TYPE::NEWLINE&&m_Char!='/'&&c!=CHAR_TYPE::SINGLE_SIGN)
                    m_Text+=m_Char;
                    if      (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char == '/')                                                                              {OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {m_Text.pop_back();OutputInfo();m_Text="";m_Text+=m_Char;m_State=STATE::STATE_WAIT_EQUAL;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE||c==CHAR_TYPE::DIGIT)                         {m_State=STATE::STATE_IDENT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_TO_COMMENT:
                {
                    m_Text="";
                    if(c!=CHAR_TYPE::NEWLINE&&c!=CHAR_TYPE::WHITE_SPACE_CHAR&&m_Char!='/'&&m_Char!='*')
                        m_Text+=m_Char;
                    if      (m_Char=='/')                                                                                {m_State=STATE::STATE_COMMENT_LINE;}
                    else if (m_Char=='*')                                                                                {m_State=STATE::STATE_COMMENT_PARAGRAPH;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {char temp = m_Char;m_Char='/';OutputInfo();m_State=STATE::STATE_BEGIN;m_Char=temp;OutputInfo();}
                    else if (m_Char=='<'||m_Char=='>'||m_Char=='!'||m_Char=='=')                                         {char temp = m_Char;m_Char='/';OutputInfo();m_Text="";m_Text+=temp;m_State=STATE::STATE_WAIT_EQUAL;}
                    else if (m_Char=='|')                                                                                {m_Char='/';OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Char='/';OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else if (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {m_Char='/';OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::DIGIT&&m_Char!='0')                                                           {m_Char='/';OutputInfo();m_State=STATE::STATE_INT_DEC;}
                    else if (m_Char=='0')                                                                                {m_Char='/';OutputInfo();m_State=STATE::STATE_PREFIX_ZERO;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE)                                              {m_Char='/';OutputInfo();m_State=STATE::STATE_IDENT;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_COMMENT_PARAGRAPH:
                {
                    if(m_Char=='*')                                                                                      {m_State=STATE::STATE_END_COMMENT;}
                    else                                                                                                 {m_State=STATE::STATE_COMMENT_PARAGRAPH;}
                    break;
                }
                case STATE::STATE_COMMENT_LINE:
                {
                    if(c==CHAR_TYPE::NEWLINE)                                                                            {m_State=STATE::STATE_BEGIN;}
                    else                                                                                                 {m_State=STATE::STATE_COMMENT_LINE;}
                    break;
                }
                case STATE::STATE_END_COMMENT:
                {
                    if(m_Char=='/')                                                                                      {m_State=STATE::STATE_BEGIN;}
                    else                                                                                                 {m_State=STATE::STATE_COMMENT_PARAGRAPH;}
                    break;
                }
                case STATE::STATE_WAIT_EQUAL:
                {
                    if(c!=CHAR_TYPE::NEWLINE&&c!=CHAR_TYPE::WHITE_SPACE_CHAR&&m_Char!='/'&&m_Char!='*')
                        m_Text+=m_Char;
                    if      (m_Char=='=')                                                                                {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::NEWLINE||c==CHAR_TYPE::WHITE_SPACE_CHAR)                                      {m_Char=m_Text[0];OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::SINGLE_SIGN)                                                                  {m_Text.pop_back();OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else if (c==CHAR_TYPE::ALPHA||c==CHAR_TYPE::UNDERSCORE)                                              {m_Char=m_Text[0];m_Text=m_Text.substr(1,m_Text.size()-1);OutputInfo();m_State=STATE::STATE_IDENT;}
                    else if (c==CHAR_TYPE::DIGIT&&m_Char!='0')                                                           {m_Char=m_Text[0];m_Text=m_Text.substr(1,m_Text.size()-1);OutputInfo();m_State=STATE::STATE_INT_DEC;}
                    else if (m_Char=='0')                                                                                {m_Char=m_Text[0];m_Text=m_Text.substr(1,m_Text.size()-1);OutputInfo();m_State=STATE::STATE_PREFIX_ZERO;}
                    else if (c==CHAR_TYPE::DOT)                                                                          {m_Char=m_Text[0];m_Text=m_Text.substr(1,m_Text.size()-1);OutputInfo();m_State=STATE::STATE_DEC_DOT_BEGIN;}
                    else if (m_Char=='/')                                                                                {m_Char=m_Text[0];m_Text="";OutputInfo();m_State=STATE::STATE_TO_COMMENT;}
                    else if (m_Char=='|')                                                                                {m_Text.pop_back();m_Char=m_Text[0];OutputInfo();m_Text="|";m_State=STATE::STATE_WAIT_OR;}
                    else if (m_Char=='&')                                                                                {m_Text.pop_back();m_Char=m_Text[0];OutputInfo();m_Text="&";m_State=STATE::STATE_WAIT_AND;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_WAIT_AND:
                {
                    if(c!=CHAR_TYPE::NEWLINE&&c!=CHAR_TYPE::WHITE_SPACE_CHAR&&m_Char!='/'&&m_Char!='*')
                        m_Text+=m_Char;
                    if      (m_Char=='&')                                                                                {OutputInfo();m_State=STATE::STATE_BEGIN;}
                    else                                                                                                 {reportError();}
                    break;
                }
                case STATE::STATE_WAIT_OR:
                {
                    if(c!=CHAR_TYPE::NEWLINE&&c!=CHAR_TYPE::WHITE_SPACE_CHAR&&m_Char!='/'&&m_Char!='*')
                        m_Text+=m_Char;
                    if      (m_Char=='|')                                                                                {OutputInfo();m_State=STATE::STATE_BEGIN;}
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
    #ifdef TIMETEST
        std::cout<<m_CharCount<<std::endl;
    #endif
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
                m_State == STATE::STATE_HEX_FLOAT_READY) {
            reportResult(3);
        }
        else if(m_State==STATE::STATE_WAIT_EQUAL||m_State==STATE::STATE_WAIT_OR||m_State==STATE::STATE_WAIT_AND)
        {
            if(m_Text.size()==2)
            {
                reportResult(4);
            }
            else
            {
                char temp_c = m_Char;
                if(char2CharType(m_Char)==CHAR_TYPE::SINGLE_SIGN)
                    temp_c=m_Text[0];
                std::string temp="";
                temp+=temp_c;
                reportResult(4,temp);
            }
        }
        else if(m_Char=='/'){
            reportResult(4,"/");
        }

        if(m_Char=='+'||m_Char=='-'||m_Char=='*')
        {
            std::string temp="";
            temp+=m_Char;
            reportResult(4,temp);
        }
        if(m_Char=='{'||m_Char=='}'||m_Char=='['||m_Char==']'||m_Char=='('||m_Char==')')
        {
            std::string temp="";
            temp+=m_Char;
            reportResult(5,temp);
        }
        if(m_Char==','||m_Char==';')
        {
            std::string temp="";
            temp+=m_Char;
            reportResult(6,temp);
        }
    }

    void Scanner::reportError()
    {
        CORE_INFO("Line: {0}", m_Line);
        CORE_ERROR("Unexpected Input '{0}' In Line: {1}, char: {2}",getChar(),getLine(),getCharCount());
        m_State = STATE::STATE_BEGIN;
        //Reset();
    }

    void Scanner::reportResult(int categoryCode,std::string text)
    {
        if(text=="")text=m_Text;

        CORE_INFO("Line: {0}", m_Line);
        CORE_INFO("CharBegin: {0}, CharEnd: {1}", m_CharCount - text.size(), m_CharCount-1);
        CORE_INFO("({0} , {1})", categoryCode, text);
        result_buffer.push_back({categoryCode,text});
        position_buffer.push_back({m_Line,m_CharCount - (int)text.size(),m_CharCount-1});
        (*m_file_out)<<"("<<categoryCode<<" , "+text+")"<<std::endl;
    }
}
