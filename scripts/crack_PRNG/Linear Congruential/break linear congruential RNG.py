#Tutorial: https://jazzy.id.au/2010/09/20/cracking_random_number_generators_part_1.html
#Java Random() source code: http://developer.classpath.org/doc/java/util/Random-source.html

#-----------------------
#Linear congruential RNG
#-----------------------
#implementing the PRNG in Python might not have been the
#smartest idea, since Python automatically switches between
#32 bit and 64 bit ints, which other languages don't.
#the seed was a long in Java though, so maybe it's not a problem.

import math

seed = 6
modulus = 48 #seed's precision is 48 bits
RNGprecision = 32 #random number has a precision of 32 bits
multiplier = 25214903917
addition = 11

#bitshifting to the left 48 times is equivalent to multiplying by pow(2,48)
#one = 1L << 48
#print bin(one -1)
randomNumbers = []
seeds = []

for i in range(10):
	# '& (4-1)' is equal to '% 4'
	seed = (seed*multiplier + addition) & ((1L << modulus) -1)
	seeds.append(seed) 
	#we will now lose 16 bits of precision
	randomNumber = seed >> (modulus - RNGprecision)
	randomNumbers.append(randomNumber)
	
print "the random numbers are"
for i in range(10):
	print randomNumbers[i]
	
	
#----------------
#Breaking the RNG
#---------------
#We will have to bruteforce the 16 bits we lost.

num1 = randomNumbers[0]
num2 = randomNumbers[1]

bfseed = 0 #the seed we will bruteforce

print 'calculating original seed: ' 

for i in range(pow(2,16)):
	#just have to try out every single combination
	bfseed = (num1 << 16) + i
	#see if this gives us the correct next variable
	bfseed2 = (bfseed*multiplier + addition) & ((1L << modulus) -1)
	if (bfseed2 >> 16) == num2:
		print 'seed is ', bfseed
		break
		
#-----------------------
#Cracking previous seeds
#-----------------------
#Because the seed overflows and is then trimmed back down to
#48 bits, we can't just subtract the addition and divide by the multiplier

#tutorial here: https://jazzy.id.au/2010/09/21/cracking_random_number_generators_part_2.html
#Hard to do in Python because of how you can't manually 
#select what datatype your variable is. Refer to the Java one.


















		
