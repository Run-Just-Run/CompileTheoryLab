#include <iostream>
#include "Log.h"
#include"Scanner/src/Scanner.h"
#include "Parser/src/RecurParser.h"
#include "Log.h"
#include "Timer.h"
#include <filesystem>



int main() {
    Compiler::Log::Init("SCANNER");
    CORE_WARN("Initialize Log!");
    /*std::filesystem::path path("C:\\Users\\HP\\Desktop\\test");
    {
        Timer t;
        for (const auto &part: std::filesystem::directory_iterator(path)) {
            SCANNER::SCANNER::getScanner().setIn(part.path().string());
            SCANNER::SCANNER::getScanner().setOut("result_Scanner.txt");
            SCANNER::SCANNER::getScanner().Run();
        }
    }*/
    SCANNER::Scanner::getScanner().setIn("text.txt");
    SCANNER::Scanner::getScanner().setOut("result.txt");
    SCANNER::Scanner::getScanner().Run();
    Compiler::Log::Init("Parser");
    PARSER::Parser::setMode(PARSER::MODE::LRPARSER);
    PARSER::Parser::getParser().setTokenBuffer(SCANNER::Scanner::getScanner().getResult());
    PARSER::Parser::getParser().setPositionBuffer(SCANNER::Scanner::getScanner().getPositionInfo());
    std::cout << PARSER::Parser::getParser().Run() << std::endl;
    //RecurParser::PrintTree(RecurParser::RecurParser::getParser().getAST());


    system("pause");
}

