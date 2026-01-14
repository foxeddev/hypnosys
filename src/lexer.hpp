#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

struct SourceLocation
{
    std::string file;
    std::size_t line;
    std::size_t column;
};

enum class DiagnosticLevel
{
    Error,
    Warning
};

struct Diagnostic
{
    DiagnosticLevel level;
    SourceLocation location;
    std::string message;
};

enum TokenType
{
    Number,
    String,
    Equals,
    OpenBrace,
    CloseBrace,
    Semicolon,
    Quote,
    Invalid,
};

struct Token
{
    std::string lexeme;
    TokenType type;
    SourceLocation location;
};

std::vector<Token> tokenize(const std::string sourceCode, const std::string file);

#endif
