/* Header file of Bit output
 * transfer bit to byte
 * Name: Jiahe Liu
 * Account: jil061@ieng6.ucsd.edu
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP
#include <iostream>

typedef unsigned char byte;
typedef unsigned int unsignInt;

using namespace std;


/** Reference from "stepik" 7.4
 * Used for bitwise output
 */
class BitOutputStream{

public:
    // Constructor 
    BitOutputStream(ostream & output): out(output), buf(0), nbits(0){}
    // Deal with bitwise buffer 
    void flush();
    // input bit to the bitwise buffer 
    void writeBit(unsignInt bit);

    int getBits();
private:
    byte buf;   // one byte bitwise buffer
    int nbits;   // bits written to the buffer 
    ostream & out; // output ref
};

#endif

