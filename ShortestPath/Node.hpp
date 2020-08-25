/*
 * Node.hpp
 * Author:  Jiahe Liu
 * Date: 06/01/2019
 * Email: jil061@ieng6.ucsd.edu
 *
 * Node class for every author
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Node;

// Struct of edges include a weight and movie's name and year together
struct List{
    string nameAndYear;
    int weight;
    vector<Node*> connection;
};

// Actor Node class 
class Node{
public:
    string actorName;
    bool visit;
    vector<List*> movie;
    string movieN;
    int dist;
    Node* prev;
    bool done;

    Node* parent;
    int height;

    /*  Constructor */
    Node(){
        visit = false;
        prev = NULL;
    }
    /* Destructor */
    ~Node();
};







#endif
