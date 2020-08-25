/* Main Uncompress cpp
 * Used for uncompress the encoded message to original
 * Name: Jiahe Liu
 * Account: jil061@ieng6.ucsd.edu
 */
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cstdlib>
#include "HCTree.hpp"

#define IN 1
#define OUT 2
#define ARGUMENT 3
#define ASCII 256

using namespace std;


int main(int argc, char** argv){

    if (argc != ARGUMENT){
        cout << "This program requires 2 arguments"<< endl;
        return false;
    }
    ifstream in;
    ofstream out;
    byte buffer;
    string buf;
    vector<int> frequency (ASCII, 0); // initialize a vector to all 0's
    HCTree tree;
    int freq;
    int decodeNumber;
    int count;
    bool empty = true;

    in.open(argv[IN], ios::binary);
    if(!in.is_open()){
    cout << "Invalid input file. No file was opened." <<
        "Please try again." << endl;
        in.close();
        return false;
    }
    
    getline(in, buf);
    count = atoi(buf.c_str());  // Get the total chars
    
    BitInputStream input(in);
    tree.rebuild(input);    // Rebuild the tree structure
    out.open(argv[OUT], ios::binary);
    while(count > 0){   // uncompress messages until touch the total chars
        decodeNumber = tree.decode(input);
        buffer = (byte)decodeNumber;
        out << buffer;
        count--;
    }
    in.close();
    out.close();
}

