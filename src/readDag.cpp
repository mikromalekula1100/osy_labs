
#include "../include/readDag.hpp"

void readDag(std::string fileName, std::vector<std::vector<int>>& dag){

    YAML::Node config = YAML::LoadFile("../" + fileName);

    const auto& values = config["nodes"];

    for(const auto& i : values){

       int idNode = i["id"].as<int>() - 1;

       std::vector<int> vectorDepends = i["depends_on"].as<std::vector<int>>(); 

       std::vector<int> list;

       for(int i : vectorDepends){

           list.push_back(i - 1);
       }

       dag.push_back(list);
    }

}
