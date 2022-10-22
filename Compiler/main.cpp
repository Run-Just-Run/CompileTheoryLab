#include <iostream>
#include "Log.h"
#include"Scanner/src/Scanner.h"
#include "Parser/src/Parser.h"
#include "Log.h"
#include "Timer.h"
#include <filesystem>



int main() {
    Compiler::Log::Init("Scanner");
    CORE_WARN("Initialize Log!");
    /*std::filesystem::path path("C:\\Users\\HP\\Desktop\\test");
    {
        Timer t;
        for (const auto &part: std::filesystem::directory_iterator(path)) {
            Scanner::Scanner::getScanner().setIn(part.path().string());
            Scanner::Scanner::getScanner().setOut("result_Scanner.txt");
            Scanner::Scanner::getScanner().Run();
        }
    }*/
    Scanner::Scanner::getScanner().setIn("test.txt");
    Scanner::Scanner::getScanner().setOut("result.txt");
    Scanner::Scanner::getScanner().Run();
    Compiler::Log::Init("Parser");
    Parser::Parser::getParser().setTokenBuffer(Scanner::Scanner::getScanner().getResult());
    Parser::Parser::getParser().setPositionBuffer(Scanner::Scanner::getScanner().getPositionInfo());
    std::cout<<Parser::Parser::getParser().Run()<<std::endl;
    Parser::PrintTree(Parser::Parser::getParser().getAST());


    system("pause");
}

