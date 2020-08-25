#include <stdint.h>
#include "BloomFilter.hpp"

/* Create a new bloom filter with the size in bytes */
BloomFilter::BloomFilter(size_t numBytes)
{
    numSlots = 8 * numBytes;
    table = new unsigned char[numBytes];
}

/* Insert an item into the bloom filter */
void BloomFilter::insert(std::string item) 
{
    unsigned int firstVal = hashFirst( item, numSlots );
    setSlot( firstVal );
    unsigned int secondVal = hashSecond( item, numSlots );
    setSlot( secondVal );
    unsigned int thirdVal = hashThird( item, numSlots );
    setSlot( thirdVal );
}

/* Determine whether an item is in the bloom filter */
bool BloomFilter::find(std::string item) 
{
    unsigned int firstVal = hashFirst( item, numSlots );
    unsigned int secondVal = hashSecond( item, numSlots );
    unsigned int thirdVal = hashThird( item, numSlots );
    const unsigned char check = 0x00000001;
    if( getSlot(firstVal) == check && (getSlot(secondVal) == check && 
        getSlot(thirdVal) == check) )
        return true;
    return false;
}

/* Destrctor for the bloom filter */
BloomFilter::~BloomFilter()
{
    delete table;
}

/*
 * * Hash a string by interpreting it as a base-256 number. Unlike the
 * * other hash functions in this file, this function must be passed the
 * * table size as an argument and performs the modulo function itself
 * * (otherwise it wouldn&#39;t be able to generate a correct result).
 * *
 * * Note that instead of multiplying by 256, we shift left by 8 bits
 * * (BYTE_WIDTH). This is faster on almost all machines, and happens to be
 * * a bit easier to write in C++ due to the definition of BYTE_WIDTH.
 * */
unsigned int BloomFilter::hashFirst ( string key, int len )
{
    unsigned int BYTE_WIDTH = 8;
    unsigned int hashValue = 0;
    for (string::const_iterator i = key.begin(); i != key.end(); i++)
    {
        hashValue = (hashValue << BYTE_WIDTH) + (unsigned)*i;
        hashValue %= len;
    }
    return hashValue;
}

/*
 * * Hash a string using an algorithm taken from Aho, Sethi, and Ullman,
 * * &quot;Compilers: Principles, Techniques, and Tools,&quot; Addison-Wesley,
 * * 1985, p. 436. PJW stands for Peter J. Weinberger, who apparently
 * * originally suggested the function.
 * *
 * * The basic idea of this algorithm is similar to that of the modified
 * * CRC algorithm, except that instead of shifting the top bits right
 * * so that they line up with the newly emptied bottom bits (a rotate),
 * * the top bits are shifted only far enough to line up with the top
 * * half of the character just XOR-ed into the hash value.
 * */
unsigned int BloomFilter::hashSecond( string key, int len ) {

    // How much to shift hash by, per char hashed
    static const unsigned int PJW_HASH_SHIFT = 4;
    // Right-shift amount, if top 4 bits NZ 32-BIT
    static const unsigned int PJW_HASH_RIGHT_SHIFT = 24;
    // Mask for extracting top 4 bits 32-BIT
    static const unsigned int PJW_HASH_MASK = 0xf0000000;
    unsigned int hashValue = 0;

    for (string::const_iterator i = key.begin(); i != key.end(); i++)
    {
        hashValue = (hashValue << PJW_HASH_SHIFT) + (unsigned)*i;
        unsigned int rotate_bits = hashValue & PJW_HASH_MASK;
        hashValue ^= rotate_bits | (rotate_bits >> PJW_HASH_RIGHT_SHIFT);
    }

    hashValue = hashValue%len;
    return hashValue;

}

unsigned int BloomFilter::hashThird( string myStr, unsigned int length )
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i = 0;

    for (i = 0; i < myStr.length(); ++i)
    {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (myStr.at(i)) * (hash >> 3)) :
                (~((hash << 11) + ((myStr.at(i)) ^ (hash >> 5))));
    }

    hash = hash%length;
    return hash;

}

void BloomFilter::setSlot( unsigned int hashValue ){
    int byte = hashValue/8;
    int bit = hashValue%8;
    table[byte] |= 0x00000001 << (7-bit);
}

unsigned char BloomFilter::getSlot( unsigned int hashValue ){
    int byte = hashValue/8;
    int bit = hashValue%8;
    unsigned char check = table[byte]>>(7-bit) & 0x00000001;
    return check;
}
