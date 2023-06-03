#include<iostream>
#include<string>
#include<cmath>
#include <cstring>
#include <typeinfo>
#include <vector>

#define int_size 32

/*
Function to convert decimal to binary
*/
void decToBinary(int n, std::string *bitstr)
{
    // Size of an integer is assumed to be 32 bits
    
    for (int i = int_size - 1; i >= 0; i--) {
        int k = n >> i;
        if (k & 1)
            (*bitstr).append("1");
        else
            (*bitstr).append("0");
    }
}

/*
Function to convert binary to decimal
*/
int binaryToDec(std::string_view bitstr)
{
    int num = 0;
    int len = bitstr.length();
    for (int i = len - 1; i >= 0; i--) 
        // ASCII value of 1
        if (bitstr[i] == 49)
            num += pow(2, len - 1 - i);
    return num;
}

/*
The address class converts and holds the binary address of a decimal address
The methods getTag, getIndex and getOffset returns Tag bits, the Index bits
and the Offset bits respectively associated with the address
*/
class Address {
public:
    int address;
    std::string bitaddress;
    int tagLength;
    int indexLength;
    int offsetLength;

    // One way, two way... or fully associative
    int associativity;

    Address(int num)
    {
        address = num;
        bitaddress = "";
        decToBinary(address, &bitaddress);
    }

    int getTag(int indexLength, int offsetLength) 
    {
        int tag = binaryToDec(bitaddress.substr(0, int_size - offsetLength - indexLength));
        return tag;
    }

    int getIndex(int indexLength, int offsetLength) 
    {
        int index = binaryToDec(bitaddress.substr(int_size - offsetLength - indexLength, indexLength));
        return index;
    }

    int getOffset(int indexLength, int offsetLength) 
    {
        int offset = binaryToDec(bitaddress.substr(int_size - offsetLength, offsetLength));
        return offset;
    }
};
