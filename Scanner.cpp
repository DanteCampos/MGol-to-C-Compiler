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

        std::tuple<Token,int,int> SCANNER(){
            DFA dfa;
            char actualChar,firstChar=0,lastChar;
            std::string lexem;
            int initialLine,initialColumn;
            int state=0,inittoken=0;
            // Loop to read characters until get a token or error
            while(true){

                //if there is a char in the buffer(last), takes it, else takes from the input
                if(!havelast){
                    actualChar = archive.get();
                }else
                {
                    actualChar = last;
                    havelast=false;
                }
                //try to do a transition
                int symbol = dfa.getSymbol(actualChar);
                int newState = dfa.transition(state,symbol);
                
                //Bad transition
                if(newState==-1){
                    havelast=true;
                    last=actualChar;
                    break;
                }
                //Succeed Transition
                state=newState;

                //update position an lexem, case the char is not an ignore
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
                    lastChar = actualChar;
                }

                //Simulating pointer movement
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
            //call again function since Comment should be ignored by lex
            if(stateclass=="Comment")
                return SCANNER();

            //updating symbol table
            if(stateclass=="Id")
            {
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