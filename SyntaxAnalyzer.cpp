#include <string>
#include "Scanner.cpp"
#include "SymbolsTable.cpp"

int main(int argc, char** argv){
    SymbolsTable table;
    Scanner scanner(argv[1],&table);
    Token tk;
    int i = 1;
    char mov;
    while (scanner.isOpen()){
        tk = scanner.SCANNER();
        
        // std::cout << "TOKEN " << i++ << '\n';

        std::cout << "Class: " << tk.lex_class << " , "
        << "Lexem: "<< tk.lex << " , "
        << "Type: "<< tk.type << "\n";
        // << "At line "<< tk.line << ", column " << tk.column << '\n'
        // << '\n';
        if(tk.lex_class=="ERROR")
        {
            if(tk.type=="ERL1")
                    std::cout<<"Lexical Error - Invalid Character";
            if(tk.type=="ERL2")
                    std::cout<<"Lexical Error - Missing Character";
            std::cout<<" at line "<<tk.line<<" and column "<<tk.column<<"\n";
        }
        // std::cin>>mov;
    }
    // std::cout<<"Imprimindo a Tabela de Simbolos\n";
    // table.show();
    return 0;
}