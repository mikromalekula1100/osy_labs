#include "../include/split_string.hpp"

std::vector<std::string> split_string(std::string input){
    
    std::istringstream iss(input);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word) {

            words.push_back(word);
        }

    return std::move(words);
}