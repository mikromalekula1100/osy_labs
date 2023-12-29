#include "../include/validate.hpp"
#include "../include/readDag.hpp"


std::vector<std::vector<int>> dag;

std::vector<std::vector<int>> adj;

void validate(){

    if((findWeaklyConnectedComponents(dag, adj) != 1) || (hasCycle(dag))){

        throw std::runtime_error("Invalid data");
    }
    cout<<"Valid data"<<endl;
    findStartEndNodes(dag);
}

int main(){


    readDag("config.yaml", dag);

    validate();
   
    
    
   
    


}

