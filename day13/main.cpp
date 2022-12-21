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

struct node_t {
    node_t() {}
    node_t(int v) : data(v) {}
    node_t(vector<node_t> v) : data(v) {}
    std::variant<vector<node_t>,int> data;
};

ostream& operator<<(ostream& os, const node_t& n);

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    os <<"[";
    for (auto& a : v){
        os <<a <<",";
    }
    os <<"]";
    return os;
}

ostream& operator<<(ostream& os, const node_t& n){
    if(const int* p = get_if<int>(&n.data)){
        os <<*p;
    } else {
        os <<get<vector<node_t>>(n.data);
    }
    return os;
}

istream& operator>>(istream& in, node_t& n){
    char brace;
    brace = in.peek();
    int breakcounter = 0;
    if (brace == '['){
        in >> brace; //eat the '['
        while((brace != ']')&&(++breakcounter < 100)){
            brace = in.peek();
            //cout <<"\t Peeked " <<brace<<endl;
            if (brace == '['){
                //cout <<"\t Found2 " <<brace <<"-Starting new node" <<breakcounter<<endl;
                node_t next;
                in >>next;
                get<0>(n.data).push_back(next); 
                in.get(brace);
            } else if (brace == ','){
                //cout <<"\t Found3 " <<brace <<" - eating it " <<breakcounter<<endl;
                in.get(brace);
            } else if (brace == ']'){
                //cout <<"\t Found4 " <<brace <<" - closing list " <<breakcounter<<endl;
                in.get(brace);
            } else {
                int v; 
                in >>v;
                //cout <<"Read int (wl) " <<v  <<" - "<<breakcounter<<endl;
                get<0>(n.data).push_back(v);
                in.get(brace);
            }
        }
    } else if (brace == ']'){
        //cout <<"\t Reading " <<brace<<endl;
        in.get(brace); //consume it.
    } else {
        int v;
        in >>v;
        //cout <<"Read int " <<v <<endl;
        n.data = v;
    }
    return in;
}

//Returns 0 for inconclusive;
//Returns 1 for correct order.
//Returns -1 for incorrect order;
int compare(const node_t& first,const node_t& second, int nest = 1){
    //for (int i = 0; i < nest; ++i) cout <<"\t";
    //cout <<" Compare "<<first <<" vs " <<second <<"\n";
    if(const int* fip = get_if<int>(&first.data)){
        //first is an Int.
        if (const int* sip = get_if<int>(&second.data)){
            //first is an int, second is an int. 
            if (*fip == *sip){
                //for (int i = 0; i < nest; ++i) cout <<"\t";
                return 0;
            }
            else if (*fip < *sip){
                //for (int i = 0; i < nest; ++i) cout <<"\t";
                //cout <<" Left is smaller. Correct Order\n";
                return 1;
            } else {
                //for (int i = 0; i < nest; ++i) cout <<"\t";
                //cout <<" Right is smaller. Incorrect Order\n";
                return -1;
            }
            //return 2*(*fip < *sip) - 1;
        } else {
            //First is an Int, second a list
            //Convert first to a list and compare.
            //for (int i = 0; i < nest; ++i) cout <<"\t";
            //cout <<" Mixed Types; convert left to [" <<*fip <<"]\n";
            node_t fl;
            get<0>(fl.data).push_back(*fip);
            return compare(fl,second, nest+1);
        }
    } else {
        //First is a list
        const vector<node_t>* fvp = get_if<vector<node_t>>(&first.data);
        if (const int* sip = get_if<int>(&second.data)){
            //first is a list, second is an int.
            //convert second to list and compare.
            //for (int i = 0; i < nest; ++i) cout <<"\t";
            //cout <<" Mixed Types; convert right to [" <<*sip <<"]\n";
            node_t sl;
            get<0>(sl.data).push_back(*sip);
            return compare(first,sl, nest+1);
        } else {
            //both lists.
            const vector<node_t>* svp = get_if<vector<node_t>>(&second.data);
            auto fstart = fvp->cbegin();
            auto fend = fvp->cend();
            auto sstart = svp->cbegin();
            auto send = svp->cend();
            while((fstart != fend) && (sstart != send)){
                auto res = compare(*fstart++,*sstart++, nest+1);
                if (res == 1)
                    return res;
                if (res == -1)
                    return res;
            }
            if ((fstart == fend)&&(sstart != send)) {
                //for (int i = 0; i < nest; ++i) cout <<"\t";
                //cout <<"Left ran out. Correct.\n";
                return 1;
            }
            if ((fstart != fend)&&(sstart == send)){
                //for (int i = 0; i < nest; ++i) cout <<"\t";
                //cout <<"Right ran out. Incorrect.\n";
                return -1;
            }
            return 0;
        }
    }
}

struct compare_t{
    bool operator()(const node_t& lhs, const node_t& rhs){
        return !(compare(lhs,rhs) > 0);
    }
};

template<typename Q>
void print_queue(Q q) {
    // NB: q is passed by value because there is no way to traverse
    // priority_queue's content without erasing the queue.
    for (; !q.empty(); q.pop())
        std::cout <<"\t" << q.top() << "\n";
    std::cout << '\n';
}
template <typename Q>
int get_index(Q q, node_t div2){
    int idx = 1;
    for (; !q.empty(); q.pop(), ++idx){
        if (compare(q.top(), div2) == 0)
            return idx;
    }
    return -1;
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

    node_t first, second;
    std::priority_queue<node_t, std::vector<node_t>, compare_t> pq;
    int counter = 0;

    int pair_index = 1;
    std::vector<int> valid_indexes;
    int pair_sum = 0;
    auto check = [&pair_index,&valid_indexes,&pair_sum](const node_t& first, const node_t& second){
        auto res = compare(first, second);
        cout <<"\tResult: ";
        if (res == -1) cout << " Incorrect order.\n";
        //if (res == 1){
        else {
            cout <<" Correct order.\n";
            valid_indexes.push_back(pair_index);
            pair_sum += pair_index;
        }
    };
    while (getline(in,line)){
        if (line.empty()){
            cout <<"Read " <<pair_index <<"\n\t" <<first <<"\n\t"<<second<<endl;
            check(first,second);
            pq.push(first);
            pq.push(second);
            ++pair_index;
            counter = 0;
            first = node_t();
            second = node_t();
            continue;
        }
        stringstream ss(line);
        ss >> ((counter++) ? second : first);
    }
    cout <<"Read " <<pair_index <<"\n\t" <<first <<"\n\t"<<second<<endl;
    check(first,second);
    pq.push(first);
    pq.push(second);
    cout <<"Valid Indexes: "<<valid_indexes <<endl;
    cout <<"Sum: "<<pair_sum <<endl;

    //Part 2 
    node_t div2; 
    get<0>(div2.data).emplace_back(std::vector<node_t>{2});
    pq.push(div2);
    node_t div6; 
    get<0>(div6.data).emplace_back(std::vector<node_t>{6});
    pq.push(div6);
    cout <<endl;
    print_queue(pq);
    auto idx2 = get_index(pq,div2);
    auto idx6 = get_index(pq,div6);
    cout <<"Index 2 : " <<idx2 <<endl;
    cout <<"Index 6 : " <<idx6 <<endl;
    cout <<"Decoder Key: " <<idx2 * idx6 <<endl;

    return 0;
}
