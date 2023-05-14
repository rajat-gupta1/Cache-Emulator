## Capabilities

1) Variable RAM Size
2) Variable Cache Size
3) Variable Block Size
4) Block Placement Strategy  
    a) Direct Mapped Cache  
    b) Fully associative Cache  
    c) N-way associative cache for arbitrary n  
5) Block Replacement Policy:  
    a) Random
    b) LRU
    c) FIFO
6) Write Policy – Write through with write allocate
7) Algorithms:  
    a) Daxpy
    b) MxM
    c) MxM_Block
8) CPU instructions for double precision floating point arithmetic:  
    (a) value3 = addDouble(value1, value2)
    (b) value3 = multDouble(value1, value2)
    (c) value = loadDouble(address)
    (d) storeDouble(address, value)

## Events

The program is monitoring the following events:  
1) Instruction Counts  
2) Read Hits  
3) Read Misses  
4) Write Hits  
5) Write Misses  

## Interface

The program is supporting the following command line options:
1) -c : The size of the cache in bytes (default: 65,536)
2) -b : The size of a data block in bytes (default: 64)
3) -n : The n-way associativity of the cache. -n 1 is a direct-mapped cache. (default: 2) •-r : The replacement policy. Can be random, FIFO, or LRU. (default: LRU)
4) -a : The algorithm to simulate. Can be daxpy (daxpy product), mxm (matrix-matrix multiplication), mxm block (mxm with blocking). (default: mxm block).
5) -d : The dimension of the algorithmic matrix (or vector) operation. -d 100 would result in a 100 ×100 matrix-matrix multiplication. (default: 480)
6) -p : Enables printing of the resulting “solution” matrix product or daxpy vector aYer the emulation is complete. Elements should be read in emulation mode (e.g., using your loadDouble method), so as to
assess if the emulator actually produced the correct solution.
7) -f : The blocking factor for use when using the blocked matrix multiplication algorithm. (default: 32)

An example code for executing the executable is:  
./cache-sim -a mxm -d 512 -n 8  