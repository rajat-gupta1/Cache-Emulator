#include "Ram.h"

/*
The cache class is used to emulate the behaviour of L1 Cache
*/
class Cache {
public:

    // Number of cache lines
    int numSets;
    int numBlocks;
    std::vector<std::vector<DataBlock>> blocks;

    // Map type of vector for the tags associated with the cache
    std::vector<std::vector<int>> tagMap;
    int cacheSize;
    int dataBlockSize;
    int nWayAssociativity;

    
    int indexLength;
    int offsetLength;

    void Cache_init(int cSize, int dbSize, int nWAssociativity)
    {
        cacheSize = cSize;
        dataBlockSize = dbSize;
        nWayAssociativity = nWAssociativity;

        // The number of bits in index
        indexLength = log2(cacheSize) - log2(dataBlockSize) - log2(nWayAssociativity);

        // The number of bits in the offset (doubles in the blocks)
        offsetLength = log2(dataBlockSize) - log2(double_size);
        
        // Number of sets 
        numSets = pow(2, indexLength);

        // Number of doubles in a cache line
        numBlocks = pow(2, offsetLength);;

        blocks.resize(numSets, std::vector<DataBlock>(numBlocks));
        tagMap.resize(numSets, std::vector<int>(nWayAssociativity, -1));
    }

    /*
    The getBlock method is used to get a block equivalent to cache line 
    size from Cache to Ram
    */
    DataBlock getBlock(Address address, int i = 0)
    {
        return blocks[address.getIndex(indexLength, offsetLength)][i];
    }

    /*
    The setBlock method is used to set a block equivalent to cache line 
    size to Cache from Ram
    */
    void setBlock(Address address, DataBlock value, int i = 0)
    {
        blocks[address.getIndex(indexLength, offsetLength)][i] = value;
    }

    /*
    The getDouble method is used to get a double from Cache to CPU (Register)
    */
    double getDouble(Address address, int i = 0)
    {
        tagMap[address.getIndex(indexLength, offsetLength)][i] = address.getTag(indexLength, offsetLength);
        return blocks[address.getIndex(indexLength, offsetLength)][i].data[address.getOffset(indexLength, offsetLength)/8];
    }

    /*
    The setDouble method is used to set a double from CPU (Register) to Cache
    */
    void setDouble(Address address, double value, int i = 0)
    {
        blocks[address.getIndex(indexLength, offsetLength)][i].data[address.getOffset(indexLength, offsetLength)/8] = value;
        tagMap[address.getIndex(indexLength, offsetLength)][i] = address.getTag(indexLength, offsetLength);
    }
};

