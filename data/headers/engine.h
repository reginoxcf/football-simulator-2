#include <iostream>
#include <random>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <vector>

//mt19937
std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
//take random number
int take(int l, int r){return rng()%(r-l+1)+l;}

struct team{
    std::string name;
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

//generate round robin schedule
std::vector<std::pair<int, int>> round_robin(int noTeams){
    std::vector<int> cyc(noTeams-1);
    iota(cyc.begin(), cyc.end(), 1);
    shuffle(cyc.begin(), cyc.end(), rng);
    std::vector<std::pair<int, int>> vtmp; vtmp.clear();
    std::vector<std::pair<int, int>> schedule; schedule.clear();
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
    return schedule;
}

//engine thingy
int enginev0(team A, team B, double drawRate){
    //really simple, but works! (doesn't)
    int a = A.ovr, b = B.ovr; //power rating
    double random = (double)take(1, 1e9)/1e9*(double)(a+b)*(1.0 + drawRate);
    double drawChance = drawRate * (double)(a+b);
    if(random <= drawChance) return 0;
    if(random <= drawChance + (double)a) return 1;
    return -1;
}

std::pair<int, int> simulate(team A, team B, double drawrate=1.775){
    //doesn't work as well as expected (tbf it is sh*t)
    int chances = take(7, 13);
    std::pair<int, int> score = {0, 0};
    for(int i = chances; i; i--){
        int r = enginev0(A, B, drawrate);
        if(r == -1) score.second++;
        else if(r == 1) score.first++;
    }
    return score;
}

//print standings
void printStandings(std::vector<team> t, std::string OUTFILE, std::string printMode){
    int noTeams = t.size();
    //sort standings
    std::sort(t.begin(), t.end(), [&](team a, team b){
        if(a.pts() == b.pts()){
            if(a.gd() == b.gd()) return a.gf > b.gf;
            else return a.gd() > b.gd();
        }
        return a.pts() > b.pts();
    });
    //start outputing
    std::ofstream file(OUTFILE);
    if(printMode == "full"){
        file << "#,Team,Pld,Pts,W,D,L,+/-,GD,Ovr\n";
        for(int i = 0; i < noTeams; i++){
            file << i+1 << " ";
            if(i == 0) file << "(C),";
            else if(i < 3) file << "(P),";
            else if(i > noTeams-4) file << "(R),";
            else file << ",";
            file << t[i].name << "," << t[i].pld << ","  << t[i].pts() << "," << t[i].w << "," << t[i].d << "," << t[i].l << "," << t[i].gf << "-" << t[i].ga << "," << t[i].gd() << "," << t[i].ovr << "\n";
        }
    }
    else if(printMode == "short"){
        file << "#,Team,Pld,Pts,GD,Ovr\n";
        for(int i = 0; i < noTeams; i++){
            file << i+1 << " ";
            if(i == 0) file << "(C),";
            else if(i < 3) file << "(P),";
            else if(i > noTeams-4) file << "(R),";
            else file << ",";
            file << t[i].name << "," << t[i].pld << "," << t[i].pts() << "," << t[i].gd() << "," << t[i].ovr << "\n";
        }
    }
    else{
        file << "ERROR: cannot detect valid printMode";
    }
    file.close();
}