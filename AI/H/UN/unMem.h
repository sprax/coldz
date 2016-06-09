/*****************************************************************************
 unMem.h -- for libun.a clients.    S.R.Lines (spraxlo@ai)    93.5.13-6.27
 *****************************************************************************/
#ifndef unMem_h__
#define unMem_h__

#ifdef	MAC
#include	<size_t.h>
#else
#include	<stddef.h>
#endif
#ifdef	SUN
#include	<stdio.h>       /* Sun's loser assert.h needs stdio.h */
#endif

#include	<assert.h>


/*
extern void *memset(void *s, int c, size_t n);
*/


#define mallocAss(dst, type, count)  \
                 {dst=(type*)malloc((size_t)(count)*sizeof(type));\
                  assert(dst != (type*)NULL);}
#define callocAss(dst, type, count)  \
                 {dst=(type*)calloc((size_t)count,sizeof(type));\
                  assert(dst != (type*)NULL);}
#define reallocAss(dst, src, type, count)  \
                 {dst=(type*)realloc(src,(size_t)count*sizeof(type));\
                  assert(dst != (type*)NULL);}

#define BCOPY(dst,src,size) 	memcpy(dst,src,size)
#ifndef NeXT
void	bcopy(void *dst, void *src, size_t size);
#endif

#define BZERO(ptr,size) 	memset(ptr,0,size)
#ifndef NeXT
void	bzero(void *ptr, size_t size);
#endif

#define BSET(dst,val,size) 	memset(dst,val,size)

#if	defined (WIN32) || defined(MAC) || defined(NeXT)
void	bset(void *dst, int val, size_t size);
#else
#include <unistd.h> /* extern	void bset(char *, bitnum_t); */
#endif


#ifndef _NO_PROTO

void *recalloc (void* oldptr, size_t count, size_t eltsize);

#else

void *recalloc ();

#endif

#endif	/* unMem_h__ */


