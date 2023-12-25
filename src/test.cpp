
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using std::endl;
using std::cout;
using std::cin;



int main(){
    std::string command;
    cin>>command;
    std::vector<std::string> enumCommand;
    std::vector<int> count;
    count.push_back(0);
    cout<<command.size()<<endl;
    for(size_t i = 0; i < command.size(); ++i){
        if(command[i] == 'a'){
            cout<<1<<endl;
        }
        
    }
    cout<<enumCommand.size()<<endl;
}