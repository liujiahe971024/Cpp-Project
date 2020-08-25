/**
 *  CSE 100 PA2 C++ Autocomplete
 *  Authors: Yiran Shao    Jiahe Liu
 */

#ifndef TRINODE_HPP
#define TRINODE_HPP

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;


/**
 *  The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 */
class TriNode
{
public:

    unordered_map<char, TriNode*>* next;
    unsigned int frequency;
    unsigned int maxFreq;
    /* Create a new Dictionary that uses a Trie back end */
    TriNode(unsigned int freq):frequency(freq){
        next = new unordered_map<char, TriNode*>(); // Add by LK
        maxFreq = 0;
    }

    /* Destructor */
    ~TriNode(){
        for( auto itr = next->begin(); itr != next->end(); itr++ )
            delete itr->second;
        delete next;
    }

};

#endif // TRINODE_H
