#include <iostream>
#include "Log.h"
#include"Scanner.h"



int main() {
    Scanner::Log::Init();
    Scanner::SCANNER_CORE_WARN("Initialize Log!");

    Scanner::Scanner::getScanner().setIn("test.txt");
    Scanner::Scanner::getScanner().setOut("result.txt");
    Scanner::Scanner::getScanner().Run();
    system("pause");
}

