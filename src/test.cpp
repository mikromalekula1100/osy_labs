
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
    const int port = 2222;
    
    std::string s = "tcp://127.0.0.1:"+std::to_string(port);
    cout<<s<<endl;
}
