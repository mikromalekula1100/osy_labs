#include "../include/validate.hpp"



int main(){


    readDag("config.yaml");

    int components = findWeaklyConnectedComponents();
    cout << components << endl;

    findStartEndNodes();
   
    // cout<<hasCycle()<<endl;


}

