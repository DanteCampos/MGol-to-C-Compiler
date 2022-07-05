#include <string>
#include "Scanner.cpp"
#include "SymbolsTable.cpp"

int main(int argc, char** argv){
    SymbolsTable table;
    Scanner scanner(argv[1],&table);
    Token tk;
    int i = 1,line,column;
    char mov;
    while (scanner.isOpen()){
        std::tie(tk,line,column) = scanner.SCANNER();
        
        // std::cout << "TOKEN " << i++ << '\n';

        std::cout << "Class: " << tk.lex_class << " , "
        << "Lexem: "<< tk.lex << " , "
        << "Type: "<< tk.type << "\n";
        // std::cout<< "At line "<< line << ", column " << column << '\n'
        // << '\n';
        if(tk.lex_class=="ERROR")
        {
            if(tk.type=="ERL1")
                    std::cout<<"Lexical Error - Invalid Character";
            if(tk.type=="ERL2")
                    std::cout<<"Lexical Error - Missing Character";
            std::cout<<" at line "<<line<<" and column "<<column<<"\n";
        }
        // std::cin>>mov;
    }
    // std::cout<<"Printing Symbol Table\n";
    // table.show();
    return 0;
}