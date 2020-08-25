#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <string>

using namespace std;

/**
 * The class for bloom filter that provides memory efficient check
 * of whether an item has been inserted before. Small amount of 
 * false positives is possible but there will be no false negatives.
 */
class BloomFilter {

public:

    /* Create a new bloom filter with the size in bytes */
    BloomFilter(size_t numBytes);

    /* Insert an item into the bloom filter */
    void insert(std::string item);

    /* Determine whether an item is in the bloom filter */
    bool find(std::string item);

    /* Destrctor for the bloom filter */
    ~BloomFilter();

private:

    // The char array as the hash table
    unsigned char* table;
    unsigned int numSlots;
    
    // TODO: Add your own data and method here
    unsigned int hashFirst ( string item, int len );
    unsigned int hashSecond( string item, int len );
    unsigned int hashThird( string item, unsigned int len );
    void setSlot( unsigned int hashValue );
    unsigned char getSlot( unsigned int hashValue );

};
#endif // BLOOM_FILTER
