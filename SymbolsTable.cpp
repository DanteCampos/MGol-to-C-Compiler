#pragma once
#include <map>
#include <string>
#include <vector>
#include "Token.cpp"
#include <iostream>
class SymbolsTable{
    private:
        std::map<std::string, Token> table;

    public:
        SymbolsTable(){
            table = {
                {"inicio",Token("inicio", "inicio", "inicio", 0, 0)},
                {"varinicio",Token("varinicio", "varinicio", "varinicio", 0, 0)},
                {"varfim",Token("varfim", "varfim", "varfim", 0, 0)},
                {"escreva",Token("escreva", "escreva", "escreva", 0, 0)},
                {"leia",Token("leia", "leia", "leia", 0, 0)},
                {"se",Token("se", "se", "se", 0, 0)},
                {"entao",Token("entao", "entao", "entao", 0, 0)},
                {"fimse",Token("fimse", "fimse", "fimse", 0, 0)},
                {"Repita",Token("Repita", "Repita", "Repita", 0, 0)},
                {"fimRepita",Token("fimRepita", "fimRepita", "fimRepita", 0, 0)},
                {"fim",Token("fim", "fim", "fim", 0, 0)},
                {"inteiro",Token("inteiro", "inteiro", "inteiro", 0, 0)},
                {"literal",Token("literal", "literal", "literal", 0, 0)},
                {"real",Token("real", "real", "real", 0, 0)}
            };
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
                std::cout<<"ERROR ALREADY PUT TOKEN\n";
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
                std::cout<<"ERROR NO TOKEN UPT\n";
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
                std::cout<<"ERROR NO TOKEN GETS "<<lex<<"\n";
                show();
                exit(1);
            }
            return table[lex];
            
        }
};