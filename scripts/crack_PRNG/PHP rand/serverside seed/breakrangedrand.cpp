/*	--DOESN'T WORK--
 * 
 * CONCERNS:
 * [+] if accuracy isn't all that important, you can just add together index-31 and index-3 to get index.
 * [+] When calculating the new minimum state based on the old ones, it's sometimes larger by two.
 * It's possible that this is normal, but just for the sake of safety, pretend like it's 0 (and thus 
 * includes the two). You won't lose much precision, and it'll be safer.
 * [+] When you want to type 3 billion, use LL. When doing calculations with stateRanges, cast as int32_t!!!!
 * [+] Keep in mind that outputs.txt shows line numbers from 1 to 62, you show indexes from 0 to 61. 
 * [+] WHEN SOMETHING DOESN'T WORK, CHECK THAT highestranged AND lowestranged ARE CORRECT!!!!!!
 * [+] maxState(10) + maxState(5) == maxState(16) + 1. IT IS INCORRECT BY ONE, even though maxState(10) and maxState(5) are correct individually. WTF
 * 
 * Jobs:
 * [+] index-31 and index-3 don't really give us any information about index. So, the answer may lie in 
 * 		doing something similar to the last one - use index to derive info about index-31 and index-3.
 * 		Therefore - finish the second trick in breakrandom and then use all the tricks in here.
 * [+]I wonder: is range(1,100) the same as range(0,99)? I think so. 
 * [+] Implement first trick, the sum trick and then the second trick.
 * [+] Implement accuracy increasing functionality in the loadOutputs function
 * 
 * PROBLEMS:
 * 
 * Pareto:
 * Implement first trick.
 *  	Derive the max and min ranges of the previous 2 states based on the new one. till 21:00. NVM, doesn't work
 */ 

#include <iostream>
#include "stdlib.h"
#include <fstream>
#include <vector>

using namespace std;


//The number of outputs we load from the outputs.txt file
int numberOfOutputsToLoad = 62;
//Highest outputtable number in rand(range)
int32_t highestRanged = 100;
//Lowest outputtable number in rand(range)
int32_t lowestRanged = 0;
//Highest 32 bit number
int32_t maximum32bit = 2147483647;
//Array containing the possible ranges of the last two sets of 31 states. [0] is minimum and [1] is maximum state.
//Since they're global variables, they're initialized to 0.
//They are long so that a number above pow(2,32) could be used to avoid having to use 0 for undefined numbers
//WHEN DOING CALCULATIONS, ALWAYS CAST IT AS int32_t! When typing 3billion, always add LL (long long) at the end!!
vector< vector<long long> > stateRanges(numberOfOutputsToLoad, vector<long long>(2));
//The index we're at in the stateRanges
int32_t index = 0;


//Function prototypes
int32_t getOriginalMinimumState(int32_t rangedoutput);
int32_t getOriginalMaximumState(int32_t rangedoutput);
int32_t getRangedOutput(int64_t originalState);
//makes use of index.
void loadOutputsFromFile(int numberOfOutputsToLoad, const char* fileToLoad);
//makes use of index.
void improveRangeAccuracy();

/*
 * Main
*/
int main(int argc, char* argv[]) {
	
	//Initalize all the values as undefined. Using 3,000,000,000 (3 billion) as that value
	for (int i = 0; i < numberOfOutputsToLoad; i++) {
		stateRanges[i][0] = 3000000000LL;
		stateRanges[i][1] = 3000000000LL;
	}
	//Load some outputs from the file. Be mindful of the number of outputs to load.
	loadOutputsFromFile(numberOfOutputsToLoad, "outputs.txt");
	
	
	
	return 0;
}

//Returns the minimum number the state, which corresponds to the ranged output, can be
int32_t getOriginalMinimumState(int32_t rangedoutput) {
	int32_t output_min = (double)(((double)rangedoutput - lowestRanged) / ((double)highestRanged - lowestRanged + 1.0) * (maximum32bit + 1.0)) + 1; //the +1 is needed because at the end of the answer is 0.9 and long rounds it down to 0.
	return output_min << 1;
}

//Returns the maximum number the state, which corresponds to the ranged output, can be
int32_t getOriginalMaximumState(int32_t rangedoutput) {
	int32_t output_max = (double)(((double)rangedoutput + 0.9999999999999999 - lowestRanged) / ((double)highestRanged - lowestRanged + 1.0) * (maximum32bit + 1.0));
	return (output_max << 1) + 1; //The +1 is there since the largest possible number ends with a 1-bit.
}


//uses the guessed state (so rightshift is necessary)
int32_t getRangedOutput(int64_t originalState) {
	int64_t originalOutput = (originalState >> 1) & 0x7fffffff;
	int64_t output_min = lowestRanged, output_max = highestRanged;
	int32_t tmax = maximum32bit;
	return output_min + (long) ((double) ( (double)output_max - output_min + 1.0) * ((originalOutput) / (tmax + 1.0))) ;
}

//This should actually also incorporate the range specification thing!
//Once it goes over 31 outputs, it's useless.
void loadOutputsFromFile(int numberOfOutputsToLoad, const char* fileToLoad) {
	ifstream outputfile (fileToLoad);
	for (index = 0; index < numberOfOutputsToLoad; index++) {
		if (index > 61) {
			index = 0;
		}
		string line;
		if (outputfile.is_open()) {
			getline(outputfile, line);
			
			if (line.empty()) {
				cout << "Error: Not enough numbers in output! Terminating Process." << endl;
				exit(1);
			}
			//convert string to int
			int number = atoi(line.c_str());
			//post min and max values into the stateRange
			stateRanges[index][0] = getOriginalMinimumState(number);
			stateRanges[index][1] = getOriginalMaximumState(number);
			//Increase accuracy of those min and max values if possible.
			//improveRangeAccuracy();

		}
	}
	if (outputfile.is_open()) {
		outputfile.close();
	}
}

/*
 * First trick.
 * state[3] = state[3] + state[0]
 * state[3] (the old one) = 1000...6000
 * state[0] = 7000...12000
 * state[3] (the new one) = 13000...18000
 * Wait, this is useless...
 * 13000 could be 12000 + 1000 or 7000 + 6000, doesn't tell us anything
*/
void firstTrick() {
	
}

//Based on index - 3 and index - 31. It looks at their minimum and maximum outputs
//if minimum + minimum is larger than our current one, then we update. If maximum + maximum is smaller than our current one, we update.
void improveRangeAccuracy() {
	//check that we even have those ranges that we're trying to derive info from
	//Checking for 3 billion, because that's the value for undefined in our program
	if (stateRanges[(index + 62 - 3) % 62][0] != 3000000000LL && stateRanges[(index + 62 - 31) % 62][0] != 3000000000LL
		&& stateRanges[(index + 62 - 3) % 62][1] != 3000000000LL && stateRanges[(index + 62 - 31) % 62][1] != 3000000000LL) {
		//REMEMBER TO CAST THE LONG LONG AS AN INT
		//The minimum and maximum possible values don't really give us any new information about the current one.
		//However, from the current one, we may be able to calculate the past values. Spoiler - we're not.
		//Just to repeat: these are useless.
		int32_t minimumPossibleValue = (int32_t)stateRanges[(index + 62 - 3) % 62][0] + (int32_t)stateRanges[(index + 62 - 31) % 62][0];
		int32_t maximumPossibleValue = (int32_t)stateRanges[(index + 62 - 3) % 62][1] + (int32_t)stateRanges[(index + 62 - 31) % 62][1];
		
		
		cout << index << "\t" <<  maximumPossibleValue << "\t" << stateRanges[index][1] << "\t" << maximumPossibleValue - stateRanges[index][1] << endl;// << "\t" <<  getRangedOutput(maximumPossibleValue - 1) << "\t" << getRangedOutput(stateRanges[index][1]) << endl;
		//cout << "\t" << getRangedOutput(stateRanges[(index + 62 - 3) % 62][1]) << "\t" << getRangedOutput(stateRanges[(index + 62 - 31) % 62][1]) << endl;
		
	}
}



