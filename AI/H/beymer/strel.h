#ifndef beymer_strel_h_ /***************************/
#define beymer_strel_h_

/* structuring elements */

typedef struct strel {
  int nx, ny;
  int cx, cy;
  unsigned char **data;
} strel_t;


/* From strel.c */
strel_t *box_strel(int nx, int ny, int cx, int cy);
strel_t *circular_strel(int radius);
strel_t *cross_strel(int half_size);


#endif /* beymer_strel_h_ **************************/

