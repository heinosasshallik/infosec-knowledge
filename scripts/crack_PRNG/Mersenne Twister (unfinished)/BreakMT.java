//Explanation of Mersenne Twister: https://www.reddit.com/r/learnprogramming/comments/44n7is/how_does_the_mersennes_twister_work/
//Breaking the Twister: https://jazzy.id.au/2010/09/22/cracking_random_number_generators_part_3.html

public class BreakMT {
	public static void main(String[] args) {
		//---Generate results from twister---
		long seed = 1337;
		int[] seedbuf = new int[2];
		seedbuf[0] = (int)seed;
		seedbuf[1] = (int)(seed >>> 32);
		MT19937 twister = new MT19937(seedbuf);
		
		//---Get the outputs and crack the states---
		int[] outputs = new int[624];
		for(int i = 0; i < 624; i++) {
			outputs[i] = twister.nextInt();
		}
		BreakMT breakmt = new BreakMT();
		int state = breakmt.obtainState(outputs[623]);
		
	}
	int obtainState(int output) {
		//This undoes the tempering.
		unBitshiftRightXor(output, 18);
		return output;
	}
	
	int unBitshiftRightXor(int value, int shift) {
		int index = 0;
		int result = 0;
		//Get $shift number of bits until we get all 32.
		while (index*shift < 32) {
			//Get the $shift bits.
			
			/*
			 * make a mask for the bits we're trying to get
			 *-1 is 11111111 in binary
			 * puts ½shift number of ones in the front, then shifts them to the right as needed.
			*/ 
			int partMask = (-1 << (32 - shift)) >>> index*shift;
			//obtain the part we will xor the value with.
			int part = value & partMask;
			//xor and get the bits
			//Figure this part out.
			value = value ^ part >>> shift; //bitshift happens before xor. 
			//add the part to the result
			result |= part;
			index++;
		}
		return result;
	}
	
	int unBitshiftLeftXor(int value, int shift, int mask) {
		// we part of the value we are up to (with a width of shift bits)
		int i = 0;
		// we accumulate the result here
		int result = 0;
		// iterate until we've done the full 32 bits
		while (i * shift < 32) {
			// create a mask for this part
			int partMask = (-1 >>> (32 - shift)) << (shift * i);
			// obtain the part
			int part = value & partMask;	
			// unapply the xor from the next part of the integer
			value ^= (part << shift) & mask;
			// add the part to the result
			result |= part;
			i++;	
		}
		return result;
	}
	
}

class MT19937 {
	
	int index;
	int[] state;
	
	
	public MT19937(int[] seedbuf) {
		
		this.index = 624;
		this.state = new int[624];
		this.state[0] = 19650218; //initialization seed
		//initialize it with magic seed
		for (int i = 1; i < 624; i++) {
			this.state[i] = (1812433253 * (state[i-1] ^ (state[i-1] >>> 30)) + i);
		}
		//set initial seed
		setSeed(seedbuf);
	}
	
	void setSeed(int[] seedbuf) {
		int numOfOutputs = 624; // number of values you get from twister
		int counter = 1, seedbufIndex = 0, outputCounter = numOfOutputs;
		int length = seedbuf.length; //always 2
		//System.out.println("startmersenne");
		// see nõudis päris palju jamamist. seedid olid erinevad, algoritmis oli viga, algust ei näinud sest liiga palju ridasid tuli terminalisse, fuck
		for (; outputCounter > 0; outputCounter--) { //runs through 624 values
			//System.out.println(state[counter-1]);System.out.print(" ");System.out.print(counter);System.out.print("\n");
			state[counter] = (state[counter] ^ ((state[counter-1] ^ (state[counter-1] >>> 30)) * 1664525)) + seedbuf[seedbufIndex] + seedbufIndex;
			//System.out.println(state[counter]);
			//System.out.print((state[counter] ^ ((state[counter-1] ^ (state[counter-1] >>> 30)) * 1664525))); System.out.print(" ");System.out.print(seedbuf[seedbufIndex] + seedbufIndex);System.out.print(" ");System.out.print(state[counter]);System.out.print("\n");
			//mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >>> 30)) * MAGIC_FACTOR2)) + buf[j] + j;
			counter++; seedbufIndex++;
			if (counter >= numOfOutputs) { //if counter reaches 624, it resets to 1 and the first state becomes the previous last state. This probably happens once the for-cycle is in its final cycle. Needs to be tested.
				state[0] = state[numOfOutputs - 1];
				counter = 1;
				//System.out.println("counter 624");
				//System.out.println(outputCounter);
				//System.out.println(state[0]);
				//System.out.println();
			}
			if (seedbufIndex >= length) seedbufIndex = 0; //makes the seedbufIndex cycle between 0 and 1.
		}
		for (outputCounter = numOfOutputs - 1; outputCounter > 0; outputCounter--) {
			state[counter] = (state[counter] ^ ((state[counter-1] ^ (state[counter-1] >>> 30)) * 1566083941)) - counter;
			//System.out.println(state[counter]);
			counter++;
			if(counter >= numOfOutputs) {
				state[0] = state[numOfOutputs-1]; 
				counter=1; 
			}
		}
		
		//the comment says "MSB is 1; assuring non-zero initial array", but I don't get it. 
		//If MSB is 1, the initial number IS negative, and also
		//he's not applying the mask via an operator, he's just assigning the mask to state[0]
		//This means that state[0] will always be (-1)*pow(2,31)
		state[0] = 0x80000000;
	}
	
	int nextInt() {
		return next(32);
	}
	
	int next(int bits) {
		int counter, nextnum;
		int MAGIC[] = { 0x0, 0x9908b0df };
		//Generate 624 words at a time. Index is 624 when it hits this the first time.
		// in both for statements, the state is xor'd with the 397th next number.
		if (index >= 624) {
			for(counter = 0; counter < 227; counter++) {
				nextnum = (state[counter] & 0x80000000) | (state[counter+1] & 0x7fffffff); // takes the MSB from state[counter] and LSB from state[counter+1]
				state[counter] = state[counter + 397] ^ (nextnum >>> 1) ^ MAGIC[nextnum & 0x1];
				
			}
			for (;counter < 623; counter++) {
				nextnum = (state[counter] & 0x80000000) | (state[counter+1] & 0x7fffffff);
				state[counter] = state[counter+(397-624)] ^ (nextnum >>> 1) ^ MAGIC[nextnum & 0x1]; // the order in which things are xor'd has no significance. The last one means that if the number is odd (LSB is 1), then it xors with magic number.
				//System.out.println(state[counter]);
			}
			nextnum = (state[623] & 0x80000000) | (state[0] & 0x7fffffff);
			state[623] = state[396] ^ (nextnum >>> 1) ^ MAGIC[nextnum & 0x1];
			//System.out.println(state[623]);
			index = 0;
		}
		//returns state[index], then does index++
		nextnum = state[index++];
		// Tempering
		nextnum ^= (nextnum >>> 11);
		nextnum ^= (nextnum << 7) & 0x9d2c5680;
		nextnum ^= (nextnum << 15) & 0xefc60000;
		nextnum ^= (nextnum >>> 18);
		
		return (nextnum >>> (32-bits));
	}

 }
