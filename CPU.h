#include "Cache.h"

/*
The CPU class loads values in the register from cache, does computations and 
stores them back into the cache.
*/
class Cpu {
public:
    Cache myCache;

    // Vector for storing the usage timestamps of different cache lines
    std::vector<std::vector<int>> lru;

    // Vector for storing the order of timestamps different cache lines entered
    // the cache
    std::vector<std::vector<int>> fifo;
    Ram myRam;
    int cacheSize;
    int dataBlockSize;
    int nWayAssociativity;

    // FIFO, LRU or Random replacement policy inside cache
    std::string replacementPolicy;

    // daxpy, mxm or mxm blocked
    std::string algo;

    int readMisses;
    int readHits;
    int writeMisses;
    int writeHits;

    int indexLength;
    int offsetLength;
    int numSets;

    long long globalCtr;

    Cpu(int cSize, int dbSize, int nWAssociativity, std::string rPolicy, std::string Algo, int rSize)
    {
        cacheSize = cSize;
        dataBlockSize = dbSize;

        nWayAssociativity = nWAssociativity;
        replacementPolicy = rPolicy;
        algo = Algo;

        indexLength = log2(cacheSize) - log2(dataBlockSize) - log2(nWayAssociativity);
        offsetLength = log2(dataBlockSize) - log2(double_size);

        myCache.Cache_init(cacheSize, dataBlockSize, nWayAssociativity, replacementPolicy, algo);
        myRam.Ram_init(dataBlockSize, rSize);
        numSets = pow(2, indexLength);
        resetCounts();

        
        lru.resize(numSets, std::vector<int>(nWayAssociativity, 0));
        fifo.resize(numSets, std::vector<int>(nWayAssociativity, 0));
        globalCtr = 0;
    }

    void resetCounts()
    {
        readMisses = 0;
        readHits = 0;
        writeMisses = 0;
        writeHits = 0;
        for (auto &v: myCache.tagMap) {
            std::fill(v.begin(), v.end(), -1);
        }
    }

    double loadDouble (Address address)
    {
        globalCtr++;
        int thisIndex = address.getIndex(indexLength, offsetLength);
        int thisTag = address.getTag(indexLength, offsetLength);
        double value = 0;
        int cnt = 0; 
        for (int i = 0; i < nWayAssociativity; i++)
        {
            if (thisTag == myCache.tagMap[thisIndex][i])
            {
                lru[thisIndex][i] = globalCtr;
                readHits++;
                value = myCache.getDouble(address, i);
                return value;
            }
        }
        int minLRU = lru[thisIndex][0];
        int minFIFO = fifo[thisIndex][0];
        int fifoIndex = 0;
        int lruIndex = 0;
        for (int i = 1; i < nWayAssociativity; i++)
        {
            if (lru[thisIndex][i] < minLRU)
            {
                minLRU = lru[thisIndex][i];
                lruIndex = i;
            }
            if (fifo[thisIndex][i] < minFIFO)
            {
                minFIFO = fifo[thisIndex][i];
                fifoIndex = i;
            }
        }
        readMisses++;
        readHits++;
        if (replacementPolicy == "LRU")
        {
            myCache.setBlock(address, myRam.getBlock(address), lruIndex); 
            value = myCache.getDouble(address, lruIndex);
            lru[thisIndex][lruIndex] = globalCtr;
        }
        else if (replacementPolicy == "FIFO")
        {
            myCache.setBlock(address, myRam.getBlock(address), fifoIndex); 
            value = myCache.getDouble(address, fifoIndex);
            fifo[thisIndex][fifoIndex] = globalCtr;
        }
        else 
        {
            int randomIndex = rand()%(nWayAssociativity);
            myCache.setBlock(address, myRam.getBlock(address), randomIndex); 
            value = myCache.getDouble(address, randomIndex);
        }
        return value;
    }

    void storeDouble (Address address, double value)
    {
        globalCtr++;
        int thisIndex = address.getIndex(indexLength, offsetLength);
        int thisTag = address.getTag(indexLength, offsetLength);
        int cnt = 0;
        for (int i = 0; i < nWayAssociativity; i++)
        {
            if (thisTag == myCache.tagMap[thisIndex][i])
            {
                lru[thisIndex][i] = globalCtr;
                writeHits++;
                myCache.setDouble(address, value, i);
                return;
            }
        }
        int minLRU = lru[thisIndex][0];
        int minFIFO = fifo[thisIndex][0];
        int fifoIndex = 0;
        int lruIndex = 0;
        for (int i = 1; i < nWayAssociativity; i++)
        {
            if (lru[thisIndex][i] < minLRU)
            {
                minLRU = lru[thisIndex][i];
                lruIndex = i;
            }
            if (fifo[thisIndex][i] < minFIFO)
            {
                minFIFO = fifo[thisIndex][i];
                fifoIndex = i;
            }
        }
        writeMisses++;
        writeHits++;
        if (replacementPolicy == "LRU")
        {
            myCache.setBlock(address, myRam.getBlock(address), lruIndex);   
            myCache.setDouble(address, value, lruIndex);
            myRam.setBlock(address, myCache.getBlock(address, lruIndex));
            lru[thisIndex][lruIndex] = globalCtr;
        }
        else if (replacementPolicy == "FIFO")
        {
            myCache.setBlock(address, myRam.getBlock(address), fifoIndex);   
            myCache.setDouble(address, value, fifoIndex);
            myRam.setBlock(address, myCache.getBlock(address, fifoIndex));
            fifo[thisIndex][fifoIndex] = globalCtr;
        }
        else 
        {
            int randomIndex = rand()%(nWayAssociativity);
            myCache.setBlock(address, myRam.getBlock(address), randomIndex);   
            myCache.setDouble(address, value, randomIndex);
            myRam.setBlock(address, myCache.getBlock(address, randomIndex));
        }
    }

    double addDouble (double value1, double value2)
    {
        return value1 + value2;
    }

    double multDouble (double value1, double value2)
    {
        return value1 * value2;
    }
};
