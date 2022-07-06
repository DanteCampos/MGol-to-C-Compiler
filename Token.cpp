#pragma once
#include <string>

class Token{
    public:
        std::string lex;
        std::string lex_class;
        std::string type;

        Token(){};
        Token(std::string lex, std::string lex_class, std::string type){
            this->lex = lex;
            this->lex_class = lex_class;
            this->type = type;
        }
};