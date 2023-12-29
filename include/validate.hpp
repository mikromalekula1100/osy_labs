#pragma once

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>

using std::cout;
using std::endl;

bool dfsC(int, std::vector<int>&, std::vector<std::vector<int>>&);

bool hasCycle(std::vector<std::vector<int>>&);

void dfs(int, std::vector<int>&, std::vector<std::vector<int>>& );

int findWeaklyConnectedComponents(std::vector<std::vector<int>>&, std::vector<std::vector<int>>& );

std::vector<int> findStartNodes(std::vector<std::vector<int>>&);

std::vector<int> findEndNodes(std::vector<std::vector<int>>&);