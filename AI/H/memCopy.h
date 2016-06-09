/*****************************************************************************
memCopy.h    S.R.Lines (spraxlo@ai)    93.6.12-7.4
*****************************************************************************/
#ifndef im_memCopy_h_
#define im_memCopy_h_

/*
#include <unTypes.h>
#include <size_t.h>
*/

void memCopy(void *dst, void *src, size_t count, unt dmmss, unt smmss);
void memCopyToUnc(void *dst, void *src, register size_t count, unt srctype);
void memCopyToFlt(void *dst, void *src, register size_t count, unt srctype);

#endif	/* im_memCopy_h_ */

