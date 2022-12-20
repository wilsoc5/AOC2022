#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

const uint64_t MAX_COUNT {52};
using sack_t = std::map<int,int>;

template <typename KeyType, typename LeftValue, typename RightValue>
std::map<KeyType, std::pair<LeftValue, RightValue>>
IntersectMapsBoth(const std::map<KeyType, LeftValue>& left,
              const std::map<KeyType, RightValue>& right) {
    std::map<KeyType, std::pair<LeftValue, RightValue>> result;
    typename std::map<KeyType, LeftValue>::const_iterator il  = left.begin();
    typename std::map<KeyType, RightValue>::const_iterator ir = right.begin();
    while (il != left.end() && ir != right.end()) {
        if (il->first < ir->first)
            ++il;
        else if (ir->first < il->first)
            ++ir;
        else {
            result.insert(std::make_pair(il->first, std::make_pair(il->second, ir->second)));
            ++il;
            ++ir;
        }
    }
    return result;
}
template <typename KeyType, typename LeftValue, typename RightValue>
std::map<KeyType, LeftValue>
IntersectMaps(const std::map<KeyType, LeftValue>& left,
              const std::map<KeyType, RightValue>& right) {
    std::map<KeyType, LeftValue> result;
    typename std::map<KeyType, LeftValue>::const_iterator il  = left.begin();
    typename std::map<KeyType, RightValue>::const_iterator ir = right.begin();
    while (il != left.end() && ir != right.end()) {
        if (il->first < ir->first)
            ++il;
        else if (ir->first < il->first)
            ++ir;
        else {
            result.insert(std::make_pair(il->first, il->second));
            ++il;
            ++ir;
        }
    }
    return result;
}

void fill_sack(string sackstr, sack_t& sack){
    for (auto it = sackstr.begin(); 
         it != sackstr.end(); ++it){
         size_t idx = 0;
         uint8_t start = 'a';
         uint8_t offset = 0;
         if(isupper(*it)){
             start = 'A';
             offset = 26;
         }
         idx = *it - start + offset;
         sack[idx] = 1;
    }
    return;
}

std::ostream &operator <<(std::ostream &os, const sack_t::value_type &v) {
    os <<v.first <<" " <<v.second;
    return os;
}

std::ostream &operator <<(std::ostream &os, sack_t &v) {
    using namespace std;
    for_each(v.cbegin(), v.cend(),
        [&os](const auto& val){
            os <<val.first <<" " <<val.second <<"\n\t";
        });
    return os;
}

std::ostream &operator <<(std::ostream &os, std::map<int, std::pair<int,int> > &v) {
    using namespace std;
    for_each(v.cbegin(), v.cend(),
        [&os](const auto& val){
            os <<val.first <<" " <<val.second.first <<" " <<val.second.second <<"\n\t";
        });
    return os;
}

uint64_t find_sacks(string line){
    sack_t sack1 ;//(MAX_COUNT, (uint8_t) 0);
    sack_t sack2 ;//(MAX_COUNT, (uint8_t) 0);
    auto len = line.length();
    auto sack1string = line.substr(0,len/2);
    auto sack2string = line.substr(len/2);
    fill_sack(sack1string, sack1);
    fill_sack(sack2string, sack2);
    auto both = IntersectMaps(sack1, sack2);
    return both.begin()->first + 1;
}

uint64_t find_badges(string line, std::vector<sack_t>& group){
    sack_t sack;
    fill_sack(line,sack);
    group.push_back(sack);
    if (group.size() == 3){
        auto two = IntersectMaps(group[0],group[1]);
        auto three = IntersectMaps(two,group[2]);
        group.clear();
        return three.begin()->first + 1;
    }
    return 0;
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
    std::vector<sack_t> group;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        score += find_sacks(line);
        p2score += find_badges(line, group);
        ++line_count;
    }
    //read input file. Now compute max sum:
    
    cout <<"Max score is " <<score <<endl;
    cout <<"Max badge score is " <<p2score <<endl;

    return 0;
}
