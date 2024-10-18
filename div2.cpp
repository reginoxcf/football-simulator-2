/*
DIVISION 1 SIMULATOR
*/

#include<bits/stdc++.h>
using namespace std; 
//god forgive me for using the above

//Structs, variables, etc.
int noTeams;
int noRelegated;
int noPromoted;

const string matchFile = "data/matches/div2.csv";
const string standingsFile = "data/standings/div2.csv";
const string standingsType = "short";

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
    input.open("data/divisions/div2.txt");
    noTeams = 20; //always 20
    noPromoted = 3; //three promoted to division 1
    noRelegated = 0; // no relegations
    t.resize(0); t.resize(noTeams);
    string temp, proc;
    for(int i = 0; i < noTeams; i++){
        getline(input, temp); //contains team name and ovr
        //process "temp"
        bool mode = true; //true = name, false = pos
        for(auto x:temp){
            if(x == ','){
                t[i].name = proc;
                proc.clear();
                mode = false;
            }
            else proc += x;
        }
        t[i].ovr = stoi(proc);
        proc.clear();
    }
}

string teamFormat(int position){
    return t[position].name + " (" + std::to_string(t[position].pts()) + ")";
}

void processPRs(){
    sort(t.begin(), t.end(), [&](team a, team b){
        if(a.pts() == b.pts()) return a.gd() > b.gd();
        return a.pts() > b.pts();
    });
    //process promotions and relegations
    ofstream print("data/history.txt",std::ios_base::app);
    print << "Div. 2:\n";
    print << "Champions: " << teamFormat(0) << "\n";
    if(noPromoted > 1) print << "Promoted: \n";
    for(int i = 1; i < noPromoted; i++) print << teamFormat(i) << "\n";
    if(noRelegated > 0) print << "Relegated: \n";
    for(int i = noTeams - noRelegated; i < noTeams; i++) print << teamFormat(i) << "\n";
    print << "\n";
    print.close();
    print.open("data/divisions/div2.txt");
    for(int i = 0; i < noTeams; i++){
        print << t[i].name << "," << t[i].ovr << "\n";
    }
    print.close();
}

// INT MAIN
int main(){
    importAndGen();
    vector<pair<int, int>> schedule = round_robin(noTeams);
    t = simulateSeason(schedule, t, matchFile, 0);
    printStandings(t, standingsFile, "full");
    processPRs();
    return 0;
}
