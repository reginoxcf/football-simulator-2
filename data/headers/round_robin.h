//for lack of a better word
//generator, simulator, and printer

#include <vector>
#include <algorithm>
#include <numeric>

//round robin generator using circle algorithm
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
