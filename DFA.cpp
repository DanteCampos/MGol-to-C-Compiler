#pragma once
#include <string>
#include <map>
#include<vector>
/*
Letters without E and e
ALPHABET_L_E_e = "abcdfghijklmnopqrstuvwxyzABCDFGHIJKLMNOPQRSTUVWXYZ";
ALPHABET_E_e = "Ee";
ALPHABET_D = "0123456789";
ALPHABET_S = " \t\n\r";
ALPHABET_SYMBOLS = ALPHABET_L_E_e + ALPHABET_D + "_<>=+-/*\"{}(),;\\";
*/


/* Columns:
00: \s (blank space)
01: E | e (letters e and E)
02: _
03: < 
04: >
05: =
06: +
07: -
08: /
09: *
10: "
11: {
12: }
13: (   
14: )
15: ,
16: ;
17: D (digits)
18: L-(E|e) (letters, except E and e)
19: EOF (end of file)
20: . (dot)
21: \ (escape)
*/

class DFA {
    private:
        std::vector<std::vector<char>> transitionTable;
        std::map<int,std::string> StateClassMap;
        std::map<char,int> getSymbolMap;
        std::map<int,std::string> errorMap;
        std::map<int,std::string> numTypeMap;

    public:
        DFA(){
            //initiating tables
            transitionTable = {
                /*0*/{0,15,-1,13,12,5,7,7,7,7,16,14,-1,8,9,11,10,17,15,4,-1,-1,-1},
                /*1*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1},
                /*2*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*3*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*4*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*5*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*6*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*7*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*8*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*9*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*10*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*11*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*12*/{-1,-1,-1,-1,-1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*13*/{-1,-1,-1,-1,5,5,-1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                /*14*/{14,14,14,14,14,14,14,14,14,14,14,14,3,14,14,14,14,14,14,-1,14,14,14},
                /*15*/{-1,15,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,15,15,-1,-1,-1,-1},
                /*16*/{16,16,16,16,16,16,16,16,16,16,2,16,16,16,16,16,16,16,16,-1,16,16,16},
                /*17*/{-1,22,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,18,-1},
                /*18*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,19,-1,-1,-1,-1},
                /*19*/{-1,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,19,-1,-1,-1,-1},
                /*20*/{-1,-1,-1,-1,-1,-1,21,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1},
                /*21*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1},
                /*22*/{-1,-1,-1,-1,-1,-1,23,24,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,-1},
                /*23*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,26,-1,-1,-1,-1},
                /*24*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,27,-1,-1,-1,-1},
                /*25*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,-1},
                /*26*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,26,-1,-1,-1,-1},
                /*27*/{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,27,-1,-1,-1,-1}
            };

            StateClassMap = {
                {1,"num"},{17,"num"},{19,"num"},{25,"num"},{26,"num"},{27,"num"},
                {2,"lit"},
                {3,"Comment"},
                {4,"EOF"},
                {5,"opr"},{12,"opr"},{13,"opr"},
                {6,"rcb"},
                {7,"opm"},
                {8,"ab_p"},
                {9,"fc_p"},
                {10,"pt_v"},
                {11,"vir"},
                {15,"id"}   
            };

            getSymbolMap = {
                {' ',0},{'\r',0},{'\t',0},{'\n',0},
                {'E',1},{'e',1},
                {'_',2},
                {'<',3},
                {'>',4},
                {'=',5},
                {'+',6},
                {'-',7},
                {'/',8},
                {'*',9},
                {'\"',10},
                {'{',11},
                {'}',12},
                {'(',13},
                {')',14},
                {',',15},
                {';',16},
                {-1,19},
                {'.',20},
                {'\\',21},
                {'\'',22},{':',22}
            };

            for(char c = 'a';c<='z';c++)
                if(c!='e')
                    getSymbolMap[c]=18;

            for(char c = 'A';c<='Z';c++)
                if(c!='E')
                    getSymbolMap[c]=18;

            for(char c = '0';c<='9';c++)
                getSymbolMap[c]=17;
            
            errorMap = {
                {0, "ERL1"}, // Invalid character
                {14, "ERL3"}, // Comment not closed
                {16, "ERL4"}, // Literal not closed
                {18, "ERL5"}, // Expected number after .
                {20, "ERL6"}, // Expected power after e or E
                {21, "ERL7"}, // Expected exponent number after - or +
                {22, "ERL6"}, // Expected power after e or E
                {23, "ERL7"}, // Expected exponent number after - or +
                {24, "ERL7"} // Expected exponent number after - or +
            };

            numTypeMap = {
                {1, "real"},
                {19, "real"},
                {27, "real"},
                {17, "inteiro"},
                {25, "inteiro"},
                {26, "inteiro"}
            };
        };

        // Transition the state of the DFA based on the given symbol
        int transition(int state, int symbol){
            if (symbol != -1 and state != -1)
                state = transitionTable[state][symbol];
            else
                state = -1;   
            
            return state;
        }

        // Returns column equivalent to the symbol in the DFA
        int getSymbol(char c){
            if(getSymbolMap.find(c)==getSymbolMap.end())
                return -1;
            return getSymbolMap[c];
        }

        // Returns the class of the last valid state
        // If the last valid state is not a final state, returns "ERROR"
        std::string getStateClass(int lastValidState){
            if(StateClassMap.find(lastValidState)==StateClassMap.end())
                return "ERROR";
            return StateClassMap[lastValidState];
        }

        // Returns the code of the error ocurred by stopping at the last valid state
        std::string getErrorCode(int lastValidState){
            if(errorMap.find(lastValidState)==errorMap.end())
                return "ERL2"; // Missing character
            return errorMap[lastValidState];
        }

        // Returns the type of the number got at the last valid state
        std::string getNumType(int lastValidState){
            if(numTypeMap.find(lastValidState)==numTypeMap.end())
                return "NULL"; // Type not found
            return numTypeMap[lastValidState];
        }
};

