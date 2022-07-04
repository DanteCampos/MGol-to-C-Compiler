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
            havelast=false;
            table=Syntaxtable;
        }

        bool isOpen(){
            if(havelast)
                return true;
            return archive.is_open();
        }

        Token SCANNER(){
            DFA dfa;
            char actualChar,firstChar=0;
            std::string lexem;
            int initialLine,initialColumn;
            int state=0,inittoken=0;
            // Loop to read characters until get a token or error
            while(true){
                if(!havelast){
                    actualChar = archive.get();
                }else
                {
                    actualChar = last;
                    havelast=false;
                }

                int symbol = dfa.getSymbol(actualChar);
                int newState = dfa.transition(state,symbol);
                if(newState==-1){
                    havelast=true;
                    last=actualChar;
                    break;
                }
                
                state=newState;
                if ((actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and actualChar != ' ') ||
                    (actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and firstChar == '\"')){
                    if(!inittoken)
                    {
                        initialLine = line;
                        initialColumn = column;
                        inittoken=1;
                        firstChar=actualChar;
                    }
                    lexem += actualChar;
                }

                column++;
                if(actualChar == '\n'){
                    line++;
                    column = 0;
                }
            }
            std::string stateclass=dfa.getStateClass(state);
            if(stateclass=="EOF"){
                havelast=false;
                lexem=stateclass;
                archive.close();
            }
            if(stateclass == "ERROR"){
                havelast=false;                
                if(actualChar==-1)
                {
                    lexem="NULL";   
                    return Token(lexem, stateclass, "ERL2", line, column++);             
                }else
                {
                    lexem = actualChar;
                    return Token(lexem, stateclass, "ERL1", line, column++);
                } 
            }
            if(stateclass=="Comment")
                return SCANNER();
            if(stateclass=="Id")
            {
                if(!table->haveSymbol(lexem))
                        table->putSymbol(Token(lexem, stateclass, "NULL", initialLine, initialColumn));
                return table->getSymbol(lexem);

            }

            if (state == 1 or state == 19 or state == 27) // Real number, put type in the token
                return Token(lexem, stateclass, "Real", initialLine, initialColumn);
            else if (state == 17 or state == 25 or state == 26) // Int number, put type in the token
                return Token(lexem, stateclass, "Int", initialLine, initialColumn);
            else // No type
                return Token(lexem, stateclass, "NULL", initialLine, initialColumn);
        }

    private:
        std::ifstream archive;
        int line;
        int column;
        bool havelast;
        char last;
        SymbolsTable *table;
        std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_<>=+-/*\"{}(),;\\";
        

        //Returns true if char is in the language, comparing with the alphabet
        bool isInLanguage(char c){
            return alphabet.find(c) != std::string::npos;
        }
};