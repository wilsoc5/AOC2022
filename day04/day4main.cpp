#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>
#include <sstream>

using namespace std;

struct elf_t{
    int start;
    int stop;
};

bool contained_in(elf_t lhs, elf_t rhs, bool cont=true){
    if (lhs.start <= rhs.start){
        if (rhs.stop <= lhs.stop){
            return true;
        }
        if (cont) return contained_in(rhs,lhs, false);
    } else {
        if (cont) return contained_in(rhs,lhs, false);
    }
    cout <<"Unknown"<<endl;;
    return false;
}

bool overlap(elf_t lhs, elf_t rhs){
    if (lhs.start > rhs.start) {
        return overlap(rhs,lhs);
    }
    if (lhs.stop >= rhs.start) return true;
    return false;
}

std::istream &operator >>(std::istream &is, elf_t &e) {
    char d;
    is >>e.start>>d>>e.stop;
    return is;
}
std::ostream &operator <<(std::ostream &os, elf_t &e) {
    os <<e.start<<" "<<e.stop;
    return os;
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

    char d;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        stringstream ss(line);
        elf_t e1, e2;
        ss >>e1 >>d >>e2;
        bool iscon = contained_in(e1,e2);
        bool isov = overlap(e1,e2);
        cout <<e1 <<" , " <<e2 << " " <<iscon<<endl;
        score += iscon;
        p2score += isov;
        
        ++line_count;
    }
    //read input file. Now compute max sum:

    cout <<"Contained score is " <<score <<endl;
    cout <<"Overlap score is " <<p2score <<endl;

    return 0;
}
