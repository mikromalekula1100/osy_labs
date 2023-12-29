#pragma once

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <set>

using std::cout;
using std::endl;

bool dfsC(int, std::vector<int>&, std::vector<std::vector<int>>&);

bool hasCycle(std::vector<std::vector<int>>&);


void dfs(int, std::vector<int>&, std::vector<std::vector<int>>& );

int findWeaklyConnectedComponents(std::vector<std::vector<int>>&, std::vector<std::vector<int>>& );

//--------------------



void findStartEndNodes(std::vector<std::vector<int>>&);