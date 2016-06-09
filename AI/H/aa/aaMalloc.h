/****** aaMalloc.h -- prototypes & macros for functions in aaMalloc.c.
        S.R. Lines  (srl)
        8.10.92  (Orig. 9.92) *********/

#ifndef _aaMalloc_h_   /* Should be included after aa.h */
#define _aaMalloc_h_

#ifndef  _NO_PROTO

extern  void *recalloc (void* oldptr, size_t count, size_t eltsize);
extern  void malloc_fail (char *str, size_t size);
extern  void malloc_exit (char *str, size_t size);

#else   /* ifndef _NO_PROTO */


#endif  /* _NO_PROTO */

#endif  /* _aaMalloc_h_ */


