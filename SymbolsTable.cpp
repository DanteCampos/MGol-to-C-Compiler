#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Token.cpp"

class SymbolsTable{
    std::unordered_map<std::string, std::string, std::string> table;

    public:
        SymbolsTable(){
            std::vector<Token> reservedWords;
            reservedWords.push_back(Token("inicio", "inicio", "", 0, 0));
            reservedWords.push_back(Token("varinicio", "varinicio", "", 0, 0));
            reservedWords.push_back(Token("varfim", "varfim", "", 0, 0));
            reservedWords.push_back(Token("escreva", "escreva", "", 0, 0));
            reservedWords.push_back(Token("leia", "leia", "", 0, 0));
            reservedWords.push_back(Token("se", "se", "", 0, 0));
            reservedWords.push_back(Token("entao", "entao", "", 0, 0));
            reservedWords.push_back(Token("fimse", "fimse", "", 0, 0));
            reservedWords.push_back(Token("Repita", "Repita", "", 0, 0));
            reservedWords.push_back(Token("fimRepita", "fimRepita", "", 0, 0));
            reservedWords.push_back(Token("fim", "fim", "", 0, 0));
            reservedWords.push_back(Token("inteiro", "inteiro", "", 0, 0));
            reservedWords.push_back(Token("literal", "literal", "", 0, 0));
            reservedWords.push_back(Token("real", "real", "", 0, 0));
        }

        bool putSymbol(Token tk){

        }

        bool updateSymbol(Token tk){

        }

        Token getSymbol(std::string lex){

        }
};