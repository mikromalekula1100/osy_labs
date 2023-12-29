#include "../include/validate.hpp"
#include "../include/readDag.hpp"


std::vector<std::vector<int>> dag;

std::vector<std::vector<int>> adj;

// int maxCountJob = 4;

std::mutex mtx; 

using namespace std;

void validate(){

    if((findWeaklyConnectedComponents(dag, adj) != 1) || (hasCycle(dag))){

        throw std::runtime_error("Invalid data");
    }

    cout<<"Valid data"<<endl;
}

void topologicalSortUtil(int v, vector<vector<int>>& dag, vector<bool>& visited, queue<int>& queue) {

    visited[v] = true;

    for (int i : dag[v]) {

        if (!visited[i]) {

            topologicalSortUtil(i, dag, visited, queue);
        }
    }

    queue.push(v);
}

queue<int> createPriorityQueue(vector<vector<int>>& dag, vector<int>& endNodes) {

    int numNodes = dag.size();

    vector<bool> visited(numNodes, false);

    queue<int> priorityQueue;

    queue<int> resultQueue;

    for (int i = 0; i < numNodes; ++i) {

        if (!visited[i]) {

            topologicalSortUtil(i, dag, visited, resultQueue);
        }
    }

    vector<int> distance(numNodes, -1);

    while (!resultQueue.empty()) {

        int node = resultQueue.front();

        resultQueue.pop();

        if (node == endNodes[0]) {

            distance[node] = 0;
        }

        for (int i : dag[node]) {

            if (distance[i] == -1) {

                distance[i] = distance[node] + 1;
            }
        }
    }

    vector<pair<int, int>> priorityQueueVec;

    for (int i = 0; i < numNodes; ++i) {

        priorityQueueVec.push_back({distance[i], i});
    }

    sort(priorityQueueVec.begin(), priorityQueueVec.end());

    for (auto& pair : priorityQueueVec) {

        priorityQueue.push(pair.second);
    }

    return priorityQueue;
}

vector<vector<int>> buildInverseAdjacencyList(const vector<vector<int>>& originalAdjList) {
    int numNodes = originalAdjList.size();
    vector<vector<int>> inverseAdjList(numNodes);

    for (int i = 0; i < numNodes; ++i) {
        for (int neighbor : originalAdjList[i]) {
            
            inverseAdjList[neighbor].push_back(i);
        }
    }

    return inverseAdjList;
}

set<int> completedThreads;

void functionThread(int job_id){

    sleep(2);   

    std::lock_guard<std::mutex> lock(mtx);

    cout<<"я поток номер "<< job_id + 1 <<", я выполнил свою работу"<<endl;
    
    completedThreads.insert(job_id);
}

int main() {

    readDag("config.yaml", dag);

    validate();

    vector<vector<int>> inverseDag = buildInverseAdjacencyList(dag);

    vector<int> endNodes = findEndNodes(dag);

    queue<int> priorityQueue = createPriorityQueue(dag, endNodes);

    std::vector<std::thread> threads;

    for(int i = 0; i < dag.size(); ++i){

        int job_id = priorityQueue.front();

        priorityQueue.pop();

        while(true){

            int count = 0;

            for(int i : inverseDag[job_id]){

                if(completedThreads.count(i) == 1){

                    ++count;
                }

            }

            if(count == (inverseDag[job_id]).size()){

                break;
            }
        }

        threads.emplace_back(functionThread, job_id);

    }

    
    for (auto& t : threads) {

        t.join();
    }

    return 0;
}


