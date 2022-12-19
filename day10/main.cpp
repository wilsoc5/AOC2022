#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>
#include <sstream>
#include <deque>
#include <numeric>
#include <unordered_set>
#include <functional>
#include <array>
#include <utility>

using namespace std;

template<typename T, typename U>
ostream& operator<<(ostream& os, const std::pair<T,U>& p){
    os <<"{ " <<p.first <<" , "<<p.second <<" }";
    return os;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    for (auto& a : v){
        os <<a <<" ";
    }
    os <<"\n";
    return os;
}

std::vector<string> draw_history(const std::vector<std::pair<uint64_t, int64_t>>& history){
    std::vector<string> screen;
    int64_t sprite_position = history.front().second;
    string row = "";
    for (auto it = history.cbegin(); it != history.end(); ++it){
        int64_t rowp = it->first-1;
        int64_t rowc = rowp % 40;
        if (rowc == 0){
            screen.push_back(row);
            cout <<row <<endl;
            row = "";
        }
        sprite_position = it->second;
        if (abs(sprite_position - rowc) < 2) row.push_back('#');
        else row.push_back('.');
        cout <<*it <<" SP: " <<sprite_position <<" " <<row <<endl;
    }
    screen.push_back(row);
    cout <<row <<endl;

    return screen;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t line_count = 0;
    uint64_t visited = 0;
    uint64_t p2score = 0;

    uint64_t cycle_counter = 1;
    int64_t reg = 1;

    vector<std::pair<uint64_t,int64_t>> history;

    //history.emplace_back(make_pair(cycle_counter,reg));
    auto apply_instruction = [&history,&cycle_counter,&reg]( std::string line) {
        stringstream ss(line);
        string direction;
        ss >>direction ;
        int16_t value;
        history.emplace_back(make_pair(cycle_counter,reg));
        switch (direction[0]){
            case 'n':
                ++cycle_counter;
                break;
            case 'a':
                ss >>value;
                ++cycle_counter;
                history.emplace_back(make_pair(cycle_counter,reg));
                ++cycle_counter;
                reg += value;
                break;
        }
    };

    string direction;

    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        apply_instruction(line);
        
        cout <<cycle_counter <<" "<<reg<<endl;
        ++line_count;
    }
    //read input file. Now compute max sum:
    cout <<history <<endl;

    string starts ="19";
    string skips ="40";
    if (argc > 2){
        starts = argv[2];
    }
    if (argc > 3){
        skips = argv[3];
    }
    uint64_t start = stoi(starts);
    uint64_t skip = stoi(skips);
    
    cout <<history.size() <<endl;
    int64_t signal_strength = 0;
    for(size_t i = start; i < history.size(); i += skip){
        int64_t ssi =  (history[i].first) * history[i].second;
        signal_strength += ssi;
        cout <<history[i] <<" " <<ssi << "\t" <<signal_strength<<endl;
    }
    cout <<"Signal strength "<<signal_strength <<endl;

    auto sh = draw_history(history);
    cout <<"============================="<<endl;
    for (auto s : sh){
        cout <<s <<endl;
    }

    return 0;
}
