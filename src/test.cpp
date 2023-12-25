
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
using std::endl;
using std::cout;
using std::cin;



int main(){
    std::string input;
    std::getline(std::cin, input);

    std::istringstream iss(input);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    

}