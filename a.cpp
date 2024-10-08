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
const int noTeams = 20; //number of teams

vector<string> rngNames, rngSuffs;

vector<pair<int, int>> schedule;

struct team{
    string name;
    int ovr;
    int pld;
    int w, l, d;
    int gf, ga;
    int pts(){
        return w*3+d;
    }
    int gd(){
        return gf - ga;
    }
};
team t[noTeams], t2[noTeams];

//Functions
int take(int l, int r){return rng()%(r-l+1)+l;}

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
    for(int i = 0; i < noTeams; i++){
        t[i].name = rngNames[i] + " " + rngSuffs[take(0, rngSuffs.size()-1)];
        t[i].ovr = take(x, y);
    }
    input.close();
    
    //circle method for round robin generation
    vector<int> cyc(noTeams-1);
    iota(cyc.begin(), cyc.end(), 1);
    shuffle(cyc.begin(), cyc.end(), rng);
    vector<pair<int, int>> vtmp; vtmp.clear();
    for(int i = 0; i < noTeams-1; i++){
        vtmp.push_back({0, cyc[0]});
        for(int j = 1; j <= (noTeams-1)/2; j++){
            vtmp.push_back({cyc[j], cyc[noTeams-1-j]});
        }
        shuffle(vtmp.begin(), vtmp.end(), rng);
        while(vtmp.size()){
            schedule.push_back(vtmp.back());
            vtmp.pop_back();
        }
        rotate(cyc.begin(), cyc.begin()+1, cyc.end());
    }

    for(auto [x, y]:schedule){
        schedule.push_back({y, x});
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
    //doesn't work as well as expected (tbf it is sh*t)
    int chances = take(7, 13);
    pair<int, int> score = {0, 0};
    for(int i = chances; i; i--){
        int r = enginev0(A, B, 1.75);
        if(r == -1) score.second++;
        else if(r == 1) score.first++;
    }
    return score;
}

void printStandings(){
    for(int i = 0; i < noTeams; i++){
        t2[i] = t[i];
    }
    sort(t, t+noTeams, [&](team a, team b){
        if(a.pts() == b.pts()){
            if(a.gd() == b.gd()) return a.gf > b.gf;
            else return a.gd() > b.gd();
        }
        return a.pts() > b.pts();
    });
    ofstream file("standings.csv");
    file << "#,Team,Pld,Pts,W,D,L,GF,GA,GD,Ovr\n";
    for(int i = 0; i < noTeams; i++){
        file << i+1 << " ";
        if(i == 0) file << "(C),";
        else if(i == 1) file << "(P),";
        else if(i < 6) file << "(Pl),";
        else if(i > noTeams-4) file << "(R),";
        else file << ",";
        file << t[i].name << "," << t[i].pld << ","  << t[i].pts() << "," << t[i].w << "," << t[i].d << "," << t[i].l << "," << t[i].gf << "," << t[i].ga << "," << t[i].gd() << "," << t[i].ovr << "\n";
    }
    for(int i = 0; i < noTeams; i++){
        t[i] = t2[i];
    }
    file.close();
}

// INT MAIN
int main(){
    import();
    
    ofstream res;
    res.open("matches.csv");
    res.close();
    bool md_b_md = false;
    for(int i = 0; i < (int)schedule.size(); i++){
        if(md_b_md){
            printStandings();
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
        res.open("matches.csv", std::ios_base::app);
        if(i % 10 == 0){
            if(md_b_md) std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            res << "Matchday " << i/10+1 << "\n";
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

    printStandings();
    return 0;
}
