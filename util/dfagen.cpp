
#include<bits/stdc++.h>
using namespace std;

map<vector<pair<int,int>>,int> mp;
vector<pair<string,vector<string>>> rules;
set<pair<int,int>> substate[510];
map<string,set<pair<int,int>>> edges;
map<string,vector<int>> myr;
map<string,vector<string>> first;
map<string,vector<pair<int,string>>> follow;
int vis[510],vt=0;
void gensub(int id)
{
    vt++;
    queue<int> gen;
    for(auto now:substate[id]){
        for(int at:myr[rules[now.first].second[now.second]])
            gen.push(at);
    }
    while(!gen.empty())
    {
        int at = gen.front();
        gen.pop();
        if(vis[at]==vt)
            continue;
        vis[at]=vt;
        substate[id].insert({at,0});
        for(int at1:myr[rules[at].second[0]])
            gen.push(at1);
    }
}
vector<pair<int,int>> tovec(set<pair<int,int>> at)
{
    vector<pair<int,int>> now;
    for(auto c:at)
       now.push_back(c);
    return now;   
}

map<string,int> firstvis,followvis;
int cs = 1;
set<string> calcfirst(string now)
{
    firstvis[now] = cs;
    set<string> resp,us;
    if((now[0]>='a' && now[0]<='z') || now=="$"){
        // cout<<now<<"\n";
        resp.insert(now);
        return resp;
    }
    // cout<<now<<" aaaa\n";

    for(auto c:first[now]){
        if(firstvis[c]==cs)
            continue;
        us = calcfirst(c);
        for(auto at:us){
            resp.insert(at);
        }
    }
    return resp;
}

set<string> calcfollow(string now)
{
    // int x;
    // cin>>x;
    followvis[now]=cs;
    set<string> resp,us;
    for(auto c:follow[now]){

        // cout<<c.second<<'\n';
        if(c.first){
            if(c.second==now || followvis[c.second]==cs)
                continue;
            us = calcfollow(c.second);
            for(auto at:us)
                resp.insert(at);
        }else
        {
            // cout<<"OPA\n";
            if(firstvis[c.second]==cs)
                continue;
            us = calcfirst(c.second);
            for(auto at:us){
                resp.insert(at);
            }
        }
    }
    return resp;
}

int main()
{
    string s;
    int initial=0;
    ifstream fin("states.txt");
    while(getline(fin,s))
    {
        stringstream ss(s);
        string t,lst="not";
        ss>>t;
        ss>>t;
        pair<string,vector<string>> at;
        at.first = t;
        cout<<t<<'\n';
        if(!initial)
        {
            initial=1;
            follow[at.first].push_back({0,"$"});
        }
        while(ss>>t){
            if(lst!="not"){
                follow[lst].push_back({0,t});
            }else
            {
                // cout<<at.first<<' '<<t<<'\n';
                first[at.first].push_back(t);
            }
            lst=t;
            at.second.push_back(t);
        }
        if(t[0]<'a')
        {
            follow[t].push_back({1,at.first});
        }
        at.second.push_back("$");
        myr[at.first].push_back(rules.size());
        rules.push_back(at);
    }
    ofstream fouted("DFAedges.txt");
    ofstream foutst("DFAstates.txt");
    
    queue<int> f;
    f.push(0);
    substate[0].insert({0,0});
    mp[tovec(substate[0])]=0;
    int k=1;
    while(!f.empty())
    {
        int at = f.front();
        f.pop();
        gensub(at);
        foutst<<at+1<<"\n";
        fouted<<at+1<<"\n";
        
        edges.clear();
        vector<string> order;
        map<string,int> used;
        for(auto at:substate[at])
        {
            foutst<<at.first + 1<<" "<<rules[at.first].first<<" ";
            // cout<<at.first + 1<<" "<<rules[at.first].first<<"<<<\n";
            
            if(at.second == rules[at.first].second.size()-1)
            {
                // for(auto v:follow[rules[at.first].first])
                //     cout<<v.second<<" ";
                // cout<<"\n";
                cs++;
                for(auto v:calcfollow(rules[at.first].first))
                {
                    if(!at.first && v=="$")
                        fouted<<"ACC ";
                    else{
                        fouted<<"R ";
                    }
                    if(used[v]==1){
                        cout<<"JA USEI O "<<v<<" "<<at.first + 1<<"deu uma merda\n";
                        exit(1);
                    }
                    used[v]=1;
                    fouted<<at.first + 1<<' '<<v<<"\n";
                }
                // int x;
                // cin>>x;
            }
            for (int i = 0; i < rules[at.first].second.size(); ++i)
            {
                if(i==at.second){
                    foutst<<". ";
                    if(rules[at.first].second[i]!="$"){
                        if(edges.find(rules[at.first].second[i])==edges.end())
                            order.push_back(rules[at.first].second[i]);
                        edges[rules[at.first].second[i]].insert({at.first,at.second+1});
                    }
                }
                if(rules[at.first].second[i]!="$")
                    foutst<<rules[at.first].second[i]<<" ";
            }
            foutst<<'\n';
        }
        // if(!order.empty())
        //     cout<<"Arestas\n";
        for(auto c:order)
        {
            auto veced=tovec(edges[c]);
            if(mp.find(veced)==mp.end())
            {
                substate[k]=edges[c];
                f.push(k);
                mp[veced]=k++;
            }
            if(c[0]>='a')
                fouted<<"S ";
            else
                fouted<<"G ";
            fouted<<mp[veced] + 1<<" "<<c;
            fouted<<"\n";
        }
    }
    // for(auto c:follow["D"])
    //     cout<<c.first<<' '<<c.second<<'\n';

    // for(auto c:calcfollow("D"))
    //     cout<<c<<'\n';
}
