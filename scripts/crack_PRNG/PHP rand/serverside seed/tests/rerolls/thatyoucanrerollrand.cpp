//Successful. We get the same results

//------------------------------------------
//Defines:
/* Linear congruential.  */
#define	TYPE_0		0
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0

/* x**7 + x**3 + 1.  */
#define	TYPE_1		1
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3

/* x**15 + x + 1.  */
#define	TYPE_2		2
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1

/* x**31 + x**3 + 1.  */
#define	TYPE_3		3
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

/* x**63 + x + 1.  */
#define	TYPE_4		4
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1
//-------------------------------------------------------

/*
 * Gameplan:
 * Confirm that by implementing everything again, you get the same outputs
 * Confirm that this is not the thing PHP uses (probs uses ZTS)
*/

#include <iostream>
#include "stdlib.h"

using namespace std;

static int32_t randtbl2[DEG_3 + 1] =
  {
    TYPE_3,

    -1726662223, 379960547, 1735697613, 1040273694, 1313901226,
    1627687941, -179304937, -2073333483, 1780058412, -1989503057,
    -615974602, 344556628, 939512070, -1249116260, 1507946756,
    -812545463, 154635395, 1388815473, -1926676823, 525320961,
    -1009028674, 968117788, -123449607, 1284210865, 435012392,
    -2017506339, -911064859, -370259173, 1132637927, 1398500161,
    -205601318,
  };

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

static struct random_data2 unsafe_state =
  {
/* FPTR and RPTR are two pointers into the state info, a front and a rear
   pointer.  These two pointers are always rand_sep places apart, as they
   cycle through the state information.  (Yes, this does mean we could get
   away with just one pointer, but the code for random is more efficient
   this way).  The pointers are left positioned as they would be from the call:
	initstate(1, randtbl, 128);
   (The position of the rear pointer, rptr, is really 0 (as explained above
   in the initialization of randtbl) because the state table pointer is set
   to point to randtbl[1] (as explained below).)  */

    .fptr = &randtbl2[SEP_3 + 1],
    .rptr = &randtbl2[1],

/* The following things are the pointer to the state information table,
   the type of the current generator, the degree of the current polynomial
   being used, and the separation between the two pointers.
   Note that for efficiency of random, we remember the first location of
   the state information, not the zeroth.  Hence it is valid to access
   state[-1], which is used to store the type of the R.N.G.
   Also, we remember the last location, since this is more efficient than
   indexing every time to find the address of the last element to see if
   the front and rear pointers have wrapped.  */

    .state = &randtbl2[1],

    .rand_type = TYPE_3,
    .rand_deg = DEG_3,
    .rand_sep = SEP_3,

    .end_ptr = &randtbl2[sizeof (randtbl2) / sizeof (randtbl2[0])]
};

int
__random_r2 (struct random_data2 *buf, int32_t *result)
{
  int32_t *state;

  if (buf == NULL || result == NULL)
    goto fail;

  state = buf->state;
  
  
  //It's TYPE_3
  if (buf->rand_type == TYPE_0)
    {
      int32_t val = state[0];
      val = ((state[0] * 1103515245) + 12345) & 0x7fffffff;
      state[0] = val;
      *result = val;
    }
  else
    {
      int32_t *fptr = buf->fptr;
      int32_t *rptr = buf->rptr;
      int32_t *end_ptr = buf->end_ptr;
      int32_t val;

      val = *fptr += *rptr;
      /* Chucking least random bit.  */
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
  //__set_errno (EINVAL);
  return -1;
}

long int
__random2 (void)
{
  int32_t retval;
  (void) __random_r2 (&unsafe_state, &retval);
  return retval;
}

int
rand2 (void)
{
  return (int) __random2 ();
}

int main() {
	srandom(1337);
	cout << rand() << endl;
	srand(1337);
	cout << rand() << endl;
	//cout << rand2();
}
