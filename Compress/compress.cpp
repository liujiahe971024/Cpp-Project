/* Main compress cpp
 * Used for compress file and save memory
 * Name: Jiahe Liu
 * Account: jil061@ieng6.ucsd.edu
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "HCTree.hpp"

#define IN 1
#define OUT 2
#define ARGUMENT 3
#define ASCII 256
#define AByte 8
using namespace std;

int main(int argc, char** argv){

    if (argc != ARGUMENT){
        cout << "This program requires 2 arguments"<< endl;
        return false;
    }
    ifstream in;
    ofstream out;
    byte buffer;
    vector<int> frequency (ASCII); // initialize a vector to all 0's
    int position;
    HCTree tree;
    bool empty = true;
    int countChar = 0;
     
    in.open(argv[IN], ios::binary);
    if(!in.is_open()){
    cout << "Invalid input file. No file was opened." <<
        "Please try again." << endl;
        in.close();
        return false;
    }
    // Get frequency and total char inside the file
    while((position = in.get()) != EOF){
        countChar++;
        frequency[position]++;
        empty = false; // Test if it is an empty file
    }
    in.close();
    if (!empty) // If not empty , build the tree
        tree.build(frequency);
    else{
        out.open(argv[OUT]);  // If empty, open the output file and quit
        return 0;
    }
    out.open(argv[OUT], ios::binary);
    in.open(argv[IN], ios::binary);

    BitOutputStream output(out);
    out << countChar << endl;   // Write the total chars
    tree.header(output);    //Encode the whole tree structure
    
    while(true){    //Encode the messages
        buffer = (byte)in.get();
        if (in.eof()){
            if(output.getBits() != AByte) out.flush();
            break;
        }
        tree.encode(buffer, output);
    }
    in.close();
    out.close();
}
