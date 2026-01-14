#include <string>
#include <optional>
#include <iostream>

#include "lexer.hpp"

Token token(char value, TokenType tokentype)
{
    return {std::string(1, value), tokentype};
}

Token token(std::string value, TokenType tokentype)
{
    return {value, tokentype};
}

std::vector<Token> tokenize(const std::string sourceCode, const std::string file)
{
    std::vector<Token> tokens;
    std::vector<Diagnostic> diagnostics;
    std::optional<Token> current;
    char lastch = ' ';
    char ch = ' ';
    char nextch = ' ';
    std::size_t line = 0;
    std::size_t column = 0;
    bool isQuoted = false;
    bool isSingleLineCommented = false;
    bool isMultiLineCommented = false;

    auto flush = [&]()
    {
        if (current)
        {
            tokens.push_back(*current);
            current.reset();
        }
    };

    auto currentLocation = [&]()
    {
        return (SourceLocation){file, line, column};
    };

    for (int i = 0; i < (int)sourceCode.length(); i++)
    {
        lastch = sourceCode.at((i - 1) % sourceCode.length());
        ch = sourceCode.at(i);
        nextch = sourceCode.at((i + 1) % sourceCode.length());

        if (ch == '\n')
        {
            column = 0;
            line++;
        }

        // Multi character tokens

        // Single line comments (// comment)
        if (isSingleLineCommented && ch != '\n')
            ;

        // Multi line comments (/* comment \n comment */)
        else if (isMultiLineCommented && !(lastch == '*' && ch == '/'))
            ;

        // Quoted strings ("string")
        else if (isQuoted && ch != '"')
        {
            if (!current || current->type != TokenType::String)
            {
                flush();
                current = Token{std::string(1, ch), TokenType::String};
            }
            else
            {
                current->lexeme += ch;
            }
        }

        // Unquoted Strings or dots (.) or underscores (_)
        else if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '.' || ch == '_')
        {
            if (current && current->type == TokenType::Number)
            {
                current->type = TokenType::String;
                current->lexeme += ch;
            }
            else if (!current || current->type != TokenType::String)
            {
                flush();
                current = Token{std::string(1, ch), TokenType::String};
            }
            else
            {
                current->lexeme += ch;
            }
        }

        // Numbers mixed with any dots (.)
        else if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.')
        {
            if (!current || (current->type != TokenType::Number && current->type != TokenType::String))
            {
                flush();
                current = Token{std::string(1, ch), TokenType::Number};
            }
            else
            {
                current->lexeme += ch;
            }
        }

        // Single character tokens

        else
        {
            flush();

            // Whitespaces
            if (ch == ' ' || ch == '\t' || ch == '\r')
                ;
            else if (ch == '\n')
            {
                isSingleLineCommented = false;
            }

            else if (ch == '{')
                tokens.push_back(token(ch, TokenType::OpenBrace));

            else if (ch == '}')
                tokens.push_back(token(ch, TokenType::CloseBrace));

            else if (ch == '=')
                tokens.push_back(token(ch, TokenType::Equals));

            else if (ch == ';')
                tokens.push_back(token(ch, TokenType::Semicolon));

            else if (ch == '"')
            {
                tokens.push_back(token(ch, TokenType::Quote));
                isQuoted = !isQuoted;
            }
            else if (ch == '/' && nextch == '/')
                isSingleLineCommented = true;

            else if (ch == '/' && nextch == '*')
                isMultiLineCommented = true;

            else if (lastch == '*' && ch == '/')
                isMultiLineCommented = false;

            else
            {
                diagnostics.push_back({DiagnosticLevel::Error,
                                       currentLocation(),
                                       "Invalid character: \"" + std::string(1, ch) + "\""});
                tokens.push_back(token(ch, TokenType::Invalid));
            }
        }
        column++;
    }

    // Flush last token

    flush();

    if (!diagnostics.empty())
    {
        std::cerr << std::endl;
        for (Diagnostic d : diagnostics)
        {
            std::cerr << "\e[1mError:     " << d.message << "\e[0m" << std::endl;
            if (d.location.file != "")
                std::cerr << "in file:   \e[4m" << d.location.file << "\e[0m" << std::endl;
            std::cerr << "at line:   " << d.location.line + 1 << std::endl;
            std::cerr << "at column: " << d.location.column + 1 << std::endl;
            std::cerr << std::endl;
        }
        exit(1);
    }

    return tokens;
}
