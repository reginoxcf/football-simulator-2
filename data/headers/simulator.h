#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

void simulateSeason(std::vector<std::pair<int, int>> schedule, std::vector<team> teamList, std::string outFile, bool matchByMatch){
    std::ofstream res;
    res.open(outFile);
    res << "Home,#,#,Away\n";
    res.close();
    bool matchByMatch = false;
    for(int i = 0; i < (int)schedule.size(); i++){
        if(matchByMatch){
            printStandings(teamList, "standings.csv", "short");
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
        res.open(outFile, std::ios_base::app);
        if(i % 10 == 0){
            if(matchByMatch) std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            res << "Matchday " << i/10+1 << ",,,\n";
        }
        int team1 = schedule[i].first, team2 = schedule[i].second;
        std::pair<int, int> result = simulate(teamList[team1], teamList[team2]);
        int d = result.first - result.second;
        teamList[team1].gf += result.first; teamList[team1].ga += result.second;
        teamList[team2].gf += result.second; teamList[team2].ga += result.first;
        teamList[team1].pld++; teamList[team2].pld++;
        if(d < 0){
            teamList[team1].l++, teamList[team2].w++;
        }
        else if(d == 0){
            teamList[team1].d++; teamList[team2].d++;
        }
        else {
            teamList[team2].l++, teamList[team1].w++;
        }
        res << teamList[team1].name << " (" << teamList[team1].ovr << ")," << result.first << "," << result.second << "," << teamList[team2].name << " (" << teamList[team2].ovr << ")" << "\n";
        res.close(); 
    }
}