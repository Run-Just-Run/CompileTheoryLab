#include <iostream>
#include "Log.h"
#include"Scanner/src/Scanner.h"
#include "Parser/src/RecurParser.h"
#include "Log.h"
#include "Timer.h"
#include <filesystem>
#include "Parser/src/TreeResolver.h"



int main() {
    SCANNER::Scanner::getScanner().setIn("text.txt");
    SCANNER::Scanner::getScanner().setOut("result.txt");
    SCANNER::Scanner::getScanner().Run();

    PARSER::Parser::setMode(PARSER::MODE::RECURPARSER);
    PARSER::Parser::getParser().setTokenBuffer(SCANNER::Scanner::getScanner().getResult());
    PARSER::Parser::getParser().setPositionBuffer(SCANNER::Scanner::getScanner().getPositionInfo());
    PARSER::Parser::getParser().Run();
    PARSER::TreeResolver::resolve(PARSER::RecurParser::getParser().getAST());

    //RecurParser::PrintTree(RecurParser::RecurParser::getParser().getAST());


    system("pause");
}

