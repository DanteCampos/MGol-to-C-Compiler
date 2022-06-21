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
        std::cout << "TOKEN " << i << '\n';
        i++;
        std::cout << tk.lex << "\n"
        << "Class: " << tk.lex_class << "\n"
        //<< "Type: "<< tk.type << "\n"
        << "At line "<< tk.line << ", column " << tk.column << '\n'
        << '\n';
        // std::cin>>mov;
    }
    table.show();
    return 0;
}