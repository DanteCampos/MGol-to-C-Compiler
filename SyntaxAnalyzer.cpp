#include <string>
#include "Scanner.cpp"

int main(int argc, char** argv){
    Scanner scanner(argv[1]);
    Token tk;
    int i = 1;
    while (scanner.isOpen()){
        tk = scanner.SCANNER();
        std::cout << "TOKEN " << i << '\n';
        i++;
        std::cout << tk.lex << "\n"
        << "Class: " << tk.lex_class << "\n"
        //<< "Type: "<< tk.type << "\n"
        << "At line "<< tk.line << ", column " << tk.column << '\n'
        << '\n';
    }
    
    return 0;
}