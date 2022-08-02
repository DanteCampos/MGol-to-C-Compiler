#pragma once
#include <string>
#include <map>
#include<vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class DFAsintax {
    private:
        // actions[state][lex_class] -> (action, rule_number)
        std::vector< std::map< std::string, std::tuple< std::string, int > > > actions;

        // gotos[state][lex_class] -> next state
        std::vector< std::map< std::string,int > > gotos;

        std::vector< int > sz;
        std::vector< std::vector< std::string > > vals;
    
    public:
        // Build DFA from states and rules files
        DFAsintax(){
            std::ifstream finst("util/states.txt"), fined("util/DFAedges.txt");
            std::string s, t;
            int nst = 1;

            sz.push_back(0);
            vals.push_back( std::vector< std::string >());

            while(getline(finst,s)){
                int qtd=0;
                vals.push_back( std::vector< std::string >());
                std::stringstream ss(s);
                while(ss>>t){
                    if(qtd!=0)
                        vals[nst].push_back(t);
                    qtd++;
                }
                nst++;
                sz.push_back(qtd-2);
            }

            nst = 1;
            gotos.push_back(std::map< std::string, int >());
            actions.push_back(std::map< std::string, std::tuple< std::string, int > >());
            
            while(getline(fined,s)){
                if(s[0]<='9' && s[0]>='0'){
                    gotos.push_back(std::map< std::string, int >());
                    actions.push_back(std::map< std::string, std::tuple< std::string, int > >());
                    nst = std::stoi(s);
                    // std::cout<<nst<<"\n";
                }else{
                    std::stringstream ss(s);
                    std::string type,node;
                    int st;

                    ss>>type>>st>>node;
                    if(type == "G")
                        gotos[nst][node]=st;
                    else
                        actions[nst][node] = std::make_tuple(type,st);
                }
            }

            finst.close();
            fined.close();
        };

        // Returns (action, rule_number)
        // Parameters: state at the top of the stack, lexical class of the token readed
        std::tuple< std::string, int > ACTION(int state, std::string term){ 
            if(actions[state].find(term)==actions[state].end())
                return std::make_tuple("ERROR",-1);
            return actions[state][term];
        }
        
        // Returns the state after making a reduction
        // Parameters: state at the top of the stack, symbol at the left side of the rule used in the reduction
        int GOTO(int state, std::string notterm){ 
            if(gotos[state].find(notterm)==gotos[state].end())
                return -1;
            return gotos[state][notterm];
        }

        // Returns how many symbols must be popped from the stack to reduce at given state
        // Parameters: actual state
        int sizeSTATE(int state){
            return sz[state];
        }

        std::string initial(int state){
            return vals[state][0];
        }

        // Show the gramatic rule used in the reduction
        // Returns the left side of the derivation
        // Parameter: rule number
        std::string showSTATE(int state){
            std::cout<<vals[state][0]<<" -> ";
            for (int i = 1; i < vals[state].size() ; ++i)
                std::cout<<vals[state][i]<<' ';
            std::cout<<"\n";
            return vals[state][0];
        }

        std::string getErrorCode(int state){
            return "ERS1";
        }
};

