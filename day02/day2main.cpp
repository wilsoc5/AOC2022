#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

std::map<string,int> win_map {
     {"A X", 4}
    ,{"A Y", 8}
    ,{"A Z", 3}
    ,{"B X", 1}
    ,{"B Y", 5}
    ,{"B Z", 9}
    ,{"C X", 7}
    ,{"C Y", 2}
    ,{"C Z", 6}
    };

std::map<string,string> end_to_win{
     {"A X", "A Z"}
    ,{"A Y", "A X"}
    ,{"A Z", "A Y"}
    ,{"B X", "B X"}
    ,{"B Y", "B Y"}
    ,{"B Z", "B Z"}
    ,{"C X", "C Y"}
    ,{"C Y", "C Z"}
    ,{"C Z", "C X"}
};
uint32_t score_line(string line){
    auto it = win_map.find(line);
    if (it == win_map.end()){
        cerr <<"Unable to find string: " <<line;
        exit(1);
    }
    return it->second;
}

uint32_t score_line_p2(string line){
    auto it = end_to_win.find(line);
    if (it == end_to_win.end()){
        cerr <<"Unable to find string: " <<line;
        exit(1);
    }
    return score_line(it->second);
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t score(0);
    uint64_t score_p2(0);
    uint64_t line_count = 0;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        score += score_line(line);
        score_p2 += score_line_p2(line);
        ++line_count;
    }
    //read input file. Now compute max sum:
    
    cout <<"Max score is " <<score <<endl;
    cout <<"Max score Part2 is " <<score_p2 <<endl;

    return 0;
}
