

/*
 * Tutorial: https://jazzy.id.au/2010/09/21/cracking_random_number_generators_part_2.html
 * The reason this works is because:
 *  12*?? = 420. The last number is 0, meaning it could only be 5, since 5*2 = (1)0
 *  12*?5 = 420. Now we can subtract the 5*12 to find out the next number.
 *  12*?0 = 360. Divide by 10.
 *  12*? = 36. The last number is 6, meaning the mystery number is 3, since 3*6 = (1)2
 *  
 * That's how you might do it in decimal, anyways. It's not that easy though.
 * There is a bitmask applied, and we need to do it in binary.
 * It goes the same way tho
 * result is 1 
*/
public class LinearCongruentialSeedReverser {
	public static void main(String[] args) {
		long multiplier = 0x5DEECE66DL; //multiplier needs to be odd for this to work, since the final number is 1. the tutorial mentions what to do when it's not odd, too.
		long addend = 0xBL;
		long currentSeed = 5667543;
		currentSeed = (currentSeed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
		
		long seed = currentSeed;
		// take off the addend
		seed -= addend;
		long result = 0;
		// find all 48 bits
		for (int i = 0; i < 48; i++) {
			long mask = 1L << i; //Extract the i+1th bit that you need. i = 2 (third bit from the right), mask = 0b100
			long bit = seed & mask; //mask = 0b100, seed = 1101, bit = 100 
			//add it to the result
			result |= bit; // 0000 | 100 = 0100
			//if the bit is 1, subtract its effect from the seed
			System.out.println(seed);
			System.out.println(mask);
			System.out.println(bit);
			if (bit == mask) {
				seed -= multiplier << i; //The reason that bitshift is there is that bigger and bigger numbers need to be subtracted as we move on. Think of long division. after you've written one line, you start the next line one space to the left. In decimal, that signifies a 10 times increase, in binary, it signifies a two-times increase. And a leftwise bit is a two times increase.
				System.out.println("ding");
			}
			
		}
		System.out.println("Previous seed: " + result);
	}
}
