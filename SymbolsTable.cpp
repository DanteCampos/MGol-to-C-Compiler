#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Token.cpp"
#include <iostream>
class SymbolsTable{
    private:
        std::unordered_map<std::string, Token> table;

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
            for(auto c:reservedWords)
                table[c.lex]=c;
        }
        ~SymbolsTable(){}
        bool haveSymbol(std::string lex)
        {
            return table.find(lex)!=table.end();
        }
        bool putSymbol(Token tk){
            if(table.find(tk.lex)==table.end())
            {
                table[tk.lex]=tk;
                return 1;
            }else
            {
                std::cout<<"ERRO NAO TEM TOKEN UPT\n";
                return 0;
            }
        }

        bool updateSymbol(Token tk){
            if(table.find(tk.lex)!=table.end())
            {
                table[tk.lex]=tk;
                return 1;
            }else
            {
                std::cout<<"ERRO NAO TEM TOKEN UPT\n";
                return 0;
            }
        }

        void show()
        {
            for(auto c:table)
                    std::cout<<c.first<<"\n";
        }

        Token getSymbol(std::string lex){
            if(table.find(lex)==table.end())
            {
                std::cout<<"ERRO NAO TEM TOKEN GETS "<<lex<<"\n";
                show();
                exit(1);
            }
            return table[lex];
            
        }
};