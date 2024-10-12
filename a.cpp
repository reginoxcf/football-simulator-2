#include<bits/stdc++.h>
using namespace std; 
//god forgive me for using the above

//USER DEFINED HEADERS
/*
Always put team_structure.h above all other user-defined headers, since the headers below need the struct "team" to compile

It is best to keep the order of headers as is
*/
#include "data/headers/team_structure.h" 
#include "data/headers/engine.h"
#include "data/headers/round_robin.h"
#include "data/headers/printer.h"
#include "data/headers/simulator.h"

//Structs, variables, etc.
const int noTeams = 10; //number of teams

vector<string> rngNames, rngSuffs;
vector<team> t;

//Functions

//generates team data (name, ovr)
void importAndGen(){
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
    importAndGen();
    vector<pair<int, int>> schedule = round_robin(noTeams);
    string matchFile = "matches.csv";
    string standingsFile = "standings.csv";
    setDefaults(2, 2, standingsFile, "short");
    t = simulateSeason(schedule, t, matchFile, 0);
    printStandings(t, standingsFile, "full", noPromoted, noRelegated);
    return 0;
}
