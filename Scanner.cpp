#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Token.cpp"

class Scanner{
    public:
        Scanner(std::string filePath){
            std::ifstream sourceCode(filePath);
        }
        
        Token nextToken(){
            Token tk;
            int state = 0;


            return tk;
        }

    private:
        int line;
        int column;

        // Returns true if tk wasn't on table
        // Returns false if tk was on table
        bool putOnTable(Token tk){

        }
};