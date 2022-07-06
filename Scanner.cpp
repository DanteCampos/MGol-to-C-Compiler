#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Token.cpp"
#include "DFA.cpp"
#include "SymbolsTable.cpp"

// Lexical Analyzer
class Scanner{
    private:
        std::ifstream archive;
        int line;
        int column;
        bool haveLast;
        char last;
        SymbolsTable *table;
        DFA dfa;
        //std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_<>=+-/*\"{}(),;\\";
        

        //Returns true if char is in the language, comparing with the alphabet
        bool isInLanguage(char c){
            return dfa.getSymbol(c) != -1;
        }

    public:
        Scanner(std::string filePath,SymbolsTable *symbolstable){
            archive.open(filePath);
            line = 0;
            column = 0;
            haveLast=false;
            table=symbolstable;
        }

        bool isOpen(){
            if(haveLast)
                return true;
            return archive.is_open();
        }

        std::tuple<Token,int,int> SCANNER(){
            
            char actualChar,firstChar=0,lastChar;
            std::string lexem;
            int initialLine,initialColumn;
            int state=0;
            bool initiatedToken=false;

            // Loop to read characters until get a token or error
            while(true){

                // If there is a char in the buffer(last), takes it, else takes from the input
                if(!haveLast)
                    actualChar = archive.get();
                else{
                    actualChar = last;
                    haveLast=false;
                }
                
                // Try to transition
                int symbol = dfa.getSymbol(actualChar);
                int newState = dfa.transition(state,symbol);
                
                // Bad transition
                if(newState==-1){
                    haveLast=true;
                    last=actualChar;
                    break;
                }

                // Succeed Transition
                state=newState;

                // Updates token position if char is not ignored
                if ((actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and actualChar != ' ') ||
                    (actualChar != '\n' and actualChar != '\r' and actualChar != '\t' and firstChar == '\"')){
                    if(!initiatedToken){
                        initialLine = line;
                        initialColumn = column;
                        initiatedToken=true;
                        firstChar=actualChar;
                    }
                    lexem += actualChar;
                    lastChar = actualChar;
                }

                // Simulating pointer movement
                column++;
                if(actualChar == '\n'){
                    line++;
                    column = 0;
                }
            }// Readed lexem/error

            std::string stateclass=dfa.getStateClass(state);
            if(stateclass=="EOF"){
                haveLast=false;
                lexem=stateclass;
                archive.close();
            }

            if(stateclass == "ERROR"){
                haveLast=false;                
                // if(actualChar==-1)
                // {
                //     lexem="NULL";   
                //     return std::make_tuple(Token(lexem, stateclass, "ERL2"), line, column++);             
                // }else
                // {
                    lexem = lastChar;
                    return std::make_tuple(Token(lexem, stateclass, "ERL1"), line, column++);
                // } 
            }

            // Call again function since Comment should be ignored by lex
            if(stateclass=="Comment")
                return SCANNER();

            // Updating symbol table
            if(stateclass=="Id"){
                if(!table->haveSymbol(lexem))
                        table->putSymbol(Token(lexem, stateclass, "NULL"));
                return std::make_tuple(table->getSymbol(lexem), initialLine, initialColumn);

            }

            if (state == 1 or state == 19 or state == 27) // Real number, put type in the token
                return std::make_tuple(Token(lexem, stateclass, "Real"), initialLine, initialColumn);
            else if (state == 17 or state == 25 or state == 26) // Int number, put type in the token
                return std::make_tuple(Token(lexem, stateclass, "Int"), initialLine, initialColumn);
            else // No type
                return std::make_tuple(Token(lexem, stateclass, "NULL"), initialLine, initialColumn);
        }
};