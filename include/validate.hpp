#pragma once

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <set>

using std::cout;
using std::endl;

bool dfsC(int, std::vector<int>&);

bool hasCycle();


void dfs(int, std::vector<int>&);

int findWeaklyConnectedComponents();

//--------------------

void readDag(std::string);

void findStartEndNodes();