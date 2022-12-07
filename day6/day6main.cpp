#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>
#include <sstream>
#include <deque>

using namespace std;

using ring_t = deque<char>;

size_t process_line(string line, size_t ring_size){
    size_t first = line.size();
    ring_t r; 
    auto it = find_if(line.begin(), line.end(),[&r, ring_size](char c){
        r.push_back(c);
        if (r.size() > ring_size){
            r.pop_front();
        }
        if (r.size() < ring_size) return false;
        ring_t rs = r;
        std::sort(rs.begin(), rs.end());
        auto it = std::adjacent_find(rs.begin(), rs.end());
        if (it == rs.end()) return true;
        return false;
    });
    auto d = distance(line.begin(), it);
    return d + 1;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t line_count = 0;
    uint64_t score = 0;
    uint64_t p2score = 0;

    bool stacking = true;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        score = process_line(line, 4);
        cout <<"score is " <<score <<endl;

        p2score = process_line(line, 14);
        cout <<"P2 score is " <<p2score <<endl;
        ++line_count;
    }
    //read input file. Now compute max sum:

    cout <<"\nP2: " <<endl;

    return 0;
}
