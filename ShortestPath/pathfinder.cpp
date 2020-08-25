/*
 *  Pathfinder.cpp
 *  Author: Jiahe Liu
 *  Date: 06/01/2019
 *  Email: jil061@ieng6.ucsd.edu
 *
 *  Main file to call all method for build the graph and find the shortest
 *  path between two actors whatever weighted or not
 *
 */ 
#include <iostream>
#include <string>
#include "ActorGraph.hpp"
#define Arg 5
#define In 1
#define UorW 2
#define Find 3
#define Out 4

using namespace std;

/* Main function */
int main(int argc, char** argv){

    if (argc != Arg){
        cout << "This Program need four argument!" << endl;
        return 0;
    }

    ActorGraph graph;
    string uOrw = argv[UorW]; 
    cout << "Reading " << argv[1] << " ..." << endl;
    if (uOrw == "u"){    // Unweighted method
        if (!graph.loadFromFile(argv[1], false)) return 0;
        graph.find(argv[Find], argv[Out]);
    }
    else if (uOrw == "w"){  // Weighted method
        if (!graph.loadFromFile(argv[1], true)) return 0;
        graph.wFind(argv[Find], argv[Out]);
    }
    else{
        cout << "This Program only take u and w" << endl;
        return 0;
    }

}
