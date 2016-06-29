#include <iostream>
#include <cstdlib>
#include "stdlib.h"
#include <bitset>
#include "math.h"

using namespace std;
#include <typeinfo>

int main() {
	
	
	int fptr = 1040273694;
	int rptr =-1726662223;
	int val = fptr += rptr;
	std::string binary = std::bitset<32>(2147483647).to_string();
	unsigned long decimal = std::bitset<32>(binary).to_ulong();
	cout << binary;
	/*
	string binary1="01111011000011011111111011001100";
	string binary2="01100011111100000100110101100000";
	
	int32_t decimal1 = std::bitset<32>(binary1).to_ulong();
	int32_t decimal2 = std::bitset<32>(binary2).to_ulong();
	int32_t decimal3 = decimal1 + decimal2;
	cout << ((decimal3 >> 1) & 0x7fffffff);*/

}
/*
 * 11010111000101101000101011001111
 * 11101011100010110100010101100111
 * 01101011100010110100010101100111
 */ 
