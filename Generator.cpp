#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include "Token.cpp"
#include "DFAsintax.cpp"
#include "SymbolsTable.cpp"

class Generator
{
private:
    SymbolsTable *table;
    DFAsintax dfa_syntax;
    std::ofstream output;
    std::string global_type,global_exp_r;
    int dtemps,itemps;
    bool generating;
public:

    std::stack<Token> Stack;
    Generator(std::string filePath,SymbolsTable *symbolstable){
        output.open(filePath);
        itemps = dtemps = 0;
        generating = false;
        table=symbolstable;
    }
    void start_code(){
        generating = true;
        //includes and init
        std::ifstream aux("util/baseinit.c");
        std::string s;
        while(getline(aux,s))
            output<<s<<"\n";
        if(itemps!=0)
            output<<"int itemp["<<itemps<<"];\n";
        
        if(dtemps!=0)
            output<<"double dtemp["<<dtemps<<"];\n";

        output<<"//codigo gerado\n";
        itemps = dtemps = 0;
    }
    void finish_code(){
        std::ifstream aux("util/baseend.c");
        std::string s;
        while(getline(aux,s))
            output<<s<<"\n";
    }
    void reduction(int state,int line,int column,bool &valid){
        // if(!valid)
        //     return;
        // std::cout<<state<<' ';
        // dfa_syntax.showSTATE(state);
        Token tk,tk1,tk2;
        
        if(state == 7 || state == 8){
            //L1 → id vir L2
            //L → id
            if(state == 7){ //L1 → id vir L2
                Stack.pop();
                Stack.pop();
            }
            tk = Stack.top();
            tk1 = table->getSymbol(tk.lex);
            if(tk1.type!="NULL")
            {
                valid = false;
                std::cout<<"Semantic Error " << tk.lex<<" has already been declared at line "<<line<<", column "<<column<<'\n';
            }
            tk.type = global_type;
            if(generating){
                if(tk.type == "real")
                    output<<"double "<<tk.lex;
                else if (tk.type == "inteiro")
                    output<<"int "<<tk.lex;
                else if (tk.type == "literal")
                    output<<"literal "<<tk.lex;
                output<<";\n";
            }
            // std::cout<<tk.lex<<" "<<tk.lex_class<<' '<<tk.type<<"\n";
            
            table->updateSymbol(tk);
            tk.lex = tk.lex_class = "L";
            Stack.pop();
            Stack.push(tk);
        }else if (state>= 9 && state <=11){
            //TIPO → real | inteiro | literal
            tk = Stack.top();
            global_type = tk.type;
            tk.lex = tk.lex_class = "TIPO";
            Stack.pop();
            Stack.push(tk);
        }else if (state==13){
            //ES → leia id pt_v
            Stack.pop();
            tk = Stack.top();
            Stack.pop();
            Stack.pop();
            if(tk.type=="NULL"){
                valid = false;
                std::cout<<"Semantic Error " << tk.lex<<" was not decleared at line "<<line<<", column "<<column<<'\n';
            }
            if(generating){
                output<<"scanf(";
                if(tk.type == "real")
                    output<<"\"\%lf\",&"<<tk.lex;
                else if (tk.type == "inteiro")
                    output<<"\"\%d\",&"<<tk.lex;
                else if (tk.type == "literal")
                    output<<"\"\%s\","<<tk.lex;
                output<<");\n";
            }
            tk.lex = tk.lex_class = tk.type = "ES";
            Stack.push(tk);
        }else if (state==14){
            //ES -> escreva ARG pt_v
            Stack.pop();
            tk = Stack.top();
            Stack.pop();
            Stack.pop();
            if(generating){
                output<<"printf(";
                if(tk.lex_class != "id")
                    output<<tk.lex;
                else if(tk.type == "real")
                    output<<"\"\%lf\","<<tk.lex;
                else if (tk.type == "inteiro")
                    output<<"\"\%d\","<<tk.lex;
                else if (tk.type == "literal")
                    output<<"\"\%s\","<<tk.lex;
                output<<");\n";
            }
            tk.lex = tk.lex_class = tk.type = "ES";
            Stack.push(tk);
        }else if (state == 15 || state == 16){
            //ARG -> literal | num
            tk = Stack.top();
            Stack.pop();
            Stack.push(tk);
        }else if (state == 17){
            //ARG -> id
            tk = Stack.top();
            if(tk.type=="NULL"){
                valid = false;
                std::cout<<"Semantic Error " << tk.lex<<" was not decleared at line "<<line<<", column "<<column<<'\n';
            }
            Stack.pop();
            Stack.push(tk);
        }else if (state == 19){
            //CMD → id rcb LD pt_v
            Stack.pop();
            tk2 = Stack.top();
            Stack.pop();
            tk1 = Stack.top();
            Stack.pop();
            tk = Stack.top();
            if(tk.type=="NULL"){
                valid = false;
                std::cout<<"Semantic Error " << tk.lex<<" was not decleared at line "<<line<<", column "<<column<<'\n';
            }else if(tk.type!=tk2.type){
                valid = false;
                std::cout<<"Semantic Error atribution types doesnt match at line "<<line<<", column "<<column<<'\n';
            }
            if(generating){
                output<<tk.lex<<" = "<<tk2.lex<<";\n";
            }
            Stack.pop();
            tk.lex = tk.lex_class = tk.type = "CMD";
            Stack.push(tk);
        }else if (state == 20){
            //LD → OPRD opm OPRD
            tk2 = Stack.top();
            Stack.pop();
            tk1 = Stack.top();
            Stack.pop();
            tk = Stack.top();
            if(tk.type!=tk2.type || tk.type == "literal"){
                valid = false;
                std::cout<<"Semantic Error Operands does not match "<<line<<", column "<<column<<'\n';
            }
            std::string aux;
            if(tk.type == "inteiro")
            {
                aux = "itemp["+std::to_string(itemps++)+"]";
            }else
            {
                aux = "dtemp["+std::to_string(dtemps++)+"]";
            }
            if(generating){
                output<<aux<<" = "<<tk.lex<<" "<<tk1.lex<<" "<<tk2.lex<<";\n";
            }
            
            Stack.pop();

            tk.lex_class = "LD";
            tk.lex = aux;
            Stack.push(tk);
        }else if (state == 21){
            //LD → OPRD
            tk = Stack.top();
            Stack.pop();
            tk.lex_class = "LD";
            Stack.push(tk);
        }else if (state == 22){
            //OPRD → id
            tk = Stack.top();
            Stack.pop();
            if(tk.type=="NULL"){
                valid = false;
                std::cout<<"Semantic Error " << tk.lex<<" was not decleared at line "<<line<<", column "<<column<<'\n';
            }
            Stack.push(tk);
        }else if (state == 23){
            //OPRD → num
            tk = Stack.top();
            Stack.pop();
            Stack.push(tk);
        }else if (state == 25){
            //COND → CAB CP
            Stack.pop();
            Stack.pop();
        
            if(generating)
                output<<"}\n";

            tk.lex = tk.lex_class = tk.type = "COND";
            Stack.push(tk);
        }else if (state == 26){
            //CAB → se ab_p EXP_R fc_p entao
            Stack.pop();
            Stack.pop();
            tk = Stack.top();
            Stack.pop();
            Stack.pop();
            Stack.pop();
            if(generating)
                output<<"if("<<tk.lex<<"){\n";

            tk.lex = tk.lex_class = tk.type = "CAB";
            Stack.push(tk);
        }else if (state == 27){
            //EXP_R → OPRD opr OPRD
            tk2 = Stack.top();
            Stack.pop();
            tk1 = Stack.top();
            Stack.pop();
            tk = Stack.top();
            if(tk.type!=tk2.type || tk.type == "literal"){
                valid = false;
                std::cout<<"Semantic Error Operands does not match "<<line<<", column "<<column<<'\n';
            }

            std::string aux = "itemp["+std::to_string(itemps++)+"]";
            if(generating){
                output<<aux<<" = "<<tk.lex<<" "<<tk1.lex<<" "<<tk2.lex<<";\n";
                global_exp_r = aux+" = "+tk.lex+" "+tk1.lex+" "+tk2.lex+";\n";
            }
            Stack.pop();

            tk.lex_class = "EXP_R";
            tk.lex = aux;
            Stack.push(tk);
        }
        else if (state == 33){
            //R → CABR CPR
            Stack.pop();
            Stack.pop();
            if(generating){
                output<<global_exp_r;
                output<<"}\n";
            }

            tk.lex = tk.lex_class = tk.type = "R";
            Stack.push(tk);
        }else if (state == 34){
            //CABR → repita ab_p EXP_R fc_p
            Stack.pop();
            tk = Stack.top();
            Stack.pop();
            Stack.pop();
            Stack.pop();
            if(generating)
                output<<"while("<<tk.lex<<"){\n";

            tk.lex = tk.lex_class = tk.type = "CABR";
            Stack.push(tk);
        }else{
            for (int i = 0; i < dfa_syntax.sizeSTATE(state); i++)
                Stack.pop();
            tk.lex = tk.lex_class = tk.type = dfa_syntax.initial(state);
            Stack.push(tk);
        }
    }
    // ~Generator();
};