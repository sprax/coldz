/*****************************************************************************
 sarnoff.h -- for RCA/Sarnoff code    S.R.Lines    93.5.23-8.24
 From c/rh/src/imghdr.h, etc., with names changed to protect the guilty.

#include "sarnoff/imghdr.h"
 *****************************************************************************/
#ifndef	rha_sarnoff_h__
#define	rha_sarnoff_h__

#include "sarnoff/imgdec.h"
#include "sarnoff/imgmacro.h"

extern int             Image_border;

#define SARN_DUNNO  0
#define SARN_BYTE   1
#define SARN_UBYTE  2
#define SARN_SHORT  3
#define SARN_USHORT 4
#define SARN_LONG   5
#define SARN_FLOAT  6

#define IMAGE_MAGIC 0xffff      /* "valid image header": never used! */
#define SARN_RESERVED_LENGTH 504 /* 512 - size of used up fields */

typedef struct sarnimg_header_s {
  unsigned short width;			/* width of image */
  unsigned short height;		/* height of image */
  short type;				/* type of image */
  unsigned short depth;			/* depth of image (sequences) */
  char reserved[SARN_RESERVED_LENGTH];	/* user specific field */
} sarnimg_header;			/* Sarnoff names this IMAGE_HEADER */

#define LAPLACIAN 1

#endif	/* rha_sarnoff_h__: eof */


