/*****************************************************************************
 unMem.c -- (un)common memory funcs.  S.R.Lines (spraxlo@ai)   93.5.13-6.27
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "unLibPrivate.h"  /* unLib's internal switchboard */
#include "unMem.h"	   /* Macros and prototypes for libun.a client srcs */

void *recalloc (void* oldptr, size_t count, size_t eltsize)
{
	size_t size = count*eltsize;
	void *newptr;
	if (oldptr == NULL)
		newptr = calloc(count,eltsize);
	else {
		newptr = realloc(oldptr,size);
		if (newptr != NULL)
			memset(newptr,0,size);
	}
	return(newptr);
}



/* Old BSD routines for compatibility */

#ifdef	UN_BZERO
void bzero(char *ptr, size_t size)
{
  BZERO(ptr,size);
}
#endif


