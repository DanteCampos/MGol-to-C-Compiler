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
                {"inicio",Token("inicio", "inicio", "inicio")},
                {"varinicio",Token("varinicio", "varinicio", "varinicio")},
                {"varfim",Token("varfim", "varfim", "varfim")},
                {"escreva",Token("escreva", "escreva", "escreva")},
                {"leia",Token("leia", "leia", "leia")},
                {"se",Token("se", "se", "se")},
                {"entao",Token("entao", "entao", "entao")},
                {"fimse",Token("fimse", "fimse", "fimse")},
                {"Repita",Token("Repita", "Repita", "Repita")},
                {"fimRepita",Token("fimRepita", "fimRepita", "fimRepita")},
                {"fim",Token("fim", "fim", "fim")},
                {"inteiro",Token("inteiro", "inteiro", "inteiro")},
                {"literal",Token("literal", "literal", "literal")},
                {"real",Token("real", "real", "real")}
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