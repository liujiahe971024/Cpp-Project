/* Main cpp for BituputStream header
 * Include two methods
 * Name: Jiahe Liu
 * Account jil061@ieng6.ucsd.edu
 */
#include <iostream>
#include "BitInputStream.hpp"
#define MaxBit 8
#define SwitchBit 7

using namespace std;
/* Fill in bits when 8*/
void BitInputStream::fill(){

    buf = in.get();
    nbits = 0;

}

/* transfer the byte to bits*/
unsignInt BitInputStream::readBit(){

    if(nbits == MaxBit) fill(); // Read a byte
    unsignInt nextBit = 0;
    Byte temp = 0x00;
    temp = temp | buf;  
    temp = temp >> SwitchBit;   // Consider one or zero 
    if (temp == 0x01) nextBit = 1;
    buf = buf << 1; 
    nbits++;    // track the position
    return nextBit;

}
