#include "DictionaryTrie.hpp"
#include "util.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
using namespace std;

/**
 * IMPORTANT! You should use the following lines of code to match the correct output:
 * 
 * cout << "This program needs exactly one argument!" << endl;
 * cout << "Reading file: " << file << endl;
 * cout << "Enter a prefix/pattern to search for:" << endl;
 * cout << "Enter a number of completions:" << endl;
 * cout << completion << endl;
 * cout << "Continue? (y/n)" << endl;
 * 
 * arg 1 - Input file name (in format like freq_dict.txt)
 */
int main(int argc, char** argv)
{
    if( argc != 2){
        cout << "This program needs exactly one argument!" << endl;
        return 0;
    }
    ifstream in;
    in.open(argv[1], ios::binary);
    if(!in.is_open()) {
        cout<< "Invalid input file. No file was opened. Please try again.\n";
        in.close();
        return false;
    }
    
    cout << "Reading file: " << argv[1] << endl;
    DictionaryTrie* dictionary = new DictionaryTrie();
    Utils::load_dict( *dictionary, in );
    

    string prefix;
    string numLine;
    string buf;
    unsigned int completion;
    std::vector<std::string> myVector;
    string check;
    cout << "Enter a prefix/pattern to search for:" << endl;
    while( getline( cin, prefix ) ){
        
        cout << "Enter a number of completions:" << endl;
        getline( cin, buf );
        completion = atoi(buf.c_str());
        int prefixPosition = prefix.find("_");
        if (prefixPosition == -1)  // LK
            myVector = dictionary->DictionaryTrie::predictCompletions( prefix, 
                completion );
        else  // LK
            myVector = dictionary->DictionaryTrie::predictUnderscore ( prefix,
                completion );
       
        for( unsigned int i = 0; i < myVector.size(); i++ )
            cout << myVector[i] << endl;
        cout << "Continue? (y/n)" << endl;
        getline(cin, check);
        if( check.compare("y") == 0 ){
            cout << "Enter a prefix/pattern to search for:" << endl;
        }
        else{
            break;
        }
    }
    delete dictionary;
    return 0;
}
