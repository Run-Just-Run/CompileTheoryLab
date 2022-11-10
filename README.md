# CompileTheoryLab
How to use Scanner in the self-made Compiler:
```cpp
    SCANNER::Scanner::getScanner().setIn("text.txt");//set the source file.
    SCANNER::Scanner::getScanner().setOut("result.txt");//set the output file. Attention:this CANNOT be skipped!
    SCANNER::Scanner::getScanner().Run();//run the scanner, this will try to get the token pair required.
    SCANNER::Scanner::getScanner().getResult();//this return the result of the previous step in the form of a vector of container std::pair.
    //the first of the pair is the catagory code which will be explained later.the second is the exact content of the token in the form of std::string
    //please use this result as the source of your self-made Parser or the Parser we provided.(setTokenBuffer() if the Parser is what we provided.)
    SCANNER::Scanner::getScanner().getPositionInfo();//this return the result of the previous step in but is the position information of the tokens.
    //the result will be in the form of vector<vector<int>>the [0]of each vector<int> is line number .the [1] and [2] of each vector<int> are start pos and end pos.
```
if you are kind of confused.please refer to the main function we provided.<br>
catagory code:<br>
1:ident<br>
2:reserved words<br>
3:int<br>
4:operators<br>
5:all brackets<br>
6:',' and ';'<br>

