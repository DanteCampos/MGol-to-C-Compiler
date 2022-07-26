#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "Token.cpp"
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
                {"repita",Token("repita", "repita", "repita")},
                {"fimrepita",Token("fimrepita", "fimrepita", "fimrepita")},
                {"fim",Token("fim", "fim", "fim")},
                {"inteiro",Token("inteiro", "inteiro", "inteiro")},
                {"literal",Token("literal", "literal", "literal")},
                {"real",Token("real", "real", "real")}
            };
        }
        
        ~SymbolsTable(){}

        bool haveSymbol(std::string lex){
            return table.find(lex)!=table.end();
        }

        bool putSymbol(Token tk){
            if(table.find(tk.lex)==table.end()){
                table[tk.lex]=tk;
                return 1;
            }else{
                std::cout<<"ERROR: TOKEN ALREADY IN THE TABLE\n";
                return 0;
            }
        }

        bool updateSymbol(Token tk){
            if(table.find(tk.lex)!=table.end()){
                table[tk.lex]=tk;
                return 1;
            }else{
                std::cout<<"ERROR: THERE IS NO TOKEN TO UPDATE\n";
                return 0;
            }
        }

        Token getSymbol(std::string lex){
            if(table.find(lex)==table.end()){
                std::cout<<"ERROR: TOKEN "<<lex<<" IS NOT IN THE TABLE\n";
                show();
                exit(1);
            }
            return table[lex];
        }

        void show(){
            for(auto c:table)
                    std::cout<<c.first<<"\n";
        }
};