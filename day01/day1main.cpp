#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t value(0);
    vector<uint64_t> deer;
    uint64_t line_count = 0;
    while(getline(in,line)){
        if (line.empty()){
            deer.push_back(value);
            value=0;
            ++line_count;
            continue;
        }
        value += stoull(line);
        ++line_count;
    }
    //read input file. Now compute max sum:
    auto itr = max_element(deer.begin(), deer.end());
    int argmax = distance(deer.begin(), itr);
    
    cout <<"Max deer is " <<argmax <<" with calorie: " << deer[argmax] <<endl;

    auto to_sum = 3UL;
    auto top_sum = 0ULL;
    for (uint32_t i = 0; i < to_sum; ++i){
        top_sum += *itr;
        deer.erase(itr);
        itr = max_element(deer.begin(), deer.end());
    }
    cout <<"Top " << to_sum <<" deer are carrying " <<top_sum <<" calories."<<endl;
}
