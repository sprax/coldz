/*****************************************************************************
 *
 * image structure
 */

#ifndef	_libvision_image_h
#define	_libvision_image_h

#include <sys/types.h>

/*
 * image types
 */

#define IMG_1BIT		0
#define IMG_U8BIT		1
#define IMG_S8BIT		2
#define IMG_S32BIT		3
#define IMG_FLOAT		4

/*
 * boundary connditions for images
 */

#define BOUNDARY_REFLECT			0
#define BOUNDARY_WRAP				1

/*
 * basic image representation
 */

typedef struct image {
  int width, height;
  int type;			/* one of the above types */
  int bytes_per_line;		/* accelerator to next line, lines padded to 32 bits */
  char **image;			/* points to an array of row pointers */
  char *mem;			/* pointer to raw memory block */
  void *extra1, *extra2;	/* extra vars for non library routines */
} image_t;

/* for accessing binary images
 */

#define getBit(line,col)	(((*(line+((col)>>3)))>>(7-((col)&7)))&1)
#define setBit(line,col)	*(line+((col)>>3)) = *(line+((col)>>3))|(1<<(7-((col)&7)))
#define clrBit(line,col)	*(line+((col)>>3)) = *(line+((col)>>3))&~(1<<(7-((col)&7)))

/* function declarations
 */

image_t *allocate_image();
image_t *allocate_image_like();
image_t *copy_image();
image_t *read_image();
image_t *read_grok();
image_t *read_dump();
image_t *read_raw();
image_t *read_rast();
image_t *read_vis();
image_t *read_jlm();
image_t *extract_center();

char *read_image_dir();

double image_percentile();

#endif	/* !_libvision_image_h */

