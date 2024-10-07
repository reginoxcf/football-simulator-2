#include<bits/stdc++.h>
#define ll long long
#define all(v) begin(v), end(v)
#define pi pair<int, int>
#define vi vector<int>
#define task "test"
using namespace std;
//-1 for loss, 0 for draw, 1 for win
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
//Structs, variables, etc.
int noTeams; //number of teams

vector<string> rngNames, rngSuffs;

struct team{
    string name;
    int ovr;
    int w, l, d;
    int gd;
    int pts(){
        return w*3+d;
    }
};
vector<team> t;
vector<pair<int, int>> schedule;

//Functions
int take(int l, int r){return rng()%(r-l+1)+l;}

void init(){
    ifstream input;
    input.open("in.txt");
    input >> noTeams; //get number of teams
    input.close();

    //import random team names
    input.open("uk-cities.txt");
    string tmp;
    while(getline(input, tmp)){
        rngNames.push_back(tmp);
    }
    shuffle(rngNames.begin(), rngNames.end(), rng);
    input.close();
    input.open("team-suffixes.txt");
    while(getline(input, tmp)){
        rngSuffs.push_back(tmp);
    }
    input.close();
    
    //generate schedule


    //import team names from file (last step)
    input.open("in.txt");
    t.resize(noTeams);
    for(int i = 0; i < noTeams; i++){
        getline(input, t[i].name);
        getline(input, t[i].name);
        input >> t[i].ovr;
        // cin >> t[i].ovr;
    }
}  

int enginev0(team A, team B, double drawRate){
    //really simple, but works!
    int a = A.ovr, b = B.ovr; //power rating
    double random = (double)take(1, 1e9)/1e9*(double)(a+b)*(1.0 + drawRate);
    double drawChance = drawRate * (double)(a+b);
    if(random <= drawChance) return 0;
    if(random <= drawChance + (double)a) return 1;
    return -1;
}

pair<int, int> simulate(team A, team B){
    int chances = take(1, 10);
    pair<int, int> score = {0, 0};
    for(int i = chances; i; i--){
        int r = enginev0(A, B, 0.2);
        if(r == -1) score.second++;
        else if(r == 1) score.first++;
    }
    return score;
}

// INT MAIN
int main(){
    init();
    // simulation start
    for(int i = 0; i < noTeams; i++){
        for(int j = 0; j < noTeams; j++){
            if(i == j) continue;
            pair<int, int> result = simulate(t[i], t[j]);
            int d = result.first - result.second;
            t[i].gd+=d; t[j].gd-=d;
            if(d < 0){
                t[i].l++, t[j].w++;
            }
            else if(d == 0){
                t[i].d++; t[j].d++;
            }
            else {
                t[j].l++, t[i].w++;
            }
        }
    }

    sort(t.begin(), t.end(), [&](team a, team b){
        if(a.pts() == b.pts()) return a.gd > b.gd;
        return a.pts() > b.pts();
    });

    ofstream file("standings.csv");
    file << ",Team,Pts,W,D,L,GD,Ovr\n";
    // int playoffPromote = take(2, 5);
    for(int i = 0; i < noTeams; i++){
        file << i+1 << " ";
        if(i == 0) file << "(C),";
        else if(i < 4) file << "(UCL),";
        else if(i == 4) file << "(UEL),";
        // else if(i < 6) file << "(Pl),";
        else if(i > noTeams-4) file << "(R),";
        else file << ",";
        file << t[i].name << "," << t[i].pts() << "," << t[i].w << "," << t[i].d << "," << t[i].l << "," << t[i].gd << "," << t[i].ovr << "\n";
    }
    return 0;
}
