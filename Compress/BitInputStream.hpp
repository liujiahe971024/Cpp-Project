/* Bit input header
 * Used for transfer byte to bit
 * Name: Jiahe Liu
 * Account: jil061@ucsd.edu
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char Byte;
typedef unsigned int unsignInt;

using namespace std;
/* Main header class */
class BitInputStream{
public:
    /* constructor*/
    BitInputStream(istream & is): in(is), buf(0), nbits(8){}
    void fill();    // If bits get 8, transfer to one byte
    unsignInt readBit(); // read bit
private:
    Byte buf;   //unsigned char
    int nbits;
    istream & in;
};


#endif
