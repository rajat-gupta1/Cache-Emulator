#include "Address.h"

#define double_size 1

class DataBlock {
public:
    static int size;
    std::vector<double> data;

    DataBlock()
    {
        data.resize(size/8);
    }
};

class Ram {
public:
    int numBlocks; 
    std::vector<DataBlock> data;

    int cacheSize;
    int dataBlockSize;
    int nWayAssociativity;
    std::string replacementPolicy;
    std::string algo;

    int indexLength;
    int offsetLength;

    void Ram_init(int cSize, int dbSize, int nWAssociativity, std::string rPolicy, std::string Algo, int rSize)
    {
        cacheSize = cSize;
        dataBlockSize = dbSize;
        nWayAssociativity = nWAssociativity;
        replacementPolicy = rPolicy;
        algo = Algo;

        indexLength = log2(cacheSize) - log2(dataBlockSize) - log2(nWayAssociativity);
        offsetLength = log2(dataBlockSize) - log2(double_size);

        numBlocks = pow(2, rSize);

        data.resize(numBlocks);
    }

    DataBlock getBlock(Address address)
    {
        return data[floor(address.address/dataBlockSize)];
    }

    void setBlock(Address address, DataBlock value)
    {
        data[floor(address.address/dataBlockSize)] = value;
    }
};
