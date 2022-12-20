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
#include <queue>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<vector<T>>& p){
    for (const auto& v : p){
        for (const auto& a : v){
            os <<setw(3) <<a <<" ";
        }
        os <<endl;
    }
    return os;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    for (auto& a : v){
        os <<a <<"\n";
    }
    os <<"\n";
    return os;
}

using hmap = std::vector<string>;

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

    hmap m;
    int dest_row = -1;
    int dest_col = -1;
    int src_row = -1;
    int src_col = -1;
    int rowc = 0;
    while (getline(in,line)){
        m.push_back(line);
        auto spos = line.find('S');
        if (spos != std::string::npos){
            src_row = rowc;
            src_col = spos;
        }
        auto dpos = line.find('E');
        if (dpos != std::string::npos){
            dest_row = rowc;
            dest_col = dpos;
        }
        ++rowc;
    }
    cout <<"Source found at: " <<src_row <<" " <<src_col <<endl;
    cout <<"Dest found at: " <<dest_row <<" " <<dest_col <<endl;

    std::vector<vector<int>> distmap;
    cout <<m <<endl;
    vector<int> v(m[0].size());
    std::fill(v.begin(), v.end(),999);
    for (int i = 0; i < m.size(); ++i){
        distmap.push_back(v);
    }
    int cur_dist = 0;
    distmap[dest_row][dest_col] = cur_dist++;
    m[dest_row][dest_col] = 'z' ;
    m[src_row][src_col] = 'a' ;

    int rcur = dest_row;
    int ccur = dest_col;
    std::deque<std::pair<int,int>> thelist;
    int iter = 0;
    while(!((rcur == src_row)&&(ccur == src_col))&&(iter++ < 10000)){
        for (auto ro = -1; ro < 2; ++ro){
            auto ridx = rcur + ro;
            if (ridx < 0) continue;
            if (ridx >= distmap.size()) continue;
            for (auto co = -1; co < 2; ++co){
                if (abs(ro) == abs(co)) continue;
                auto cidx = ccur + co;
                if (cidx < 0) continue;
                if (cidx >= distmap[0].size()) continue;

                cout <<ridx <<" " <<cidx << " " <<m[rcur][ccur] << m[ridx][cidx] << " " <<m[rcur][ccur] - m[ridx][cidx] <<"\n";
                if ((m[rcur][ccur] - m[ridx][cidx]) < 2){
                    if (distmap[ridx][cidx] == 999 ){
                        cout <<"\t emplace_back " <<ridx <<" " <<cidx <<"\n";
                        thelist.emplace_back(make_pair(ridx,cidx));
                    }
                    distmap[ridx][cidx] = std::min(distmap[ridx][cidx],distmap[rcur][ccur]+1);
                }
            }
        }
        if (thelist.size() == 0) break;
        cout <<"Moving from "<< rcur <<" " <<ccur <<" to ";
        rcur = thelist.front().first;
        ccur = thelist.front().second;
        cout << rcur <<" " <<ccur <<" currdist = " <<cur_dist <<"\n";
        ++cur_dist;
        thelist.pop_front();
    }

    cout <<distmap <<endl;
    cout <<"Min route: " << distmap[src_row][src_col]<<endl;
    //P2
    auto scenic = 99999;
    for (auto rowc = 0; rowc < m.size(); ++rowc){
        for(auto colc = 0; colc < m[0].size(); ++colc){
            if (m[rowc][colc] == 'a')
                scenic = std::min(distmap[rowc][colc],scenic);
        }
    }
    cout <<"scenic from a: " <<scenic <<endl;
    return 0;
}
