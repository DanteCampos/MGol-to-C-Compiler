#include <string>
#include <map>
#include<vector>
#include <fstream>
#include <sstream>
#include <string>

class DFAsintax {
    private:
        std::vector< std::map< std::string, std::tuple< std::string,int > > > actions;
        std::vector< std::map< std::string,int > > gotos;
        std::vector< int > sz;
        std::vector< std::vector< std::string > > vals;
    public:
        DFAsintax(){
            std::ifstream finst("states.txt"),fined("DFAedges.txt");
            std::string s,t;
            sz.push_back(0);
            int nst=1;
            vals.push_back( std::vector< std::string >());
            while(getline(finst,s))
            {
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
            nst=1;
            gotos.push_back(std::map< std::string, int >());
            actions.push_back(std::map< std::string, std::tuple< std::string, int > >());
            while(getline(fined,s))
            {
                if(s[0]<='9' && s[0]>='0'){
                    gotos.push_back(std::map< std::string, int >());
                    actions.push_back(std::map< std::string, std::tuple< std::string, int > >());
                    nst=std::stoi(s);
                    // std::cout<<nst<<"\n";
                }else
                {
                    std::stringstream ss(s);
                    std::string type,node;
                    int st;
                    ss>>type>>st>>node;
                    if(type=="G")
                    {
                        gotos[nst][node]=st;
                    }else
                    {
                        actions[nst][node]=std::make_tuple(type,st);
                    }
                }
            }
        };

        // Transition the state of the DFA based on the given symbol
        std::tuple< std::string, int > ACTION(int state, std::string term){ 
            if(actions[state].find(term)==actions[state].end())
                return std::make_tuple("ERROR",-1);
            return actions[state][term];
        }

        int GOTO(int state, std::string notterm){ 
            if(gotos[state].find(notterm)==gotos[state].end())
                return -1;
            return gotos[state][notterm];
        }

        int sizeSTATE(int state)
        {
            return sz[state];
        }

        std::string initial(int state)
        {
            return vals[state][0];
        }

        void showSTATE(int state)
        {
            std::cout<<vals[state][0]<<" -> ";
            for (int i = 1; i < vals[state].size() ; ++i)
            {
                std::cout<<vals[state][i]<<' ';
            }
            std::cout<<"\n";
        }
};

