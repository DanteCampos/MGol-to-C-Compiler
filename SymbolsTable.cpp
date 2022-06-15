#pragma once
#include <unordered_map>
#include <string>
#include "Token.cpp"

class SymbolsTable{
    std::unordered_map<std::string, std::string, std::string> table;

    public:
        bool putSymbol(Token tk){

        }

        bool updateSymbol(Token tk){

        }

        Token getSymbol(std::string lex){

        }
};