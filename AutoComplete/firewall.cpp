#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "BloomFilter.hpp"
#define BAD 1
#define MIX 2
#define OUT 3
#define ARGUMENT 4
using namespace std;

/**
 * IMPORTANT! You should use the following lines of code to match the correct output:
 * cout << "This program requires 3 arguments!" << endl;
 * 
 * arg1 - list of malicious urls/bad words filter out
 * arg2 - list of mixed (good/bad) to only write good urls to
 * arg3 - file to write only the good urls to (one on each line)
 */

int main(int argc, char** argv) {
/*ifstream in;
    string temp;
in.open( argv[1] );
    BloomFilter* urls = new BloomFilter(9);

while( in.good() ){
getline( in, temp);
urls->insert(temp);
}

    in.close();
    in.clear();

in.open( argv[1] );
while( in.good() ){
getline( in, temp );
if(urls->find(temp))
cout<<"nice"<<endl;
else
cout<<"not find " << temp <<endl;
}


    in.close();
    in.clear();
*/
    if (argc != ARGUMENT){
        cout << "This program requires 3 arguments!" << endl;
        return 0;
    }

    double good = 0;
    double lineMix = 0;
    size_t lineBad = 0;
    double badOrProFalse = 0;
    ifstream in;
    ofstream out;
    string temp;

    in.open(argv[BAD]);
    if(!in.is_open()){
        cout << "Invalid input file. No file was opened." << 
            " Please try again." << endl;
        in.close();
        return false;
    }
    // Count the total lines in the BadUrls file
    while(in.good()){
        getline(in, temp);
        lineBad++;
    }

    size_t tableSize = lineBad * (3.0/2.0);
   // lineBad = lineBad * (3.0/2.0);
  //  size_t numByte = ceil(lineBad);

    in.close();
    in.clear();
    BloomFilter* urls = new BloomFilter(tableSize);
    
    // Fills all bad urls into the bloom filter 
    in.open(argv[BAD]);
    while(in.good()){
        getline(in, temp);
        urls->insert(temp);
    }
    in.close();
    in.clear();


/*    in.open(argv[MIX]);
    if(!in.is_open()){
        cout << "Invalid input file. No file was opened." << 
            " Please try again." << endl;
        in.close();
        return false;
    }
    
    //Count the total lines in the MixUrls file
    while(in.good()){
        getline(in, temp);
        lineMix++;
    }
    in.close();
    in.clear();
*/
    in.open(argv[MIX]);
    if(!in.is_open()){
        cout << "Invalid input file. No file was opened." << 
            " Please try again." << endl;
        in.close();
        return false;
    }
    out.open(argv[OUT]);
    if(!out.is_open()){
        cout << "Invalid input file. No file was opened." << 
            " Please try again." << endl;
        out.close();
        return false;
    }

    // Compare MixUrls with BadUrls
    while(in.good()){
        getline(in, temp);
        lineMix++;
        if (urls->find(temp))
            badOrProFalse++; // If Urls found, maybe bad maybe prositive false
        else {
            out << temp << endl;    // Output good urls into the file
            good++; // Count good urls
        }
    }
    in.close();
    out.close();

    in.open(argv[BAD], ios_base::binary);
    in.seekg(0,ios_base::end);
    double fileSize = in.tellg();
    in.close();
    double filterSize = tableSize;
    
    double safeUrls = lineMix - lineBad;
    double errorRate = (safeUrls - good) / safeUrls;
    double memory = fileSize / filterSize;
    
    cout << "False positive rate: " << errorRate << endl;
    cout << "Saved memory ratio: " << memory << endl;
}
    
