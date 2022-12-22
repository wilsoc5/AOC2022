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
#include <variant>

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
    //os <<"[";
    for (auto& a : v){
        os <<a <<"\n";
    }
    //os <<"]";
    return os;
}

struct point_t {
    point_t() : x(0), y(0) {}
    point_t(int a, int b) : x(a), y(b) {}
    int x, y;
};

struct line_segment_t {
    point_t e1, e2;
};

point_t operator+(const point_t& lhs, const point_t& rhs){
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

bool operator<(const point_t& lhs, const point_t& rhs){
    if (lhs.x == rhs.x){
        return lhs.y < rhs.y;
    } else {
        return lhs.x < rhs.x;
    }
}

using lines_t = std::vector<line_segment_t>;

using sand_t = point_t;

ostream& operator<<(ostream& os, const point_t& p){
    os <<"( " <<p.x <<" , " <<p.y << " )";
    return os;
}

ostream& operator<<(ostream& os, const line_segment_t& l){
    os <<"[ " <<l.e1<<" , " <<l.e2<< " ]";
    return os;
}

istream& operator>>(istream& in, point_t& p){
    char d;
    in >>p.x >>d >>p.y;
    return in;
}

//Returns true if point is in a line. 
bool check_lines(lines_t& l, point_t p){

    return false;
}

sand_t drop_sand(const vector<point_t> blocks, const vector<sand_t>& stationary_sand, int maxy){
    //Sand starts at 500,0
    sand_t s {500,0};
    int loopc = 0;
    auto check = [&](point_t p) -> bool {
        if (std::binary_search(blocks.begin(), blocks.end(), p)) {
            return false;
        }
        if (std::binary_search(stationary_sand.begin(), stationary_sand.end(), p)){
            return false;
        }
        return true;
    };
    bool falling = true;
    do{
        if (check(s + sand_t{0,1})){
            s.y += 1;    
        } else if (check (s + sand_t{-1,1})){
            s.x -= 1;
            s.y += 1;
        } else if (check (s + sand_t{1,1})){
            s.x += 1;
            s.y += 1;
        } else {
            falling = false;
        }
    } while (falling && (s.y < maxy) && (loopc < 10000));
    return s;
}
std::vector<point_t> fill_blocks(const std::vector<line_segment_t>& ls){
    std::vector<point_t> b;
    for (auto& lsi : ls){
        const point_t& s = lsi.e1;
        const point_t& e = lsi.e2;
        if (s.x == e.x){
            int sy = s.y;
            int ey = e.y;
            if (ey < sy){
                std::swap(ey,sy);
            }
            for (int i = sy; i <= ey; ++i){
                b.emplace_back(s.x,i);
            }
        } else if (s.y == e.y){
            int sx = s.x;
            int ex = e.x;
            if (ex < sx){
                std::swap(ex,sx);
            }
            for (int i = sx; i <= ex; ++i){
                b.emplace_back(i, s.y);
            }
        }
        else { 
            cerr <<"fill_blocks line unsupported: " <<lsi <<endl;
        }

    }
    return b;
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

    point_t sand = {500,0};
    uint64_t sand_counter = 1;

    int counter = 0;

    std::vector<line_segment_t> ls;
    std::vector<sand_t> stationary_sand;
    int minx = 1000000;
    int miny = 1000000;
    int maxx = 0;
    int maxy = 0;

    while (getline(in,line)){
        stringstream ss(line);
        string arrow;
        point_t p, pold;
        std::vector<point_t> v;
        int idx = 0;
        while(ss){
            ss >>p;
            ss >>arrow;
            if (idx){
               ls.push_back({pold,p}); 
            }
            minx = std::min(minx,p.x);
            miny = std::min(miny,p.y);
            maxx = std::max(maxx,p.x);
            maxy = std::max(maxy,p.y);
            ++idx;
            pold = p;
        }
    }
    cout <<"Read Lines \n" <<ls;
    std::vector<point_t> blocks = fill_blocks(ls) ;
    std::sort(blocks.begin(), blocks.end());

    point_t pmin {minx,miny};
    point_t pmax {maxx, maxy};
    cout <<"Bounds: "  <<pmin <<" ==> " <<pmax <<endl;
    
    cout <<"Total of " << blocks.size() <<" blocks filled."<<endl;
    int loop_counter = 0;
    sand_t cur_sand;
    do {
       cur_sand = drop_sand(blocks, stationary_sand, maxy + 2);
       if (cur_sand.y < maxy){
           stationary_sand.push_back(cur_sand);
           std::sort(stationary_sand.begin(), stationary_sand.end());
       }
    } while(( cur_sand.y < maxy+2) && (++loop_counter < 1000000));


    cout <<"Total of " <<stationary_sand.size() <<" units of sand." <<endl;
    cout <<"Loop counter: " <<loop_counter <<endl;

    //p2
    line_segment_t floor = { point_t{minx-minx,maxy+2}, point_t{2*maxx,maxy+2} };
    vector<line_segment_t> fv;
    fv.push_back(floor);
    auto b2 = fill_blocks(fv);
    b2.insert(b2.end(),blocks.begin(), blocks.end());
    std::sort(b2.begin(), b2.end());
    std::vector<sand_t> s2 = stationary_sand;
    loop_counter = 0;
    do {
       cur_sand = drop_sand(b2, s2, maxy + 2);
       s2.push_back(cur_sand);
       std::sort(s2.begin(), s2.end());
    } while((!((cur_sand.x == 500) && ( cur_sand.y == 0))) && (++loop_counter < 1000000));
    cout <<"cur_sand: " <<cur_sand <<endl;
    cout <<"Total of " <<s2.size() <<" units of sand on floor." <<endl;
    cout <<"Loop counter: " <<loop_counter <<endl;


    return 0;
}
