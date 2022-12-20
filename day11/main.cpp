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

template<typename T, typename U>
ostream& operator<<(ostream& os, const std::pair<T,U>& p){
    os <<"{ " <<p.first <<" , "<<p.second <<" }";
    return os;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    for (auto& a : v){
        os <<a <<" ";
    }
    os <<"\n";
    return os;
}

template <typename M, typename ...Rest>
constexpr auto lcm(const M& first, const Rest&... rest) {
    return std::lcm(first, lcm(rest...));
}

struct item_t{
    item_t (uint64_t wl) : _wl(wl) {};
    item_t () : item_t(0) {};
    void apply_operation(){
        _wl = _wl / 3;
    }
    int64_t _wl;
};

item_t operator+(item_t i, item_t n){
    return item_t(i._wl + n._wl);
}
item_t operator+(item_t i, int n){
    return item_t(i._wl + n);
}
item_t operator*(item_t i, int n){
    return item_t(i._wl * n);
}
item_t operator/(item_t i, int n){
    return item_t(i._wl / n);
}
item_t operator*(item_t i, item_t n){
    return item_t(i._wl * n._wl);
}
item_t operator/(item_t i, item_t n){
    return item_t(i._wl / n._wl);
}
struct monkey_t{
    int32_t id = -1;
    std::vector <item_t> items;
    std::function<item_t(const item_t)> op;
    int32_t test_value = -1;
    int32_t throw_id_true = -1;
    int32_t throw_id_false = -1;
    uint64_t inspect_counter = 0;
};

bool perform_test(const monkey_t& m, const item_t& i){
    return !(i._wl % m.test_value);
}


istream& operator>>(istream& os, monkey_t& m){
    std::string line;
    m.items.clear();
    string word;
    char comma;
    auto monkey_counter = 6;
    while(getline(os,line)){
        stringstream ss(line);
        if (line[0] == 'M'){
            ss >>word;
            ss >>word;
            m.id = stoi(word);
            --monkey_counter;
        }
        if (line[2] == 'S'){
            ss >>word; //Starting
            ss >>word; //items: 
            ss >>word; //start of list
            while (ss){
                m.items.push_back(stoi(word));
                ss >>word; //start of list
            }
            --monkey_counter;
        }
        else if (line[2] == 'O'){
            string op;
            ss >>word; //Operation:
            ss >>word; //new;
            ss >>word; //=
            ss >>word; //old
            ss >>op; //operator
            ss >>word; //operand;
            if (op[0] == '+'){
                if (word == "old"){
                    m.op = [](item_t i){ return i + i; };

                } else {
                    auto sf = stoi(word);
                    m.op = [sf](item_t i){ return i + sf; };
                }
            } else if (op[0] == '*'){
                if (word == "old"){
                    m.op = [](item_t i){ return i * i; };

                } else {
                    auto sf = stoi(word);
                    m.op = [sf](item_t i){ return i * sf; };
                }
            }
            --monkey_counter;
        }
        else if (line[2] == 'T'){
            ss >>word; //Test:
            ss >>word; //divisible
            ss >>word; //by
            ss >>word; //<value>
            m.test_value = stoi(word);
            --monkey_counter;
        }
        else if (line[4] == 'I') {
            ss >>word; //If
            ss >>word; //true;
            ss >>word; //throw
            ss >>word; //to
            ss >>word; //monkey
            ss >>word; //<value>
            if (line[7] == 't'){
                m.throw_id_true = stoi(word);
            } else {
                m.throw_id_false = stoi(word);
            }
            --monkey_counter;
        }
        if (monkey_counter == 0) break;
    }
    return os;
}
ostream& operator<<(ostream& os, const monkey_t& m){
    os <<"Monkey " <<m.id <<endl;
    os <<"\t Starting Items: "<< m.items <<endl;
    os <<"\t Test: Divide by: " <<m.test_value <<endl;
    os <<"\t Throw to Monkey: True:  " <<m.throw_id_true <<endl;
    os <<"\t                : False: " <<m.throw_id_false <<endl;
    os <<"\t Inspect Counter: " <<m.inspect_counter <<endl;
    return os;
}
ostream& operator<<(ostream& os, const item_t& i){
    os <<i._wl; 
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
    uint64_t visited = 0;
    uint64_t p2score = 0;

    uint64_t cycle_counter = 1;
    int64_t reg = 1;

    vector<std::pair<uint64_t,int64_t>> history;

    std::vector<monkey_t> monkeys;
    while (in){
        monkey_t m;
        in >>m;
        if (m.id >= 0){
            cout <<m <<endl;
            monkeys.emplace_back(m);
        }
    }

    string rounds ="20";
    if (argc > 2){
        rounds = argv[2];
    }
    uint64_t roundp = stoi(rounds);
     
    cout <<monkeys <<endl;

    size_t lcm_m = monkeys[0].test_value;
    for (const auto&m : monkeys){
       lcm_m = std::lcm(lcm_m,m.test_value);
    }
    cout <<"LCM: " <<lcm_m <<endl;

    for(size_t ri = 0; ri < roundp; ++ri ){
        cout <<" === Round " <<ri <<" === " <<endl;
        for(auto& m : monkeys){
            auto it = m.items.begin();
            while (it != m.items.end()){
                ++m.inspect_counter;
                *it = m.op(*it);
                //it->apply_operation(); //p1
                it->_wl = it->_wl % lcm_m; //P2
                auto id = perform_test(m,*it) ? m.throw_id_true : m.throw_id_false;
                monkeys[id].items.push_back(*it);
                it = m.items.erase(it);
            }
        }
        //cout <<monkeys <<endl;
    }
    
    std::priority_queue<int64_t> pq;

    for (const auto& m : monkeys){
        pq.push(m.inspect_counter);
    }

    auto monkey_business = pq.top();
    pq.pop();
    monkey_business *= pq.top();

    cout <<"Monkey Business: "<<monkey_business <<endl;

    //cout <<"============================="<<endl;
    //for (auto s : sh){
    //    cout <<s <<endl;
    //}

    return 0;
}
