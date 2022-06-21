#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Token.cpp"
#include "DFA.cpp"
#include "SymbolsTable.cpp"

class Scanner{
    public:
        Scanner(std::string filePath,SymbolsTable *Syntaxtable){
            archive.open(filePath);
            line = 0;
            column = 0;
            last=-1;
            table=Syntaxtable;
        }

        bool isOpen(){
            return archive.is_open();
        }

        Token SCANNER(){
            DFA dfa;
            char actualChar,fc=0;
            std::string lexem;
            int initialLine,initialColumn;
            int state=0,inittoken=0;
            // Loop to read characters until get a token or error
            while(true){
                if(last==-1){
                    actualChar = archive.get();
                }else
                {
                    actualChar = (char)last;
                    last=-1;
                }

                int symbol = dfa.getSymbol(actualChar);
                int newState = dfa.transition(state,symbol);
                if(newState==-1){
                    last=actualChar;
                    break;
                }
                
                state=newState;
                if ((actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and actualChar != ' ') ||
                    (actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and fc == '\"')){
                    if(!inittoken)
                    {
                        initialLine = line;
                        initialColumn = column;
                        inittoken=1;
                        fc=actualChar;
                    }
                    lexem += actualChar;
                }

                column++;
                if(actualChar == '\n'){
                    line++;
                    column = 0;
                }
            }
            
            if (actualChar == -1){
                archive.close();
            }
            std::string stateclass=dfa.getStateClass(state);
            if(stateclass=="Comment")
                return SCANNER();
            if(stateclass=="Id")
            {
                if(!table->haveSymbol(lexem))
                    table->putSymbol(Token(lexem, stateclass, "NULL", initialLine, initialColumn));
                return table->getSymbol(lexem);

            }
            return Token(lexem, stateclass, "", initialLine, initialColumn);
        }

    private:
        std::ifstream archive;
        int line;
        int column;
        int last;
        SymbolsTable *table;
        std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_<>=+-/*\"{}(),;\\";
        

        //Returns true if char is in the language, comparing with the alphabet
        bool isInLanguage(char c){
            return alphabet.find(c) != std::string::npos;
        }
};