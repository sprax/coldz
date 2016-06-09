/**************************************************************************
 imghdr.h -- Sarnoff Image I/O and header declarations (cf. sarnpix.h)
 **************************************************************************/
#ifndef sarnoff_imghdr_h_
#define sarnoff_imghdr_h_
#define IMAGE_MAGIC 0xffff	/* valid image header: never used! */

#define DUNNO  0
#define BYTE   1
#define UBYTE  2
#define SHORT  3
#define USHORT 4
#define LONG   5
#define FLOAT  6

#define RESERVED_LENGTH 504	/* 512 - size of used up fields */

typedef struct {
  unsigned short width;			/* width of image */
  unsigned short height;		/* height of image */
  short          type;    		/* type of image */
  unsigned short depth;			/* depth of image (sequences) */
  char reserved[RESERVED_LENGTH];	/* user specific field */
} IMAGE_HEADER;

#endif /* sarnoff_imghdr_h_ */

