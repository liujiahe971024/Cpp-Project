/*
 * movietraveler.cpp
 * Author:  Jiahe Liu
 * Date:    06/01/2019
 * Email:   jil061@ieng6.ucsd.edu
 *
 * This file is meant to find the shortest path in the graph
 * Output all the edges in the output file
 */
#include <iostream>
#include <string>
#include "Node.hpp"
#include "ActorGraph.hpp"
#define Arg 3
#define OUT 2

using namespace std;


int main(int argc, char** argv){

    if (argc != Arg){
        cout << "This Program need two argument!" << endl;
        return 0;
    }

    ActorGraph main;
    cout << "Reading " << argv[1] << " ..." << endl;
    main.buildEdge(argv[1]);    // Build the edges
    main.mst(argv[OUT]);    // MST method build the graph and output


}
