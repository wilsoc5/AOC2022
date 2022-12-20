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

using namespace std;

using tree_t = uint16_t;
using row_t = vector<tree_t>;
using forest_t = vector<row_t>;

const char VISIBLE = 1;
const char HIDDEN = 0;

template<typename T>
ostream& operator<<(ostream& os, vector<T>& v){
    for (auto& a : v){
        os <<a <<" ";
    }
    os <<"\n";
    return os;
}

size_t process_forest(const forest_t& forest){
    size_t outside_visible = forest[0].size() + forest.back().size() + 2*(forest.size()-2);
    //Now walk down the main rows and find how many are visible:
    forest_t vmap = forest;
    for (auto& r : vmap){
        std::fill(r.begin(), r.end(), HIDDEN);
        r.front() = VISIBLE;
        r.back() = VISIBLE;
    }
    std::fill(vmap[0].begin(), vmap[0].end(), VISIBLE);
    std::fill(vmap.back().begin(), vmap.back().end(), VISIBLE);
    cout <<vmap <<endl;

    for (size_t rowc = 1; rowc < vmap.size(); ++rowc){
        auto colsize = vmap[rowc].size();
        auto lmax = forest[rowc][0];
        auto rmax = forest[rowc].back();
        cout <<"= rowc " <<rowc <<"============"<<endl;
        cout <<vmap <<endl;
        for (size_t colc = 1; colc < colsize; ++colc){
            size_t idx1 = colc;
            if (forest[rowc][idx1] > lmax){
                cout <<"forest[" <<rowc <<"]["<<idx1<<"] (" <<forest[rowc][idx1]<<") > " <<lmax<<endl;
                lmax = forest[rowc][idx1];
                vmap[rowc][idx1] = VISIBLE;
            }
            size_t ridx1 = colsize-colc;
            if (forest[rowc][ridx1] > rmax){
                cout <<"forest[" <<rowc <<"]["<<ridx1<<"] (" <<forest[rowc][ridx1]<<") > " <<rmax<<endl;
                rmax = forest[rowc][ridx1];
                vmap[rowc][ridx1] = VISIBLE;
            }
        }
    }
    cout <<"============="<<endl;
    cout <<vmap <<endl;
    auto rowsize = vmap.size();
    for (size_t colc = 1; colc < vmap[0].size(); ++colc){
        auto lmax = forest[0][colc];
        auto rmax = forest.back()[colc];
        for (size_t rowc = 1; rowc < vmap.size(); ++rowc){
            size_t idx1 = rowc;
            if (forest[idx1][colc] > lmax){
                lmax = forest[idx1][colc];
                vmap[idx1][colc] = VISIBLE;
            }
            size_t ridx1 = rowsize-rowc;
            if (forest[ridx1][colc] > rmax){
                rmax = forest[ridx1][colc];
                vmap[ridx1][colc] = VISIBLE;
            }
        }
    }
    cout <<"============="<<endl;
    cout <<vmap <<endl;

    auto visible = 0;
    for (auto& r : vmap){
        visible += std::accumulate(r.begin(), r.end(), 0 );
    }
        

    return visible; //inside_visible + outside_visible;
}
size_t process_forest_scenic(const forest_t& forest){
    size_t scenic = 0;
    cout <<"Scenic"<<endl;
    std::vector<vector<uint64_t>> vmap(forest.size());
    for (auto& r : vmap){
        r.resize(forest[0].size());
        std::fill(r.begin(), r.end(), 1);
        r.front() = 0;
        r.back() = 0;
    }
    std::fill(vmap[0].begin(), vmap[0].end(), 0);
    std::fill(vmap.back().begin(), vmap.back().end(), 0);
    cout <<" "<<vmap <<endl;

    for (size_t rowidx = 1; rowidx < vmap.size(); ++rowidx){
        const auto fstart = forest[rowidx].cbegin();
        const auto fend = forest[rowidx].cend();

        const auto rstart = forest[rowidx].crbegin();
        const auto rend = forest[rowidx].crend();

        cout <<"= rowidx " <<rowidx <<"============"<<endl;
        auto rowimax = forest[rowidx].size();
        for (size_t colidx = 1; colidx < vmap[0].size(); ++colidx){
            const auto value = forest[rowidx][colidx];
            auto lam = [value](const tree_t i){
                return (i >= value); 
            };
            //Right
            auto dleft = 0;
            for (int32_t colc = colidx-1; colidx >= 0; --colc){
                if (colc < 0) break;
                auto v = forest[rowidx][colc];
                if (v < value){ 
                    cout <<"[" <<rowidx <<"][" <<colidx <<"] = " <<value << "  > [" <<rowidx <<"][" <<colc <<"] = " <<v <<endl;
                    ++dleft;
                } else { 
                    ++dleft;
                    break;
                }
            }
            //auto current_start = fstart + colidx + 1;
            //auto forit = find_if(current_start, fend, lam);
            //auto dright = std::distance(current_start,forit) + (forit == fend ? 0 : 1);
            if (colidx != 0){
                dleft = dleft ? dleft : 1;
            }

            //Left
            auto dright = 0;
            for (int32_t colc = colidx+1; colidx < vmap[0].size(); ++colc){
                if (colc >= vmap[0].size()) break;
                auto v = forest[rowidx][colc];
                if (v < value){ ++dright;}
                else { ++dright; break; }
            }
            if (colidx != vmap[0].size()-1){
                dright = dright ? dright : 1;
                //++dright;
            } else {
                dright = 0;
            }
            //auto current_rstart = rstart + rowimax - colidx;
            //auto revit = find_if(current_rstart, rend, lam);
            //auto dleft = std::distance(current_rstart, revit) + (revit == rend ? 0 : 1);
            
            //UP
            auto dup = 0;
            for (int32_t rowc = rowidx-1; rowc >= 0; --rowc){
                auto v = forest[rowc][colidx];
                if (v < value){ ++dup;}
                else { ++dup; break; }
            }
            if (rowidx != 0){
                dup = dup ? dup : 1;
            }
            auto ddown = 0;
            for (int32_t rowc = rowidx+1; rowc < forest.size(); ++rowc){
                auto v = forest[rowc][colidx];
                if (v < value){ ++ddown;}
                else { ++ddown; break; }
            }
            if (rowidx != vmap.size() - 1){
                ddown = ddown ? ddown : 1;
            } else {
                ddown = 0;
            }


            cout <<"[" <<rowidx <<"][" <<colidx <<"] = " <<dleft << " " <<dup <<" " <<ddown <<" " <<dright <<endl;
            vmap[rowidx][colidx] *= dright * dleft * dup * ddown; 
            if (vmap[rowidx][colidx] > scenic) scenic = vmap[rowidx][colidx];
        }
    }
    cout <<" "<<vmap <<endl;

    return scenic;
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
    forest_t forest;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        forest.emplace_back(line.begin(), line.end());
        ++line_count;
    }
    //read input file. Now compute max sum:
    auto visible = process_forest(forest);
    cout <<"Visible: " << visible <<endl;

    auto scenic = process_forest_scenic(forest);
    cout <<"Scenic: " << scenic <<endl;

    return 0;
}
