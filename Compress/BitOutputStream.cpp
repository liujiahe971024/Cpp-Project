/* Main BitOutput filr
 * Include three methods
 * Name: Jiahe Liu
 * Account: jil061@ieng6.ucsd.edu
 */ 
#include <iostream>
#include "BitOutputStream.hpp"
#define MaxBit 8
using namespace std;
/* Transfer 8 bits to one byte*/
void BitOutputStream::writeBit(unsignInt bit){
   
    byte tempBit; 
    if (bit == 1)
        tempBit = 0x80;
    else
        tempBit = 0x00;
    tempBit = tempBit >> nbits;
    buf = buf | tempBit; 
    nbits++;
    if (nbits == MaxBit) flush();
}

/*Write out the byte*/
void BitOutputStream::flush(){

    out.put(buf);
    out.flush();
    buf = 0;
    nbits = 0;

}
/* Handle padding */
int BitOutputStream::getBits(){
    out.put(buf);
    return nbits;
}
