#include "Ram.h"

class Cache {
public:
    int numSets;
    int numBlocks;
    std::vector<std::vector<DataBlock>> blocks;
    std::vector<std::vector<int>> tagMap;
    int cacheSize;
    int dataBlockSize;
    int nWayAssociativity;
    std::string replacementPolicy;
    std::string algo;

    int indexLength;
    int offsetLength;

    void Cache_init(int cSize, int dbSize, int nWAssociativity, std::string rPolicy, std::string Algo)
    {
        cacheSize = cSize;
        dataBlockSize = dbSize;
        nWayAssociativity = nWAssociativity;
        replacementPolicy = rPolicy;
        algo = Algo;

        indexLength = log2(cacheSize) - log2(dataBlockSize) - log2(nWayAssociativity);
        offsetLength = log2(dataBlockSize) - log2(double_size);

        numSets = pow(2, indexLength);
        numBlocks = pow(2, offsetLength);;

        blocks.resize(numSets, std::vector<DataBlock>(numBlocks));
        tagMap.resize(numSets, std::vector<int>(nWayAssociativity, -1));
    }

    DataBlock getBlock(Address address, int i = 0)
    {
        return blocks[address.getIndex(indexLength, offsetLength)][i];
    }

    void setBlock(Address address, DataBlock value, int i = 0)
    {
        blocks[address.getIndex(indexLength, offsetLength)][i] = value;
    }

    double getDouble(Address address, int i = 0)
    {
        tagMap[address.getIndex(indexLength, offsetLength)][i] = address.getTag(indexLength, offsetLength);
        return blocks[address.getIndex(indexLength, offsetLength)][i].data[address.getOffset(indexLength, offsetLength)/8];
    }

    void setDouble(Address address, double value, int i = 0)
    {
        blocks[address.getIndex(indexLength, offsetLength)][i].data[address.getOffset(indexLength, offsetLength)/8] = value;
        tagMap[address.getIndex(indexLength, offsetLength)][i] = address.getTag(indexLength, offsetLength);
    }
};

