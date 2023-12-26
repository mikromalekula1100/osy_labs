
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>
using std::endl;
using std::cout;
using std::cin;



int main(){
    std::map<int, std::string> kek = {{1, "sds"}, {21, "dsdsd"}};
    for(auto i : kek){
        cout<<i.second<<endl;
    }
}
