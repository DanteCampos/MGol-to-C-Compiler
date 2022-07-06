#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
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
        std::map <std::string, std::string> errorMessageMap={
            {"ERL1", "Invalid character"},
            {"ERL2", "Missing character"},
            {"ERL3", "Comment not closed"},
            {"ERL4", "Literal not closed"},
            {"ERL5", "Expected number after ."},
            {"ERL6", "Expected power after e or E"},
            {"ERL7", "Expected exponent number after - or +"}
        };

        //Returns true if char is in the language, comparing with the alphabet
        bool isInLanguage(char c){
            return dfa.getSymbol(c) != -1;
        }

        std::string getErrorMessage(std::string errorCode){
            if(errorMessageMap.find(errorCode)==errorMessageMap.end())
                return "Unidentified error"; // Code not found
            return errorMessageMap[errorCode];
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

            std::string stateClass=dfa.getStateClass(state);
            Token returnToken = Token(lexem, stateClass, "NULL");
            int returnLine = initialLine;
            int returnColumn = initialColumn;

            // Treating special tokens

            // If reached the end of file, closes it
            if(stateClass=="EOF"){
                haveLast=false;
                lexem="EOF";
                archive.close();
                returnToken = Token("EOF", "EOF", "NULL");
            }

            // If it is an error, get it's code, prints it's error message and returns error token
            if(stateClass == "ERROR"){
                lexem = lastChar;
                std::string code = dfa.getErrorCode(state);
                
                if(code=="ERL1")
                {
                    lexem = actualChar;
                    haveLast=false;
                    returnColumn = column++;
                }else
                {
                    lexem=lastChar;
                    returnColumn = column;
                }
                
                returnToken = Token(lexem, "ERROR", code);
                returnLine = line;
                std::cout << "Lexical Error " << code << " - " << errorMessageMap[code] << " at line " << returnLine << " and column " << returnColumn << "\n";
            }

            // Call again function since Comment should be ignored by the scanner
            if(stateClass=="Comment")
                return SCANNER();

            // Updating symbol table
            if(stateClass=="Id"){
                if(!table->haveSymbol(lexem))
                        table->putSymbol(Token(lexem, "Id", "NULL"));
                returnToken = table->getSymbol(lexem);
            }

            // If it is a number, get it's type
            if (stateClass=="Num")
                returnToken = Token(lexem, "Num", dfa.getNumType(state));

            if (stateClass == "Lit")
                returnToken = Token(lexem, "Lit", "Literal");

            // Returns token and it's position
            return std::make_tuple(returnToken, returnLine, returnColumn);
        }
};