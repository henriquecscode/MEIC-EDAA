#include <random>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "loader.h"
#include "edge.h"
#include "node.h"

using namespace std;
unsigned int SEED = 42;
unsigned int DEFAULT_NUMBER_OF_PAIRS = 2;

string DATA_DIR = "data/";
string DIJKSTRA_DIR = DATA_DIR + "dijkstra/";
string DFS_DIR = DATA_DIR + "dfs/";
string ERDOS_DIR = DATA_DIR + "erdos/";
string BFS_DIR = DATA_DIR + "bfs/";
string SPANNING_TREE_DIR = DATA_DIR + "spanning_tree/";

Multigraph multigraph;

string getDate()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S");
    string str = oss.str();
    return str;
}

void initDirs()
{
    int check;
    check = mkdir(DATA_DIR.c_str(), 0777);

    check = mkdir(DIJKSTRA_DIR.c_str(), 0777);
    check = mkdir(DFS_DIR.c_str(), 0777);
    check = mkdir(ERDOS_DIR.c_str(), 0777);
    check = mkdir(BFS_DIR.c_str(), 0777);
    check = mkdir(SPANNING_TREE_DIR.c_str(), 0777);
}

Node *getRandomNode()
{
    // call function to get random node
    auto nodes = multigraph.getNodes();
    int randomIndex = rand() % nodes.size();
    return nodes[randomIndex];
}

vector<pair<Node *, Node *>> getPairNodesForTesting(int numberOfPairs)
{
    vector<pair<Node *, Node *>> pairs;
    for (int i = 0; i < numberOfPairs; i++)
    {
        Node *origin = getRandomNode();
        Node *destination = getRandomNode();
        pairs.push_back(make_pair(origin, destination));
    }
    return pairs;
}
vector<pair<Node *, Node *>> getPairNodesForTesting()
{
    return getPairNodesForTesting(DEFAULT_NUMBER_OF_PAIRS);
}

vector<pair<string, EdgeFilter>> getEdgeFilters()
{
    vector<pair<string, EdgeFilter>> edgeFilters;
    EdgeFilter filter;
    string filterName;
    filter = Edge::getEdgeFilter();
    filterName = "noFilter";
    edgeFilters.push_back(make_pair(filterName, filter));

    return edgeFilters;
}

vector<pair<string, EdgeWeighter>> getEdgeWeighters()
{
    vector<pair<string, EdgeWeighter>> edgeWeighters;
    EdgeWeighter weighter;
    string weighterName;
    weighter = Edge::getEdgeWeighter("distance");
    weighterName = "distanceWeighter";
    edgeWeighters.push_back(make_pair(weighterName, weighter));

    return edgeWeighters;
}

void testDijkstra()
{
    vector<pair<Node *, Node *>> pairs = getPairNodesForTesting();
    vector<pair<string, EdgeFilter>> edgeFilters = getEdgeFilters();
    vector<pair<string, EdgeWeighter>> edgeWeighters = getEdgeWeighters();
    vector<string> algorithmNames = {"dijkstra",
                                     "dijkstraByNode"};

    cout << "Starting dijskstra test" << endl;
    for (int i = 0; i < algorithmNames.size(); i++)
    {
        string algorithmName = algorithmNames[i];
        int algorithmNumber = i + 1;

        for (auto pairNameFilter : edgeFilters)
        {
            string filterName = pairNameFilter.first;
            EdgeFilter filter = pairNameFilter.second;
            for (auto pairNameWeigter : edgeWeighters)
            {
                string weighterName = pairNameWeigter.first;
                EdgeWeighter weighter = pairNameWeigter.second;

                string date = getDate();
                string filedir = DIJKSTRA_DIR + date + "_" + filterName + "_" + weighterName + "_" + algorithmName + "/";
                string logFilename = filedir + "log.csv";
                string totalFilename = filedir + "total.csv";
                mkdir(filedir.c_str(), 0777);

                ofstream file(logFilename);
                ofstream totalFile(totalFilename);
                file << "origin, destination, duration(mu_s)\n";
                long long total = 0;
                long long totalAverage;
                for (auto pair : pairs)
                {
                    Node *origin = pair.first;
                    Node *destination = pair.second;
                    vector<Node *> nodes = {origin, destination};
                    auto start = std::chrono::high_resolution_clock::now();
                    multigraph.getShortestPathDijkstra(nodes, filter, weighter, algorithmNumber);
                    auto finish = std::chrono::high_resolution_clock::now();
                    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
                    total += microseconds;
                    file << origin->getData().getId() << "," << destination->getData().getId() << "," << microseconds << endl;
                }
                totalAverage = total / pairs.size();
                totalFile << "total(mu_s), numberOfRuns, totalAverage(mu_s)\n";
                totalFile << total << "," << pairs.size() << "," << totalAverage << endl;

                file.close();
                totalFile.close();
            }
        }
    }

    cout << "Finished dijkstra test" << endl;
}

void testDfs()
{
    cout << "Starting dfs test" << endl;
    cout << "Finished dfs test" << endl;
}
void testErdos()
{

    cout << "Starting erdos test" << endl;
    cout << "Finished erdos test" << endl;
}
void testBfs()
{
    cout << "Starting bfs test" << endl;
    cout << "Finished bfs test" << endl;
}
void testSpanningTree()
{
    cout << "Starting spanning tree test" << endl;
    cout << "Finished spanning tree test" << endl;
}

int main(int argc, char *argv[])
{
    srand(SEED);
    initDirs();

    cout << "Loading data" << endl;
    loadData(multigraph, argc, argv);

    cout << "Loaded data" << endl;
    testDijkstra();
    testDfs();
    testErdos();
    testBfs();
    testSpanningTree();

    return 0;
}
