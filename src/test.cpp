
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <set>
#include "zmq.hpp"
using std::endl;
using std::cout;
using std::cin;



int main(){
    std::set<int> a = {1,2,3};
    for(int i : a)
        cout<<i<<endl;

}
