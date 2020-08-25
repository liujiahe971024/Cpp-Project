/** Huffman Tree header file
 * Used to describe method about build the tree, encode and decode
 * Name: Jiahe Liu
 * Email: jil061@ucsd.edu
 */
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <stack>
#include "HCTree.hpp"
#include "BitOutputStream.hpp"
#define AByte 8

using namespace std;

/* Build the huffman tree by the frequency we got from compress.cpp*/
void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    byte symbol;
    int c1Freq = 0;
    int c2Freq = 0;
    bool unique = false;
    for(int i = 0 ; i < freqs.size(); i++){
        if(freqs[i] != 0){ // if the symbol exist
            HCNode* temp = new HCNode(0,0,0,0,0);
            temp->count = freqs[i];
            temp->symbol = (byte)i;
            leaves[i] = temp;
            pq.push(leaves[i]);
        }
    }
    if(pq.size() == 1){
        HCNode* pNode = new HCNode(0,0,0,0,0);
        pNode->c0 = pq.top();
        pq.top()->p = pNode;
        pNode->count = pq.top()->count;
        pq.pop();
        pq.push(pNode);
        unique = true;
    }

    while(pq.size() != 1 && unique == false){
        HCNode* pNode = new HCNode(0,0,0,0,0);
        
        pNode->c0 = pq.top();
        pq.top()->p = pNode;
        c1Freq = pq.top()->count;
        pq.pop();

        pNode->c1 = pq.top();
        pq.top()->p = pNode;
        c2Freq = pq.top()->count;
        pq.pop();    
    
        pNode->count = c1Freq + c2Freq;
        pq.push(pNode);
    }
    
    this->root = pq.top();
    pq.pop();

}
/* Transfer a single char to some binary number and put into output file*/
void HCTree::encode(byte symbol, BitOutputStream& out) const{
    
    HCNode* node = leaves[symbol];
    stack<unsignInt> list;
    while(true){
        
        if(node == this->root) break;
        if(node->p->c1 == node) list.push(1);
        if(node->p->c0 == node) list.push(0);
        node = node->p;

    }

    int size = list.size();
    for (int i = 0; i < size; i++){
        out.writeBit(list.top());
        list.pop();
    }
}
/* Transfer a couple binary number to a single char and return the char*/
int HCTree::decode(BitInputStream& in) const{

    HCNode* node = this->root;
    if(!root) return false;
    while(node->c1 || node->c0){
        unsignInt move = in.readBit();
        if (move == 1) node = node->c1;
        else if (move == 0) node = node->c0;
        else return false;
    }
    return node->symbol;
     
}
/* Encode the header by store the whole tree structure */
void HCTree::header(BitOutputStream& out){
    
    HCNode* node = this->root;
    travelBuild(node, out);
    if (uniq) out.writeBit(1);
}
/* Recursion build the header */
void HCTree::travelBuild(HCNode* n, BitOutputStream& out){
    
    if(n->c0){
        out.writeBit(0);
        travelBuild(n->c0, out);
    }
    if(n->c1){
        out.writeBit(0);
        travelBuild(n->c1, out);
    }
    if(n == this->root) return;
    if(!n->c0 && !n->c1){
        nLeaves++;
        if (nLeaves > 1) uniq = false;
        out.writeBit(1);
        for (int i = AByte - 1; i > -1; i--){
            Byte tempChar = n->symbol;
            tempChar = tempChar >> i;
            Byte char1 = 0x01;
            if ((tempChar & char1) == 1)
                out.writeBit(1);
            else
                out.writeBit(0);
        } 
    }
}
/* Rebuild the tree structure by the header we stored */
void HCTree::rebuild(BitInputStream& in){
    HCNode* node = new HCNode(0,0,0,0,0);
    travelReBuild(node, in);
    //for (;node->c1;node = node->c1);
    //cout << node->symbol << endl;
    this->root = node;
    //deleteAll(node);
}
/* Recursion build the tree structure */
void HCTree::travelReBuild(HCNode* n, BitInputStream& in){

    unsignInt bit = in.readBit();
    if(bit == 0){
        HCNode* node = new HCNode(0,0,0,0,0);
        n->c0 = node;
        node->p = n;
        travelReBuild(n->c0, in);
        unsignInt nextBit = in.readBit();
        if (nextBit == 0){
            HCNode* rightNode = new HCNode(0,0,0,0,0);
            n->c1 = rightNode;
            rightNode->p = n;
            travelReBuild(n->c1, in);
        }
    }
    if (bit == 1){
        Byte character = 0x00;
        for(int i = 0; i < AByte; i++){
            unsignInt tempBit = in.readBit();
            character = character | tempBit;
            if(i < AByte - 1)
                character = character << 1;
        }
        n->symbol = character;
    }
}

/* destructor */
HCTree::~HCTree(){
    deleteAll(root);
}
/* Recursion delete all the node */
void HCTree::deleteAll(HCNode* node){
    if(!node) return;
    if(node->c0) deleteAll(node->c0);
    if(node->c1) deleteAll(node->c1);
    delete node; 
}
