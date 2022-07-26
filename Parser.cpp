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
    DFAsintax mydfa;
    int i = 1,line,column,state=0,type,nxstate;
    char mov;
    std::stack<int> St;
    St.push(1);
    std::tie(tk,line,column) = scanner.SCANNER();
    while (1){
        std::string mv;
        int at;
        std::tie(mv,at) = mydfa.ACTION(St.top(),tk.lex_class);
        // std::cout<<St.top()<<" "<<tk.lex_class<<'\n';
        // std::cout<<mv<<" "<<at<<'\n';
        if( mv == "S")
        {
            St.push(at);
            std::tie(tk,line,column) = scanner.SCANNER();
        } else if( mv == "R")
        {
            int qtd = mydfa.sizeSTATE(at);
            mydfa.showSTATE(at);
            while(qtd--){
                // std::cout<<"out "<<St.top()<<"\n";
                St.pop();
            }
            // std::cout<<St.top()<<" "<<mydfa.GOTO(St.top(),mydfa.initial(at))<<'\n';
            St.push(mydfa.GOTO(St.top(),mydfa.initial(at)));
        }else if(mv == "ERROR")
        {
            std::cout<<"ERRO "<<line<<" "<<column<<'\n';
            break;
        }else{
            mydfa.showSTATE(at);
            std::cout<<"ACEITOU RECEBA O MELHOR DE TODOS\n";
            break;
        }
    }
    // std::cout<<"Printing Symbol Table\n";
    // table.show();
    return 0;
}