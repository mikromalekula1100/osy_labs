#include "../include/validate.hpp"

std::vector<std::vector<int>> dag;

//нахождение компонент связности
std::vector<std::vector<int>> adj;

bool dfsC(int vertex, std::vector<int>& visited){

    visited[vertex] = 1;

    for(int to : dag[vertex]){

        if(visited[to] == 0){

            if(dfsC(to, visited)){

                return true;
            }
        } else if(visited[to] == 1){

            return true;
        }   
    }

    visited[vertex] = 2;

    return false;
}

bool hasCycle(){

    std::vector<int> visited(dag.size(), 0);

    for(int vertex = 0; vertex < dag.size(); ++vertex){

        if(!visited[vertex]){

            if(dfsC(vertex, visited)){

                return true;
            }

        }
    }

    return false;
}

//--------------------

//нахождение компонент связности


void dfs(int v, std::vector<int>& visited) {
    visited[v] = true;
    for (auto u : adj[v])
        if (!visited[u])
            dfs(u, visited);
}

int findWeaklyConnectedComponents() {

    std::vector<int> visited(dag.size(), 0);

    int n = dag.size();
    adj.resize(n);
    visited.assign(n, false);

    for (int i = 0; i < n; ++i)
        for (auto u : dag[i]) {
            adj[i].push_back(u);
            adj[u].push_back(i);
        }

    int components = 0;
    for (int i = 0; i < n; ++i)
        if (!visited[i]) {
            dfs(i, visited);
            ++components;
        }

    return components;
}

void readDag(std::string fileName){

    YAML::Node config = YAML::LoadFile(fileName);

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

void findStartEndNodes() {

    int n = dag.size();
    std::vector<int> in(n, 0), out(n, 0);

    for (int i = 0; i < n; ++i)
        for (auto u : dag[i]) {
            ++out[i];
            ++in[u];
        }

    for (int i = 0; i < n; ++i) {
        if (in[i] == 0)
            std::cout << "Start node: " << i + 1 << std::endl;
        if (out[i] == 0)
            std::cout << "End node: " << i + 1 << std::endl;
    }
}