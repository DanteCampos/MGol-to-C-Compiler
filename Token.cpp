#pragma once
#include <string>

typedef struct Token{
    std::string lex;
    std::string lex_class;
    std::string type;
    int line;
    int column;
} Token;