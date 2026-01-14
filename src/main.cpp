#include <iostream>
#include <cstring>
#include <fstream>

#include "lexer.hpp"

int main(int argc, char *argv[])
{
    std::string sourceCode;
    std::ifstream inputFile("../input.hyp");
    std::string inputLines;
    while (getline(inputFile, inputLines))
    {
        sourceCode += inputLines + '\n';
    }
    inputFile.close();

    std::vector<Token> tokens;
    tokens = tokenize(sourceCode, "input.hyp");

    int maxTokenLength = 0;
    for (Token t : tokens)
    {
        maxTokenLength = std::max(maxTokenLength, (int)t.lexeme.length());
    }

    for (Token t : tokens)
    {
        std::cout << "Value: \e[7m" << t.lexeme << std::string(maxTokenLength - t.lexeme.length(), ' ') << "\e[0m | Type: " << t.type << std::endl;
    }
}
