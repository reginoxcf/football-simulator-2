#include<bits/stdc++.h>
using namespace std; 
//god forgive me for using the above


//USER DEFINED HEADERS
#include "data/headers/team_structure.h"
#include "data/headers/engine.h"
#include "data/headers/round_robin.h"
#include "data/headers/printer.h"

//Structs, variables, etc.
const int noTeams = 20; //number of teams

vector<string> rngNames, rngSuffs;
vector<team> t;

//Functions

//generates team data (name, ovr)
void import(){
    ifstream input;

    input.open("data/team_names/uk-cities.txt");
    string tmp;
    while(getline(input, tmp)){
        rngNames.push_back(tmp);
    }
    shuffle(rngNames.begin(), rngNames.end(), rng);
    input.close();

    input.open("data/team_names/team-suffixes.txt");
    while(getline(input, tmp)){
        rngSuffs.push_back(tmp);
    }
    input.close();
    
    input.open("in.txt");
    int x, y; input >> x >> y;
    t.resize(noTeams);
    for(int i = 0; i < noTeams; i++){
        t[i].name = rngNames[i] + " " + rngSuffs[take(0, rngSuffs.size()-1)];
        t[i].ovr = take(x, y);
    }
    input.close();
}

// INT MAIN
int main(){
    import();
    vector<pair<int, int>> schedule = round_robin(noTeams);
    
    ofstream res;
    res.open("matches.csv");
    res << "Home,#,#,Away\n";
    res.close();
    bool md_b_md = false;
    for(int i = 0; i < (int)schedule.size(); i++){
        if(md_b_md){
            printStandings(t, "standings.csv", "short");
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
        res.open("matches.csv", std::ios_base::app);
        if(i % 10 == 0){
            if(md_b_md) std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            res << "Matchday " << i/10+1 << ",,,\n";
        }
        int t1 = schedule[i].first, t2 = schedule[i].second;
        pair<int, int> result = simulate(t[t1], t[t2]);
        int d = result.first - result.second;
        t[t1].gf += result.first; t[t1].ga += result.second;
        t[t2].gf += result.second; t[t2].ga += result.first;
        t[t1].pld++; t[t2].pld++;
        if(d < 0){
            t[t1].l++, t[t2].w++;
        }
        else if(d == 0){
            t[t1].d++; t[t2].d++;
        }
        else {
            t[t2].l++, t[t1].w++;
        }
        res << t[t1].name << " (" << t[t1].ovr << ")," << result.first << "," << result.second << "," << t[t2].name << " (" << t[t2].ovr << ")" << "\n";
        res.close(); 
    }

    printStandings(t, "standings.csv", "short");
    return 0;
}
