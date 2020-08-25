/*
 * ActorGraph.hpp
 * Author: Jiahe Liu
 * Date:   06/01/2019
 * Email:  jil061@ieng6.ucsd.edu
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <queue>
#include <set>
#include "Node.hpp"

// Maybe include some data structures here

using namespace std;

typedef pair<int, pair<string, pair<Node*, Node*>>> edge;
/**
 * TODO: add class header
 */
/*
class comparison {
public:
    bool operator()(pair<int, vector<string>> p1,pair<int, vector<string>> p2) {
        return p1.second < p2.second;
    }
};*/
/* Compare function for periority queue */
class comparison {
public:
    bool operator()(pair<int, Node*> p1, pair<int, Node*> p2) {
        return p1.first > p2.first; // Shortest first
    }
};
/* Compare function for periority queue */
class compare {
public:
    bool operator()(edge p1, edge p2) {
        return p1.first > p2.first; // Shortest first
    }
};

/* Main class with all method needed */
class ActorGraph {
private:
    // Maybe add class data structure(s) here
    queue<Node*> list;
    unordered_map<string, Node*> actor;
    unordered_map<string, vector<string>> movieMap;
    vector<string> fullPath;
    priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, comparison> pq;
    unordered_map<string, Node*> actorList;
    Node* find(Node* a);
    unordered_map<string, vector<string>> movieList;
    priority_queue<edge, vector<edge>, compare> p;
    int totalV;


public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void){};
    /**
     *  Destuctor of the Actor graph 
     */
    ~ActorGraph();

    // Maybe add some more methods here
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year), 
     *                      otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    /**
    *   Find the unweighted shortest path and write out into a file
    *   in_filename - input filename
    *   out - output filename
    */
    void find(const char* in_filename, const char* out);
    
    /**
     *  Find the weighted shortest path by Dij method
     *  Output the path to the output file
     *  in_filename - input filename
     *  out - ouput filename
     */
    void wFind(const char* in_filename, const char* out);

    /**
     *  Find the shortest path by BFS and store it into a priotity queue
     *  theActor - Actor Node
     *  targetName - target actor name
     */
    void uFind(Node* theActor, string targetName);

    /**
     *  Dij method to find the shortest weighted path
     *  theActor - Actor Node
     *  targetName - target actor name
     */
    void dij(Node* theActor, string targetName);

    /**
     *  Output the path into a file
     *  out - output stream
     *  name - target actor name
     *  start - begining actor name
     */
    void outputW(ofstream &out, string name, string start);

    /**
     *  Reset all node as begining
     */
    void resetVisit();

    /**
     *  movieTraveler build the edge
     *  in_ filename - input file name
     */  
    bool buildEdge(const char* in_filename);

    /**
     *  MST method output all the edges
     *  output - output file name
     */  
    void mst(string output);
 
};


#endif // ACTORGRAPH_HPP
