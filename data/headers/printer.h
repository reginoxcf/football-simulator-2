#include <fstream>
#include <vector>
#include <algorithm>

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