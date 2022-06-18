#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Token.cpp"
#include "DFA.cpp"

class Scanner{
    public:
        Scanner(std::string filePath){
            archive.open(filePath);
            line = 0;
            column = 0;
        }

        bool isOpen(){
            return archive.is_open();
        }

        Token SCANNER(){
            Token tk;
            DFA dfa;
            char actualChar;
            std::string lexem;
            
            tk.column = column;
            tk.line = line;

            // Loop to read characters until get a token or error
            while(true){
                actualChar = archive.get();

                int symbol = dfa.getSymbol(actualChar);
                int newState = dfa.transition(symbol);
                
                column++;
                if(actualChar == '\n'){
                    line++;
                    column = 0;
                }

                if(newState == -1)
                    break;
               
                if (actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and actualChar != ' ')
                    lexem += actualChar;
            }

            tk.lex = lexem;
            tk.lex_class = dfa.getStateClass();
            tk.type = "";

            if (actualChar == -1)
                archive.close();

            return tk;
        }

    private:
        std::ifstream archive;
        int line;
        int column;
        std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_<>=+-/*\"{}(),;\\";
        
        // Returns true if tk wasn't on table
        // Returns false if tk was on table
        bool putOnTable(Token tk){
            return true;
        }

        //Returns true if char is in the language, comparing with the alphabet
        bool isInLanguage(char c){
            return alphabet.find(c) != std::string::npos;
        }
};