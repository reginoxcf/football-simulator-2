#include<bits/stdc++.h>
using namespace std; 
//god forgive me for using the above

//Structs, variables, etc.
int noTeams;
int noRelegated;
int noPromoted;

const int seasons = 5;
const string matchFile = "matches.csv";
const string standingsFile = "standings.csv";

//USER DEFINED HEADERS
/*
Always put team_structure.h above all other user-defined headers
, since the headers below need the struct "team" to compile

It is best to keep the order of headers as is
*/
#include "data/headers/team_structure.h" 
#include "data/headers/engine.h"
#include "data/headers/round_robin.h"
#include "data/headers/printer.h"
#include "data/headers/simulator.h"

// vector<string> rngNames, rngSuffs;
vector<team> t;

//Functions

//generates team data (name, ovr)
void importAndGen(){
    ifstream input;
    input.open("in.txt");
    input >> noTeams >> noPromoted >> noRelegated;
    t.resize(0); t.resize(noTeams);
    string temp; getline(input, temp); // trash input
    for(int i = noTeams-1; i >= 0; i--){
        getline(input, t[i].name);
    }
    for(int i = noTeams-1; i >= 0; i--){
        input >> t[i].ovr;
    }
}

// INT MAIN
int main(){
    for(int i = 1; i <= seasons; i++){
        importAndGen();
        vector<pair<int, int>> schedule = round_robin(noTeams);
        t = simulateSeason(schedule, t, matchFile, 0);
        printStandings(t, standingsFile, "full");
        system("pause");
    }
    return 0;
}
