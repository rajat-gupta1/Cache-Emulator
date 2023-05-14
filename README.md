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
a) Instruction Counts
b) Read Hits
c) Read Misses
d) Write Hits
e) Write Misses

An example code for executing the executable is:  
./cache-sim -a mxm -d 512 -n 8  