#include <string>
#include <chrono>
#include <random>
//mt19937
std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

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