#include "../include/validate.hpp"



bool dfsC(int vertex, std::vector<int>& visited, std::vector<std::vector<int>>& dag){

    visited[vertex] = 1;

    for(int to : dag[vertex]){

        if(visited[to] == 0){

            if(dfsC(to, visited, dag)){

                return true;
            }
        } else if(visited[to] == 1){

            return true;
        }   
    }

    visited[vertex] = 2;

    return false;
}

bool hasCycle(std::vector<std::vector<int>>& dag){

    std::vector<int> visited(dag.size(), 0);

    for(int vertex = 0; vertex < dag.size(); ++vertex){

        if(!visited[vertex]){

            if(dfsC(vertex, visited, dag)){

                return true;
            }

        }
    }

    return false;
}




void dfs(int v, std::vector<int>& visited, std::vector<std::vector<int>>& adj) {
    visited[v] = true;
    for (auto u : adj[v])
        if (!visited[u])
            dfs(u, visited, adj);
}

int findWeaklyConnectedComponents(std::vector<std::vector<int>>& dag, std::vector<std::vector<int>>& adj) {

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
            dfs(i, visited, adj);
            ++components;
        }

    return components;
}



void findStartEndNodes(std::vector<std::vector<int>>& dag) {

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