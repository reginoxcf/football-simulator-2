#include <bits/stdc++.h>
using namespace std;
const string team = "Leicester City";
int main(){
    ifstream get("data/divisions/div1.txt");
    string s;
    for(int i = 1; i <= 20; i++){
        getline(get, s);
        if(s.find(team) != string::npos){
            ofstream out("tracker_log.txt", ios_base::app);
            out << "Div. 1 - " << i;
            if(i== 1) out << " (C)";
            else if(i> 17) out << " (R)";
            out << "\n";
            out.close();
            exit(0);
        }
    }
    get.close();
    get.open("data/divisions/div2.txt");
    for(int i = 1; i <= 20; i++){
        getline(get, s);
        if(s.find(team) != string::npos){
            ofstream out("tracker_log.txt", ios_base::app);
            out << "Div. 2 - " << i;
            if(i== 1) out << " (C)";
            else if(i < 4) out << " (P)";
            out << "\n";
            out.close();
            exit(0);
        }
    }
}