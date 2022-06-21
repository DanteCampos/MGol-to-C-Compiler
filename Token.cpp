#pragma once
#include <string>

class Token{
    public:
        std::string lex;
        std::string lex_class;
        std::string type;
        int line;
        int column;
        Token(){};
        Token(std::string lex, std::string lex_class, std::string type, int line, int column){
            this->lex = lex;
            this->lex_class = lex_class;
            this->type = type;
            this->line = line;
            this->column = column;
        }

        
};