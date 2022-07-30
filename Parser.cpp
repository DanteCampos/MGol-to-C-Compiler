#pragma once
#include <string>
#include <stack>
#include "Scanner.cpp"
#include "SymbolsTable.cpp"
#include "Token.cpp"
#include "DFAsintax.cpp"

int main(int argc, char** argv){
    SymbolsTable table;
    Scanner scanner(argv[1],&table);
    Token tk;
    DFAsintax dfa_syntax;
    int i = 1, line, column, state = 0, type, nxstate;
    
    std::map <std::string, std::string> errorMessageMap={
        {"ERS1", "Invalid syntax"}
    };

    // Movement readed from the DFA
    // 'S' = Shift, 'R' = Reduce, "Error", "Accept"
    char mov; 

    std::stack<int> Stack; // Stack of states and tokens
    Stack.push(1); // Initial state = 1
    std::tie(tk, line, column) = scanner.SCANNER(); // Get first token
    
    while (true){ // Loop: Read next token and move to next state
        std::string mv;
        int at;
        
        //Getting the movement from the Syntax DFA
        std::tie(mv,at) = dfa_syntax.ACTION(Stack.top(),tk.lex_class);
        // std::cout<<Stack.top()<<" "<<tk.lex_class<<'\n';
        // std::cout<<mv<<" "<<at<<'\n';
       
        if( mv == "S"){ // Shift
            Stack.push(at); // Push state
            std::tie(tk,line,column) = scanner.SCANNER();

        }else if( mv == "R"){ // Reduce
            int qtd = dfa_syntax.sizeSTATE(at); // How many states will be removed
            dfa_syntax.showSTATE(at); // Show the rule used to reduce
            while(qtd--) // Removing states from the stack
                Stack.pop();

            // std::cout<<Stack.top()<<" "<<mydfa.GOTO(Stack.top(),mydfa.initial(at))<<'\n';
            
            // Push the symbol at the left side of the rule used in the reduction
            Stack.push(dfa_syntax.GOTO(Stack.top(),dfa_syntax.initial(at)));
        }else if(mv == "ERROR"){
            std::string code = dfa_syntax.getErrorCode(at);
            std::cout<<"Syntatic Error " << code << " - " << errorMessageMap[code] <<" at line "<<line<<", column "<<column<<'\n';
            break;
        }else{ // Accept
            dfa_syntax.showSTATE(at);
            std::cout<<"Code accepted.\n";
            break;
        }
    }
    
    // std::cout<<"Printing Symbol Table\n";
    // table.show();
    return 0;
}