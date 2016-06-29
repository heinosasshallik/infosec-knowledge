//This is to figure out how exactly this algo works.


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <bitset>

#define	TYPE_3		3
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

using namespace std;

int32_t states[62];
int counter = 0;

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
      states[counter] = val;
      counter++;
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

int64_t getRangedOutput(int64_t __n, int64_t __min, int64_t __max, int32_t __tmax) {
	return     __min + (int64_t) ((double) ( (double)__max - __min + 1.0) * ((__n) / (__tmax + 1.0)));
}

int main() {
	long min = 0, max = 100;
	int32_t tmax = 2147483647;
	int64_t outputs[62];
	
	cout << getRangedOutput(__random2, min, max, tmax);
}
