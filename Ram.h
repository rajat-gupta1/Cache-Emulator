#include "Address.h"

#define double_size 1

/*
The class DataBlock is used to store data corresponding to a cache line.
By default, it is assumed to be 64 bytes
*/
class DataBlock {
public:
    static int size;
    std::vector<double> data;

    DataBlock()
    {
        // Since we are storing doubles only
        data.resize(size/8);
    }
};

/*
The class Ram is used to mimic the behaviour of memory.
*/
class Ram {
public:

    // The number of blocks the Ram can store
    int numBlocks; 
    std::vector<DataBlock> data;
    int dataBlockSize;

    void Ram_init(int dbSize, int rSize)
    {
        dataBlockSize = dbSize;

        // rSize is the size of the Ram
        numBlocks = pow(2, rSize);
        data.resize(numBlocks);
    }

    /*
    The getBlock method is used to get a block equivalent to cache line 
    size from Ram to Cache
    */
    DataBlock getBlock(Address address)
    {
        return data[floor(address.address/dataBlockSize)];
    }

    /*
    The setBlock method is used to set a block equivalent to cache line 
    size to Ram from Cache
    */
    void setBlock(Address address, DataBlock value)
    {
        data[floor(address.address/dataBlockSize)] = value;
    }
};
