#ifndef beymer_macros_h_
#define beymer_macros_h_


#ifndef Max
#define Max(x,y)      (((x) > (y)) ? (x) : (y))
#define Min(x,y)      (((x) < (y)) ? (x) : (y))
#endif
#ifndef Abs
#define Abs(x)        (((x) < 0) ? (-(x)) : (x))
#endif

#define Sign(x)	      (((x) < 0) ? -1 : 1)


/* For accessing binary images: these could be optimized -- see blend2images.c/h */

#define getBit(line,col) (((*(line+((col)>>3)))>>(7-((col)&7)))&1)
#define setBit(line,col) \
	*(line+((col)>>3)) = *(line+((col)>>3))| (1<<(7-((col)&7)))
#define clrBit(line,col) \
	*(line+((col)>>3)) = *(line+((col)>>3))& ~(1<<(7-((col)&7)))

#endif	/* beymer_macros_h_ */


