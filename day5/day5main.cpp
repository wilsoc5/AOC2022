#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>
#include <sstream>
#include <stack>

using namespace std;

struct crate_t{
    char name;
};

using cs_t = vector<crate_t>;
using dock_t = vector<cs_t>;

std::istream &operator >>(std::istream &is, crate_t &c) {
    char d;
    is >>c.name;
    return is;
}
std::ostream &operator <<(std::ostream &os, crate_t &c) {
    os <<"["<<c.name<<"] ";
    return os;
}

std::ostream &operator <<(std::ostream &os, cs_t &cs) {
    for_each(cs.begin(), cs.end(), [&os](crate_t& c){ 
        os <<"["<<c.name<<"] ";
    });
    return os;
}
std::ostream &operator <<(std::ostream &os, dock_t &dock) {
    for (int i = 0; i < dock.size(); ++i){
        os <<"Stack " <<i+1 <<" : " <<dock[i] <<"\n";
    }
    return os;
}

void process_stacks(dock_t& dock, vector<string>& start_stacks){
    auto stack_count = (start_stacks.back().size()+1)/4;
    cout <<"line_count: " <<start_stacks.back().size() <<" stack count" <<stack_count<<endl;
    dock.resize(stack_count);

    auto itend = start_stacks.rend();
    for(auto it = start_stacks.rbegin(); it != itend; ++it){
        for(int i = 0; i < stack_count; ++i){
            size_t idx = i*4 + 1;
            if (idx < it->size()){
                crate_t c{(*it)[i*4+1]};
                if (!(isspace(c.name))) dock[i].push_back(c);
            }
        }
    }
    cout <<dock;

}

void do_move(cs_t& src, cs_t& dst, size_t count){
    for(int i = 0; i < count; ++i){
        if(src.empty()) return;
        dst.push_back(src.back());
        src.pop_back();
    }
}
void process_move(dock_t& dock, string move_line){
    stringstream ss(move_line);
    uint32_t count, from, to;
    string smove, sfrom, sto;
    ss >>smove>> count >> sfrom >>from >> sto >> to; 
    do_move(dock[from-1 ],dock[to-1],count);
    cout <<dock;
}
void do_move_p2(cs_t& src, cs_t& dst, size_t count){
    cs_t buf;
    for(int i = 0; i < count; ++i){
        if(src.empty()) return;
        buf.push_back(src.back());
        src.pop_back();
    }
    for(int i = 0; i < count; ++i){
        if(buf.empty()) return;
        dst.push_back(buf.back());
        buf.pop_back();
    }
}
void process_move_p2(dock_t& dock, string move_line){
    stringstream ss(move_line);
    uint32_t count, from, to;
    string smove, sfrom, sto;
    ss >>smove>> count >> sfrom >>from >> sto >> to; 
    do_move_p2(dock[from-1 ],dock[to-1],count);
    cout <<dock;
}

void report_dock(dock_t& dock){
    for (int i = 0; i < dock.size(); ++i){
        cout <<dock[i].back().name <<"";
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
    uint64_t score = 0;
    uint64_t p2score = 0;

    dock_t dock, dock_p2;
    vector<string> start_stacks;

    bool stacking = true;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        cout <<line<<endl;;
        if (stacking){
            if (line[1] != '1'){
                start_stacks.push_back(line);
            } else {
                stacking = false;
                //end of stacks. Process stacks.
                cout <<"processing stacks"<<endl;
                process_stacks(dock, start_stacks);
                dock_p2 = dock;
            }
        } else {
          if (line[0] == 'm'){
            process_move(dock,line);
            process_move_p2(dock_p2,line);
          }
        }
        ++line_count;
    }
    //read input file. Now compute max sum:

    cout <<"P1: "<<endl;
    report_dock(dock);
    cout <<"\nP2: " <<endl;
    report_dock(dock_p2);
    //cout <<"Contained score is " <<score <<endl;
    //cout <<"Overlap score is " <<p2score <<endl;

    return 0;
}
