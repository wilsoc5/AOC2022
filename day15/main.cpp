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
#include <limits>
#include <set>

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

struct sensor_t {
    point_t p;
    int dist;
};

size_t dist(point_t p, point_t q){
    return abs(p.x - q.x) + abs(p.y - q.y);
}

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
bool operator==(const point_t& lhs, const point_t& rhs){
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

using beacon_t = point_t;

ostream& operator<<(ostream& os, const point_t& p){
    os <<"( " <<p.x <<" , " <<p.y << " )";
    return os;
}

ostream& operator<<(ostream& os, const sensor_t& l){
    os <<"[ " <<l.p<<" ]: " <<l.dist<< " ]";
    return os;
}

istream& operator>>(istream& in, point_t& p){
    char d;
    in >>p.x >>d >>p.y;
    return in;
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

    std::vector<sensor_t> sensors;
    std::vector<beacon_t> beacons;
    int minx = std::numeric_limits<int>::max();
    int miny = std::numeric_limits<int>::max();
    int maxx = std::numeric_limits<int>::min();
    int maxy = std::numeric_limits<int>::min();

    while (getline(in,line)){
        stringstream ss(line);
        string arrow;
        sensor_t s;
        beacon_t b;
        char c;
        int idx = 0;
        getline(ss, arrow, '='); //Sensor at x=
        ss >> s.p.x; //x=
        getline(ss, arrow, '='); //, y=
        ss >> s.p.y; //x=
        getline(ss, arrow, '='); //: closest beacon is at x=
        ss >> b.x;
        getline(ss, arrow, '='); //, y=
        ss >>b.y;

        s.dist = dist(s.p,b);
        
        minx = std::min(minx,s.p.x - s.dist);
        miny = std::min(miny,s.p.y - s.dist);
        maxx = std::max(maxx,s.p.x + s.dist);
        maxy = std::max(maxy,s.p.y + s.dist);

        minx = std::min(minx,b.x);
        miny = std::min(miny,b.y);
        maxx = std::max(maxx,b.x);
        maxy = std::max(maxy,b.y);
        sensors.push_back(s);
        beacons.push_back(b);
    }
    cout <<"Read sensors \n" <<sensors;
    cout <<"Read beacons \n" <<beacons;
    std::set<beacon_t> sb (beacons.begin(), beacons.end());
    //std::sort(beacons.begin(), beacons.end());
    //std:;unique(beacons.begin(), beacons.end());
    beacons = std::vector<beacon_t>(sb.begin(), sb.end());
    cout <<"Read beacons \n" <<beacons;
    cout <<"Bounding: [ "<<minx <<" , " <<miny <<" ] - [ " <<maxx <<" , " <<maxx <<" ]\n";

    int width = maxx - minx;
    cout <<"Width: " <<width <<endl;
    int row = 10;
    if (argc > 2)
        row = stoi(argv[2]);

    cout <<"Testing row: " <<row <<"\n";

    int fill_counter = 0;
    for (int x = minx - 2; x <= maxx + 2; x++){
        bool found = false;
        point_t p = {x, row};
        
        for (auto it = beacons.begin(); it != beacons.end(); ++it){
            if ((it->y == row) && (it->x == x)){
                found=true;
                //cout <<"B";
                break;
            }
        }
        if (!found) {
            for (auto it = sensors.begin(); it != sensors.end(); ++it){
                auto d = dist(p, it->p);
                if (d <= it->dist) {
                    //cout <<"x = " <<x <<" Sensor " <<*it <<endl;
                    //cout <<"#";
                    found = true;
                    ++fill_counter;
                    break;
                }
            }
        }
        if (!found){
            //cout <<"Not found in " <<x <<endl;
            //cout <<".";
        }
    }
    cout <<"\nFilled spots: " <<fill_counter<<endl;
    //for (auto it = beacons.begin(); it != beacons.end(); ++it){
        //if (it->y == row){
            //--fill_counter;
        //}
    //}
    cout <<"Beacons cannot be located in " <<fill_counter <<" spots" <<endl;
    int max_search_bound = 4000000;
    if (argc > 3){
        max_search_bound = stoi(argv[3]);
    }

    maxx = std::min(maxx, max_search_bound);
    maxy = std::min(maxy, max_search_bound);
    minx = std::max(minx, 0);
    miny = std::max(miny, 0);
    std::vector<point_t> bpos;
    for (int y = miny; y <= maxy; ++y){
        for (int x = minx; x <= maxx; ++x){
            bool found = false;
            point_t p {x,y};
            for (auto it = sensors.begin(); it != sensors.end(); ++it){
                auto d = dist(p, it->p);
                if (d <= it->dist) {
                    found = true;
                    x += (it->dist - d) /2;
                    //x += it->dist - abs(it->p.y - y;
                    break;
                }
            }
            if (!found){
                bpos.push_back(p);
            }
        }
    }
    cout <<"Possible positions: " <<bpos <<endl;
    for( auto b : bpos) {
        int64_t freq = ((int64_t) b.x) * 4000000 + (int64_t)b.y;
        cout <<b <<" = " <<freq <<endl;
    }

    return 0;
}
