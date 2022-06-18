#include <string>

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
    public:
        //Initiates the DFA
        DFA(){
            state = 0;
            lastValidState = 0;
        }

        // Transition the state of the DFA based on the given symbol
        int transition(int symbol){
            if (symbol != -1 and state != -1)
                state = transitionTable[state][symbol];
            else
                state = -1;
            if (state != -1)
                lastValidState = state;    
            
            return state;
        }

        // Returns column equivalent to the symbol in the DFA
        int getSymbol(char c){
            if (c == ' ' or c == '\r' or c == '\t' or c == '\n')
                return 0;
            if (c == 'E' or c == 'e')
                return 1;
            if (c == '_')
                return 2;
            if (c == '<')
                return 3;
            if (c == '>')
                return 4;
            if (c == '=')
                return 5;
            if (c == '+')
                return 6;
            if (c == '-')
                return 7;
            if (c == '/')
                return 8;
            if (c == '*')
                return 9;
            if (c == '\"')
                return 10;
            if (c == '{')
                return 11;
            if (c == '}')
                return 12;
            if (c == '(')
                return 13;
            if (c == ')')
                return 14;
            if (c == ',')
                return 15;
            if (c == ';')
                return 16;
            if (c >= '0' and c <= '9')
                return 17;
            if ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z'))
                return 18;
            if (c == -1)
                return 19;
            if (c == '.')
                return 20;
            if (c == '\\')
                return 21;

            return -1;
        }

        // Returns the class of the last valid state
        // If the last valid state is not a final state, returns "ERROR"
        std::string getStateClass(){
            if (lastValidState == 1 or lastValidState == 17 or lastValidState == 19)
                return "Num";
            if (lastValidState == 2)
                return "Lit";
            if (lastValidState == 3)
                return "Comment";
            if (lastValidState == 4)
                return "EOF";
            if (lastValidState == 5 or lastValidState == 12 or lastValidState == 13)
                return "OPR";
            if (lastValidState == 6)
                return "RCB";
            if (lastValidState == 7)
                return "OPM";
            if (lastValidState == 8)
                return "AB_P";
            if (lastValidState == 9)
                return "FC_P";
            if (lastValidState == 10)
                return "PT_V";
            if (lastValidState == 11)
                return "VIR";
            if (lastValidState == 15)
                return "Id";
            return "ERROR";
        }

    private:
        int state;
        int lastValidState;
        char transitionTable[22][22] ={
            {0,15,-1,13,12,5,7,7,7,7,16,14,-1,8,9,11,10,17,15,4,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,5,5,-1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {14,14,14,14,14,14,14,14,14,14,14,14,3,14,14,14,14,14,14,-1,14,14},
            {-1,15,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,15,15,-1,-1,-1},
            {16,16,16,16,16,16,16,16,16,16,2,16,16,16,16,16,16,16,16,-1,16,16},
            {-1,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,-1,-1,18,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,19,-1,-1,-1,-1},
            {-1,20,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,19,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,21,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1}
        };
};

