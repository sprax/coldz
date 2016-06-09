
/* farralloc.c */
float **_alloc_farray(int h, int w);
float **alloc_farray(int lh, int uh, int lw, int uw);
float *_alloc_fvector(int dim);
float *alloc_fvector(int l, int h);
int _free_farray(float **ptr);
int free_farray(float **ptr, int lh, int uh, int lw, int uw);
int _free_fvector(float *ptr);
int free_fvector(float *ptr, int l, int h);


