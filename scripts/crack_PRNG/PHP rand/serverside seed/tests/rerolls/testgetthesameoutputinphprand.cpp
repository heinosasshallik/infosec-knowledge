
//successful. The output is the same as in php.
//Add -std=c++11 or -std=gnu++11 compiler options if you want cstdlib. 

#include "stdlib.h"
#include <iostream>
//#include <cstdint>

using namespace std;

int main() {
	
	uint64_t seed = 1337;
	srandom((unsigned int)seed);
	cout << random() << endl; 
	return 0;
}
