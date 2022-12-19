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

using namespace std;

using point = std::pair<int16_t, int16_t>;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    for (auto& a : v){
        os <<a <<" ";
    }
    os <<"\n";
    return os;
}

ostream& operator<<(ostream& os, const point& p){
    os <<"{ " <<p.first <<", " <<p.second <<" } ";
    return os;
}

struct hashPair{
    size_t operator()(const point& p) const{
        return std::hash<size_t>()(std::hash<int16_t>()(p.first) + std::hash<int16_t>()(p.second));
    }
};

void update_tail(const point& hp, point& tp){
    auto ud = hp.first - tp.first;
    auto lr = hp.second - tp.second;
    if ((abs(ud)+abs(lr) >= 3)){
        //diagonal
        tp.first += signbit(ud) ? -1 : 1;
        tp.second += signbit(lr) ? -1 : 1;
    } else {
        tp.first += ud / 2;
        tp.second += lr / 2;
    }
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

    point head_pos = {0,0};
    point tail_pos = {0,0};

    int16_t z = 0;
    std::array<point,10> knots {
         point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
        ,point{z,z}
    };
    std::unordered_set<point, hashPair> history;
    std::unordered_set<point, hashPair> history_p2;

    auto apply_direction = [&history,&head_pos,&tail_pos]( char direction, int16_t length){
        while (length-- > 0){
            switch (direction){
                case 'U':
                    ++head_pos.first;
                    break;
                case 'D':
                    --head_pos.first;
                    break;
                case 'R':
                    ++head_pos.second;
                    break;
                case 'L':
                    --head_pos.second;
                    break;
            }
            update_tail(head_pos, tail_pos);
            history.insert(tail_pos);
        }
    };

    auto apply_direction_p2 = [&history_p2,&knots]( char direction, int16_t length){
        while (length-- > 0){
            switch (direction){
                case 'U':
                    ++knots[0].first;
                    break;
                case 'D':
                    --knots[0].first;
                    break;
                case 'R':
                    ++knots[0].second;
                    break;
                case 'L':
                    --knots[0].second;
                    break;
            }
            for (int i = 1; i < 10; ++i)
                update_tail(knots[i-1], knots[i]);
            history_p2.insert(knots[9]);
        }
    };


    bool stacking = true;
    char direction;
    int16_t distance=0;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        stringstream ss(line);
        ss >>direction >>distance;
        apply_direction(direction,distance);
        apply_direction_p2(direction,distance);
        
        cout <<direction <<" "<<distance <<" Head: " <<head_pos <<" Tail: " <<tail_pos <<" "<<history.size() <<" P2: " <<knots[9] << " "<<history_p2.size() <<endl;
        ++line_count;
    }
    //read input file. Now compute max sum:
    cout <<"Visited: " << visited <<endl;

    return 0;
}
