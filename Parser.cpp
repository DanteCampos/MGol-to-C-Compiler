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
    bool valid = true;

    std::map <std::string, std::string> errorMessageMap={
        {"ERS0", "Invalid syntax"},
        {"ERS1", "Missing \"inicio\""},
        {"ERS2", "Missing \"varinicio\""},
        {"ERS3", "Missing \"varfim\""},
        {"ERS4", "Missing \",\""},
        {"ERS5", "Missing id"},
        {"ERS6", "Missing type definition"},
        {"ERS7", "Missing id"},
        {"ERS8", "Missing argument"},
        {"ERS9", "Missing id"},
        {"ERS10", "Missing attribution"},
        {"ERS11", "Missing value"},
        {"ERS12", "Missing operator"},
        {"ERS13", "Missing \"(\""},
        {"ERS14", "Missing \"(\""},
        {"ERS15", "Missing expression"},
        {"ERS16", "Missing \"entao\""},
        {"ERS17", "Missing \"se\""},
        {"ERS18", "Missing \"fimse\""},
        {"ERS19", "Incomplete condition"},
        {"ERS20", "Missing \"(\""},
        {"ERS21", "Missing \")\""},
        {"ERS22", "Missing expression"},
        {"ERS23", "Missing parentheses"},
        {"ERS24", "Missing parentheses"},
        {"ERS25", "Missing \"fimrepita\""},
        {"ERS26", "Missing \"repita\""},
        {"ERS27", "Missing \";\""},
        {"ERS28", "Missing \";\""},
        {"ERS29", "Missing \";\""},
        {"ERS30", "Missing \";\""},
        {"ERS31", "Missing \";\""},
        {"ERS32", "Missing \"fimse\""},
        {"ERS33", "Missing \"fimse\""},
        {"ERS34", "Missing \"fimse\""},
        {"ERS35", "Missing \"fimrepita\""},
        {"ERS36", "Missing \"fimrepita\""},
        {"ERS37", "Missing \"fimrepita\""},
        {"ERS38", "Missing \"fim\""},
        {"ERS39", "Missing \"fim\""},
        {"ERS40", "Missing \"fim\""},
        {"ERS41", "Missing \"fim\""},
        {"ERS42", "Missing \"fim\""}
    };

    // Movement readed from the DFA
    // 'S' = Shift, 'R' = Reduce, "Error", "Accept"
    char mov; 
    int stoppls;
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
            if(tk.lex=="EOF")
                break;
            std::tie(tk,line,column) = scanner.SCANNER();
            // std::cout<<tk.lex<<' '<<line<<' '<<column<<'\n';
        }else if( mv == "R"){ // Reduce
            int qtd = dfa_syntax.sizeSTATE(at); // How many states will be removed
            std::string non_terminal = dfa_syntax.showSTATE(at); // Show the rule used to reduce, get the left side of derivation
            while(qtd--) // Removing states from the stack
                Stack.pop();
            
            // Checking if rule reduced is an error
            if (non_terminal.find("ERS") != std::string::npos){
                // std::cin>>stoppls;
                std::string code = non_terminal; // Known error
                std::cout<<"Syntatic Error " << code << " - " << errorMessageMap[code] <<" at line "<<line<<", column "<<column<<'\n';
                valid = false;
            }

            // std::cout<<"GO "<<Stack.top()<<" "<<dfa_syntax.GOTO(Stack.top(),dfa_syntax.initial(at))<<'\n';
            
            // Push the symbol at the left side of the rule used in the reduction
            Stack.push(dfa_syntax.GOTO(Stack.top(),dfa_syntax.initial(at)));
        }else if(mv == "ERROR"){
            std::string code = "ERS0"; // Unknown error
            std::cout<<"Syntatic Error " << code << " - " << errorMessageMap[code] <<" at line "<<line<<", column "<<column<<'\n';
            break;
        }else{ // Accept
            if(valid){
                dfa_syntax.showSTATE(at);
                std::cout<<"Code accepted.\n";
            }
            break;
        }
    }
    
    // std::cout<<"Printing Symbol Table\n";
    // table.show();
    return 0;
}