#include <string>
#include <stack>
#include "Scanner.cpp"
#include "Generator.cpp"
#include "SymbolsTable.cpp"
#include "Token.cpp"
#include "DFAsintax.cpp"

// ./exec <source.mgol> <output filepath>
int main(int argc, char** argv){
    SymbolsTable table;
    Scanner scanner(argv[1],&table);
    Generator gen(argv[2],&table);
    Token tk;
    DFAsintax dfa_syntax;
    int i = 1, line, column, state = 0, type, nxstate;
    bool valid = true;

    // Movement readed from the DFA
    // 'S' = Shift, 'R' = Reduce, "Error", "Accept"
    char mov; 
    int stoppls;
    std::stack<int> Stack; // Stack of states

    Stack.push(1); // Initial state = 1
    std::tie(tk, line, column) = scanner.SCANNER(valid); // Get first token
    while (true){ // Loop: Read next token and move to next state
        std::string mv;
        int at;
        
        //Getting the movement from the Syntax DFA
        std::tie(mv,at) = dfa_syntax.ACTION(Stack.top(),tk.lex_class);

        // std::cout<<Stack.top()<<" "<<tk.lex_class<<'\n';
        // std::cout<<mv<<" "<<at<<'\n';
       
        if( mv == "S"){ // Shift
            Stack.push(at); // Push state
            gen.Stack.push(tk);
            if(tk.lex=="EOF")
                break;
            std::tie(tk,line,column) = scanner.SCANNER(valid);
            
            
            // std::cout<<tk.lex<<' '<<line<<' '<<column<<'\n';

        }else if( mv == "R"){ // Reduce
            int qtd = dfa_syntax.sizeSTATE(at); // How many states will be removed
            // dfa_syntax.showSTATE(at); // Show the rule used to reduce, get the left side of derivation
            std::string non_terminal = dfa_syntax.initial(at); // Show the rule used to reduce, get the left side of derivation
            
            gen.reduction(at,line,column,valid);

            while(qtd--) // Removing states from the stack
                Stack.pop();
            
            // Checking if rule reduced is an error
            if (non_terminal.find("ERS") != std::string::npos){
                // std::cin>>stoppls;
                std::string code = non_terminal; // Known error
                std::cout<<"Syntatic Error " << code << " - " << dfa_syntax.errorMessageMap[code] <<" at line "<<line<<", column "<<column<<'\n';
                valid = false;
            }
            // std::cout<<"GO "<<Stack.top()<<" "<<dfa_syntax.GOTO(Stack.top(),dfa_syntax.initial(at))<<'\n';
            
            // Push the symbol at the left side of the rule used in the reduction
            Stack.push(dfa_syntax.GOTO(Stack.top(),dfa_syntax.initial(at)));
        }else if(mv == "ERROR"){
            valid = false;
            std::string code = "ERS0"; // Unknown error
            std::cout<<"Syntatic Error " << code << " - " << dfa_syntax.errorMessageMap[code] <<" at line "<<line<<", column "<<column<<'\n';
            break;
        }else{ // Accept
            if(valid){
                Stack.pop();
                gen.reduction(at,line,column,valid);
                // dfa_syntax.showSTATE(at);
                std::cout<<"Code accepted.\n";
            }
            break;
        }
    }

    if(!valid)
        return 0;
    scanner.back();
    gen.start_code();
    std::tie(tk, line, column) = scanner.SCANNER(valid); // Get first token
    // std::cout<<tk.lex<<' '<<tk.lex_class<<' '<<tk.type<<"\n";
    
    while (true){ // Loop: Read next token and move to next state
        std::string mv;
        int at;
        
        //Getting the movement from the Syntax DFA
        std::tie(mv,at) = dfa_syntax.ACTION(Stack.top(),tk.lex_class);

       
        if( mv == "S"){ // Shift
            Stack.push(at); // Push state
            gen.Stack.push(tk);
            if(tk.lex=="EOF")
                break;
            std::tie(tk,line,column) = scanner.SCANNER(valid);
            // std::cout<<tk.lex<<' '<<line<<' '<<column<<'\n';
        }else if( mv == "R"){ // Reduce
            int qtd = dfa_syntax.sizeSTATE(at); // How many states will be removed
            
            dfa_syntax.showSTATE(at); // Show the rule used to reduce, get the left side of derivation
            
            gen.reduction(at,line,column,valid);
            
            std::string non_terminal = dfa_syntax.initial(at); // Show the rule used to reduce, get the left side of derivation
            
            while(qtd--) // Removing states from the stack
                Stack.pop();

            // Push the symbol at the left side of the rule used in the reduction
            Stack.push(dfa_syntax.GOTO(Stack.top(),dfa_syntax.initial(at)));
        }else{ // Accept
            dfa_syntax.showSTATE(at);
            gen.reduction(at,line,column,valid);
            gen.finish_code();
            break;
        }
    }
    // std::cout<<"Printing Symbol Table\n";
    // table.show();
    return 0;
}