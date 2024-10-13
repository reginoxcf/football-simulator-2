#include <iostream>
#include <algorithm>

//take random number
int take(int l, int r){return rng()%(r-l+1)+l;}

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

std::pair<int, int> simulate(team A, team B, double drawrate=1.75){
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
