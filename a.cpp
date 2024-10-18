#include <bits/stdc++.h>
// using namespace std;

const int seasons = 100;

void processChanges(){
    //Top 3 from division 2 get promoted, bottom 3 from division 1 get relegated
    std::vector<std::string> temp;
    std::ifstream get("data/divisions/div1.txt");
    int d1 = 20, d2 = 20, swapcnt = 3;
    std::string inp;
    for(int i = 0; i < d1; i++){
        getline(get, inp);
        temp.push_back(inp);
    }
    get.close();
    get.open("data/divisions/div2.txt");
    for(int i = 0; i < d2; i++){
        getline(get, inp);
        temp.push_back(inp);
    }
    for(int i = -3; i < 0; i++){
        swap(temp[d1+i], temp[d1+i+swapcnt]);
    }
    std::ofstream send("data/divisions/div1.txt");
    for(int i = 0; i < d1; i++){
        send << temp[i] << "\n";
    }
    send.close();
    send.open("data/divisions/div2.txt");
    for(int i = d1; i < d1+d2; i++){
        send << temp[i] << "\n";
    }
    send.close();
}

void compile(std::string name){
    std::string command = "g++ " + name + ".cpp -o " + name;
    system(command.c_str());
}

void run(std::string name){
    std::string command = name;
    #ifdef linux
        command = "./" + command;
    #endif

    system(command.c_str());
}

int main(){
    //input
    std::vector<std::string> input;
    std::ifstream get("in.txt");
    std::string get_s;
    while(getline(get, get_s)){
        input.push_back(get_s);
    }

    std::ofstream send("data/divisions/div1.txt");
    for(int i = 0; i < 20; i++){
        send << input[i] << "\n";
    }
    send.close();
    send.open("data/divisions/div2.txt");
    for(int i = 20; i < 40; i++){
        send << input[i] << "\n";
    }
    send.close();
    //main program
    std::ofstream print("data/history.txt");
    print.close();
    compile("div1");
    compile("div2");
    for(int i = 1; i <= seasons; i++){
        print.open("data/history.txt", std::ios_base::app);
        print << "--- SEASON " << i << " ---\n";
        print.close();
        run("div1");
        run("div2");
        processChanges();
        std::cout << "Finished season " << i << ". Press any key to continue\n";
        // std::cin.get();
    }
}