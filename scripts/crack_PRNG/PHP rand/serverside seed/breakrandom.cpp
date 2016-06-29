//This generates the values of the PRNG state based on the outputs


//Start of glib rand().

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <bitset>

#define	TYPE_3		3
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

using namespace std;

int
__random_r2 (struct random_data *buf, int32_t *result)
{
  int32_t *state;

  if (buf == NULL || result == NULL)
    goto fail;

  state = buf->state;
	
	//We're type 3
  if (true)
    {
      int32_t *fptr = buf->fptr;
      int32_t *rptr = buf->rptr;
      int32_t *end_ptr = buf->end_ptr;
      int32_t val;
	  
	  //val = *fptr; *fptr = *fptr + *rptr
	  /* Keep this in mind:
	   * int1 = pow(2,32)-1;
	   * int2 = int1 + 1;
	   * int1 == 2147483647 && int2 == -2147483648
	   */
      val = (*fptr += *rptr); //this is how fptr changes and the sequence doesn't repeat.
      //cout << counter << ": " <<  std::bitset<32>(val).to_string() << endl;
      /* 
       * Chucking least random bit.
       * two rght signs is the logical right bitshift, meaning the sign is preserved.
       * then the sign is taken off with the mask.
	   */ 
      *result = (val >> 1) & 0x7fffffff;
      ++fptr;
      if (fptr >= end_ptr)
	{
	  fptr = state;
	  ++rptr;
	}
      else
	{
	  ++rptr;
	  if (rptr >= end_ptr)
	    rptr = state;
	}
      buf->fptr = fptr;
      buf->rptr = rptr;
    }
  return 0;

 fail:
  
  return -1;
}


struct random_data2
  {
    int32_t *fptr;		/* Front pointer.  */
    int32_t *rptr;		/* Rear pointer.  */
    int32_t *state;		/* Array of state values.  */
    int rand_type;		/* Type of random number generator.  */
    int rand_deg;		/* Degree of random number generator.  */
    int rand_sep;		/* Distance between front and rear.  */
    int32_t *end_ptr;		/* Pointer behind state table.  */
  };

static int32_t randtbl[DEG_3 + 1] =
  {
    TYPE_3,
	 //STATE BEGINS HERE
    -1726662223, 379960547, 1735697613, 1040273694, 1313901226,
    1627687941, -179304937, -2073333483, 1780058412, -1989503057,
    -615974602, 344556628, 939512070, -1249116260, 1507946756,
    -812545463, 154635395, 1388815473, -1926676823, 525320961,
    -1009028674, 968117788, -123449607, 1284210865, 435012392,
    -2017506339, -911064859, -370259173, 1132637927, 1398500161,
    -205601318,
  };

static struct random_data unsafe_state2 =
{
    .fptr = &randtbl[SEP_3 + 1],
    .rptr = &randtbl[1],

    .state = &randtbl[1],

    .rand_type = TYPE_3,
    .rand_deg = DEG_3,
    .rand_sep = SEP_3,

    .end_ptr = &randtbl[sizeof (randtbl) / sizeof (randtbl[0])]
};

long int
__random2 (void)
{
  int32_t retval;

  (void) __random_r2 (&unsafe_state2, &retval);

  return retval;
}

//Start of breaker

/*
 * Function prototypes
*/
void generateOutputs(int32_t* outputs, int32_t* shiftedoutputs, int32_t* knownoutputs);
void firstTrick(int32_t* outputs, int32_t* shiftedoutputs, int32_t* knownoutputs);
void secondTrick(int32_t* shiftedoutputs, int32_t* knownoutputs);
void addValues(int32_t* knownoutputs);
void displayNumbers(int32_t* shiftedoutputs, int32_t* knownoutputs);
void checkOutputValidity(int32_t* knownoutputs);
/*
 * Global variables
*/
//string mode = "least outputs";
string mode = "least outputs";
int numberofoutputs = 31*3; //the number of outputs can be freely changed

	
int main() {
	int32_t outputs[numberofoutputs];
	int32_t shiftedoutputs[numberofoutputs];
	int32_t knownoutputs[numberofoutputs];
	
	//generates numberofoutputs outputs
	generateOutputs(outputs, shiftedoutputs, knownoutputs);
	
	//In this case, the two modes are pretty much just as efficient in terms of number of outputs needed
	//However, the least outputs mode might be useful with ranges.
	int numberofloops;
	if (mode == "least time") {
		numberofloops = 1;
	} else if (mode == "least outputs") {
		numberofloops = 10;
	}
	
	for (int counter = 0;  counter < numberofloops; counter++) {
		firstTrick(outputs, shiftedoutputs, knownoutputs);
	
		secondTrick(shiftedoutputs, knownoutputs);
	
		addValues(knownoutputs);
	}
	
	displayNumbers(shiftedoutputs, knownoutputs);
	
	checkOutputValidity(knownoutputs);
	
}

void generateOutputs(int32_t* outputs, int32_t* shiftedoutputs, int32_t* knownoutputs) {
	
	//generate outputs
	ifstream outputfile ("outputs.txt");
	for (int counter = 0; counter < numberofoutputs;counter++) {
		
		string line;
		//With our own implementation
		outputs[counter] = __random2();
		/*
		//From a file
		if (outputfile.is_open()) {
			getline(outputfile, line);
			outputs[counter] = atoi(line.c_str());
		}*/

		//shift the outputs to the left
		shiftedoutputs[counter] = outputs[counter] << 1;
		
		//Makes all knownoutputs 0 initially.
		/*
		 * Warning: 
		 * The shitty thing with this is that if an actual output turns out to be 0, then that's a bit bad
		 * but it prooooobably won't break everything, and the chances of that are super low.
		 * Thus, we won't bother fixing this
		*/
		knownoutputs[counter] = 0;
	}
	if (outputfile.is_open()) {
		outputfile.close();
	}
	
}

/*
 * First trick.
 * shiftedoutputs[3] == 0010 (last bit is lost in shiftedoutputs)
 * shiftedoutputs[0] == 0100
 * shiftedoutputs[3] == 1000 (the new one)
 * state[3] = state[0] + state[3];
 * This is only possible if state[3] == 0011 and state[0] == 0101.
 * From this we can conclude that state[3] == 1000, state[0] == 0101 and state[3] == 0011
*/
void firstTrick(int32_t* outputs, int32_t* shiftedoutputs, int32_t* knownoutputs) {
	for (int counter = 0; counter < numberofoutputs; counter++) {

		//First trick.
		if (counter > 31 && shiftedoutputs[counter-31] + shiftedoutputs[counter - 3] == shiftedoutputs[counter] - 2) {
			//cout << "\tFound! " << counter%31 << " and " << (counter-3)%31 << " end with ones.";
			knownoutputs[counter] = shiftedoutputs[counter];
			knownoutputs[counter-3] = shiftedoutputs[counter - 3] + 1;
			knownoutputs[counter-31] = shiftedoutputs[counter - 31] + 1;
		}
		
	}
}

/*
 * Second trick.
 * If a state ends with a 1-bit, we can conclude two other states. 
 * state[3] == 0010 or 0011
 * state[0] == 0101
 * state[3] == 0110 or 0111
 * We can clearly see, that the new state[3] has to be 0111 and the previous state[3] is 0010.
 * Otherwise, if the old state[3] were 0011, then the new state[3] would be 1000.
*/
void secondTrick(int32_t* shiftedoutputs, int32_t* knownoutputs) {
	for (int counter = 0; counter < numberofoutputs; counter++) {
		
		//Apply bitmask to see if last bit is 1.
		if ((knownoutputs[counter] & 0x1) == 1) {
			//In this case, index-31 ends with a 1, and we will attempt calculating index-3 and index.
			if (counter < numberofoutputs - 31) {
				//Check that the first trick doesn't apply
				if (shiftedoutputs[counter] + shiftedoutputs[counter+28] == shiftedoutputs[counter+31]) {
					//Make the conclusion
					knownoutputs[counter+28] = shiftedoutputs[counter+28];
					knownoutputs[counter+31] = shiftedoutputs[counter+31] + 1;
					
				}
			}
			//In this case, index-3 ends with a 1, and we will attempt calculating index-31 and index.
			if (counter < numberofoutputs - 3 && counter > 27) {
				//Check that the first trick doesn't apply
				if (shiftedoutputs[counter-28] + shiftedoutputs[counter] == shiftedoutputs[counter+3]) {
					//Make the conclusion					
					knownoutputs[counter-28] = shiftedoutputs[counter-28];
					knownoutputs[counter+3] = shiftedoutputs[counter+3] + 1;
					
				}
			}
		}
	}
}

/*
 * state[index] = state[index-31] + state[index-3]
 * If two of those are known, we can calculate the third.
*/
void addValues(int32_t* knownoutputs) {
	//Add the known values together
	for (int counter = 0; counter < numberofoutputs; counter++) {
		//if it reaches a known number state[counter], then if state[counter+28] is also known, we can calculate state[index+31]
		if (counter < numberofoutputs - 31 && knownoutputs[counter] != 0 && knownoutputs[counter+28] != 0 && knownoutputs[counter+31] == 0) {
			knownoutputs[counter+31] = knownoutputs[counter] + knownoutputs[counter+28];
		}
		//if state[counter + 3] and state[counter-28] is known, we can calculate it
		if (counter < numberofoutputs - 3 && counter >= 28 && knownoutputs[counter-28] != 0 && knownoutputs[counter+3] != 0 && knownoutputs[counter] == 0) {
			knownoutputs[counter] = knownoutputs[counter+3] - knownoutputs[counter-28];
		}
		//if state[counter] is not known, but state[counter+28] and state[counter+31] are, we can calculate state[counter]
		if (counter < numberofoutputs - 31 && knownoutputs[counter] == 0 && knownoutputs[counter+28] != 0 && knownoutputs[counter+31] != 0) {
			knownoutputs[counter] = knownoutputs[counter+31] - knownoutputs[counter+28]; 
		}
	}
}

/*
 * Displays shiftedoutputs in one column and knownoutputs in other column (in binary).
*/
void displayNumbers(int32_t* shiftedoutputs, int32_t* knownoutputs) {
	for (int counter = 0; counter < numberofoutputs; counter++) {
		if (counter % 31 == 0) {
			cout << "[+] Currently on round:" << counter/31 << endl;
		}
		cout << counter << ": " <<  std::bitset<32>(shiftedoutputs[counter]).to_string();
		if (knownoutputs[counter] != 0) {
			cout << "\t" << std::bitset<32>(knownoutputs[counter]).to_string();
		}
		cout << endl;
	}
}

//checks validity of outputs
void checkOutputValidity(int32_t* knownoutputs) {
	for (int counter = 0; counter < numberofoutputs; counter++) {
		if (counter < numberofoutputs-31 && knownoutputs[counter] != 0 && knownoutputs[counter+28] != 0 && knownoutputs[counter+31] != 0) {
			if (knownoutputs[counter] + knownoutputs[counter+28] != knownoutputs[counter+31]) {
				cout << "fail at: " << counter << endl;
			 }
		 }
	 }
}
