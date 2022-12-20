#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>
#include <sstream>

using namespace std;

struct file_t{
    string name;
    uint64_t size;
};
struct directory_t{
    string name;
    directory_t* parent = nullptr;
    vector<directory_t> subdirs;
    vector<file_t> files;
    uint64_t size;
};

void print_file(const file_t& f, int level = 1){
    for (int i = 0; i < level; ++i)
        cout <<"  ";
    cout <<"- " <<f.name <<"(file, size = " <<f.size <<" )\n";
}

void print_directory(const directory_t& d, int level = 0){
    for (int i = 0; i < level; ++i)
        cout <<"  ";
    cout <<"- " <<d.name <<"(dir, size = " <<d.size <<" )\n";
    for (auto dit = d.subdirs.cbegin(); dit != d.subdirs.cend(); ++dit){
        print_directory(*dit, level + 1);
    }
    for (auto fit = d.files.cbegin(); fit != d.files.cend(); ++fit){
        print_file(*fit,level + 1);
    }
}

size_t propagate_dir_sizes(directory_t& d){
    for (auto dit = d.subdirs.begin(); dit != d.subdirs.end(); ++dit){
        d.size += propagate_dir_sizes(*dit);
    }
    return d.size;
}
template<typename Func>
std::vector<const directory_t*> find_size(const directory_t& d,const size_t max_size, Func op ){
    std::vector<const directory_t*> weak_ref;
    if (op(d.size, max_size)){
        weak_ref.push_back(&d);
    }
    for (auto dit = d.subdirs.begin(); dit != d.subdirs.end(); ++dit){
        auto res = find_size(*dit, max_size, op);
        std::copy(res.begin(), res.end(), std::back_inserter(weak_ref));
    }
    return weak_ref;
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
    directory_t root {"/",nullptr,{}, {},0};
    directory_t* cwd = &root;

    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        if (line[0] == '$'){
            if (line [2] == 'c'){
                string name = line.substr(5);
                if (name == ".."){
                    if (cwd != &root){
                        cwd = cwd->parent;
                    }
                } else {
                    if (name != "/"){
                    //need to add a check if cd into existing dir.
                        auto endit = end(cwd->subdirs);
                        auto it = find_if(begin(cwd->subdirs),endit,
                            [name](const directory_t& d){
                                return name == d.name;
                            });
                        if( it != endit){
                            //found the subdirectory. Should CD into it.
                            cwd = &(*it);
                        } else {
                            cwd->subdirs.emplace_back(directory_t{name, cwd, {}, {},0});
                            cwd = &(cwd->subdirs.back());
                        }
                    }
                }
                cout <<"CWD Directory Name: " <<cwd->name <<endl;
            }
            else { //ls command.
            }
        } else {
            if (line[0] == 'd') { //dir:
                //do nothing.
            }
            else {
                file_t file;
                stringstream ss(line);
                ss >>file.size >>file.name;
                cwd->files.push_back(file);
                cwd->size += file.size;
            }
        }
        ++line_count;
    }
    //read input file. Now compute max sum:
    auto root_size = propagate_dir_sizes(root);
    
    print_directory(root);
    auto results = find_size(root, 100000, std::less<size_t>{});
    score = 0;
    for (auto it = results.begin(); it != results.end(); ++it){
        cout <<"Found directory: " <<(*it)->name <<" " <<(*it)->size<<endl;
        score += (*it)->size;
    }
    cout <<"Max score is " <<score <<endl;

    int64_t total_space = 70000000;
    int64_t current_space = root.size;
    int64_t required_space = 30000000;
    auto free_up = required_space - (total_space - current_space);

    cout <<"Root is curently: " << current_space<<endl;
    cout <<"Need to open up   " << required_space<<endl;
    cout <<"Need to free up   "<<free_up<<endl;
    
    auto resultsp2 = find_size(root,free_up , std::greater<int64_t>{});
    uint64_t min_size = total_space;
    for (auto it = resultsp2.begin(); it != resultsp2.end(); ++it){
        cout <<"Found directory: " <<(*it)->name <<" " <<(*it)->size<<endl;
        min_size = std::min(min_size,(*it)->size);
    }
    
    cout <<"Min size is " <<min_size <<endl;

    return 0;
}
