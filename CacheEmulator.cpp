#include "CPU.h"

#define MIN( a, b ) ( ( a < b) ? a : b )
#define MAX( a, b ) ( ( a < b) ? b : a )

class Address;
class Cache;
class Cpu;
class DataBlock;
class Ram;

// Defining dataBlock Size to be constant to begin with to be used by the 
// classes. This can be changed during execution
int DataBlock::size = 64;

/*
Function to modify values basis user inputs
*/
int Inputs(int argc, char* argv[], int *cacheSize, int *dataBlockSize, int *nWayAssociativity, std::string *replacementPolicy, std::string *algo, int *n, int *B, std::string *to_print) 
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-c", argv[i]) == 0)
            *cacheSize = atoi(argv[++i]);

        else if (strcmp("-b", argv[i]) == 0)
            *dataBlockSize = atoi(argv[++i]);

        else if (strcmp("-n", argv[i]) == 0)
            *nWayAssociativity = atoi(argv[++i]);

        else if (strcmp("-r", argv[i]) == 0)
        {
            if (strcmp("FIFO", argv[i + 1]) == 0)
                *replacementPolicy = "FIFO";

            else if (strcmp("LRU", argv[i + 1]) == 0)
                *replacementPolicy = "LRU";

            else
                *replacementPolicy = "RANDOM";

            i++;
        }

        else if (strcmp("-a", argv[i]) == 0)
        {
            if (strcmp("daxpy", argv[i + 1]) == 0)
                *algo = "daxpy";

            else if (strcmp("mxm", argv[i + 1]) == 0)
                *algo = "mxm";
            
            else
                *algo = "mxm_blocked";
            i++;
        }

        else if (strcmp("-d", argv[i]) == 0)
            *n = atoi(argv[++i]);

        else if (strcmp("-f", argv[i]) == 0)
            *B = atoi(argv[++i]);

        else if (strcmp("-p", argv[i]) == 0)
            *to_print = "p";

        else
        {
            std::cout << "INVALID ARGUMENTS" << std::endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int cacheSize = 65536;
    int dataBlockSize = 64;
    int nWayAssociativity = 2;
    std::string replacementPolicy = "LRU";
    std::string algo = "mxm";

    int sz = 8;
    int n = 480;
    int B = 32;

    std::string to_print = "np";
    int inputs = Inputs(argc, argv, &cacheSize, &dataBlockSize, &nWayAssociativity, &replacementPolicy, &algo, &n, &B, &to_print);
    
    // In case invalid arguments were passed
    if (inputs == 1)
        return EXIT_FAILURE;

    DataBlock::size = dataBlockSize;

    int ramSize;
    double register0;

    if (algo == "daxpy")
        ramSize = MAX(ceil(log2(n * 3)), log2(cacheSize));
    else
        ramSize = MAX(ceil(log2(n * n * 3)), log2(cacheSize));

    Cpu myCpu = Cpu(cacheSize, dataBlockSize, nWayAssociativity, replacementPolicy, algo, ramSize);
    

    if (algo == "daxpy")

    {

        std::vector<Address> a(n, 0);
        std::vector<Address> b(n, 0);
        std::vector<Address> c(n, 0);

        for (int i = 0; i < n; i++)
        {
            a[i] = Address(i * sz);
            b[i] = Address((n + i) * sz);
            c[i] = Address((2 * n + i) * sz);
        }

        for (int i = 0; i < n; i++)
        {
            myCpu.storeDouble(a[i], i);
            myCpu.storeDouble(b[i], 2 * i);
        }

        double register0, register1, register2, register3, register4;
        register0 = 3;
        myCpu.resetCounts();
        for (int i = 0; i < n; i++)
        {
            register1 = myCpu.loadDouble(a[i]);
            register2 = myCpu.multDouble(register0, register1);
            register3 = myCpu.loadDouble(b[i]);
            register4 = myCpu.addDouble(register2, register3);
            myCpu.storeDouble(c[i], register4);
        }

        if (to_print == "p")
        {
            for (int i = 0; i < n; i++)
            {
                register0 = myCpu.loadDouble(c[i]);
                std::cout << "c[" << i << "] = " << register0 << std::endl;
            }
        }
    }
    else if (algo == "mxm")
    {
        std::vector<std::vector<Address>> y(n, std::vector<Address>(n, 0));
        std::vector<std::vector<Address>> z(n, std::vector<Address>(n, 0));
        std::vector<std::vector<Address>> x(n, std::vector<Address>(n, 0));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                y[i][j] = Address((i * n + j) * sz);
                z[i][j] = Address((n * n + i * n + j) * sz);
                x[i][j] = Address((2 * n * n + i * n + j) * sz);
            }
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                myCpu.storeDouble(y[i][j], n * i + j);
                myCpu.storeDouble(z[i][j], 2 * (n * i + j));
            }
        }

        double register0, register1, register2, register3;

        myCpu.resetCounts();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                register0 = 0;
                for (int k = 0; k < n; k++)
                {
                    register1 = myCpu.loadDouble(y[i][k]);
                    register2 = myCpu.loadDouble(z[k][j]);
                    register3 = myCpu.multDouble(register1, register2);
                    register0 = myCpu.addDouble(register3, register0);
                }
                myCpu.storeDouble(x[i][j], register0);
            }
        }
        if (to_print == "p")
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    register0 = myCpu.loadDouble(x[i][j]);
                    std::cout << "x[" << i << "][" << j << "] = " << register0 << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::vector<std::vector<Address>> y(n, std::vector<Address>(n, 0));
        std::vector<std::vector<Address>> z(n, std::vector<Address>(n, 0));
        std::vector<std::vector<Address>> x(n, std::vector<Address>(n, 0));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                y[i][j] = Address((i * n + j) * sz);
                z[i][j] = Address((n * n + i * n + j) * sz);
                x[i][j] = Address((2 * n * n + i * n + j) * sz);
            }
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                myCpu.storeDouble(y[i][j], n * i + j);
                myCpu.storeDouble(z[i][j], 2 * (n * i + j));
            }
        }

        double register0, register1, register2, register3;
        
        myCpu.resetCounts();
        for (int jj = 0; jj < n; jj+=B)
        {
            for (int kk = 0; kk < n; kk+=B)
            {
                for (int i = 0; i < n; i++)
                {
                    for (int j = jj; j < MIN(jj + B, n); j++)
                    {
                        register0 = 0;
                        for (int k = kk; k < MIN(kk + B, n); k++)
                        {
                            register1 = myCpu.loadDouble(y[i][k]);
                            register2 = myCpu.loadDouble(z[k][j]);
                            register3 = myCpu.multDouble(register1, register2);
                            register0 = myCpu.addDouble(register3, register0);
                        }
                        register1 = myCpu.loadDouble(x[i][j]);
                        register2 = myCpu.addDouble(register1, register0);
                        myCpu.storeDouble(x[i][j], register2);
                    }
                }
            }
        }
        if (to_print == "p")
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    register0 = myCpu.loadDouble(x[i][j]);
                    std::cout << "x[" << i << "][" << j << "] = " << register0 << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    std::cout << "INPUTS================================" << std::endl;
    std::cout << "Ram Size = " << pow(2, ramSize) << std::endl;
    std::cout << "Cache Size = " << cacheSize << std::endl;
    std::cout << "Data Block Size = " << dataBlockSize << std::endl;
    std::cout << "Total Blocks in Cache = " << cacheSize / dataBlockSize << std::endl;
    std::cout << "Associativity = " << nWayAssociativity << std::endl;
    std::cout << "Number of Sets = " << cacheSize / dataBlockSize / nWayAssociativity << std::endl;
    std::cout << "Replacement Policy = " << replacementPolicy << std::endl;
    std::cout << "Algorithm = " << algo << std::endl;
    if (algo == "mxm_blocked")
        std::cout << "MxM Blocking Factor = " << B << std::endl;
    std::cout << "Matrix or Vector Dimension = " << n << std::endl;
    std::cout << "RESULTS================================" << std::endl;
    std::cout << "Instruction Count = " << myCpu.globalCtr << std::endl;
    std::cout << "Read Hits = " << myCpu.readHits << std::endl;
    std::cout << "Read Misses = " << myCpu.readMisses << std::endl;
    std::cout << "Read Miss Rate = " << double(myCpu.readMisses) / (myCpu.readHits + myCpu.readMisses) * 100 << "%" << std::endl;
    std::cout << "Write Hits = " << myCpu.writeHits << std::endl;
    std::cout << "Write Misses = " << myCpu.writeMisses << std::endl;
    std::cout << "Write Miss Rate = " << double(myCpu.writeMisses) / (myCpu.writeHits + myCpu.writeMisses) * 100 << "%" << std::endl;
    std::cout << "Total Hits = " << myCpu.readHits + myCpu.writeHits << std::endl;
    std::cout << "Total Misses = " << myCpu.readMisses + myCpu.writeMisses << std::endl;
    std::cout << "Total Misses Rate = " << double(myCpu.readMisses + myCpu.writeMisses) / (myCpu.readHits + myCpu.writeHits + myCpu.readMisses + myCpu.writeMisses) * 100 << "%" << std::endl;

}