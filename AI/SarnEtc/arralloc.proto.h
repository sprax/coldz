

void **_alloc_array(int h, int w, int size);
void **alloc_array(int lh, int uh, int lw, int uw, int size);
void *_alloc_vector(int dim, int size);
void *alloc_vector(int l, int h, int size);
void _free_array(char **ptr, int size);
void free_array(char **ptr, int lh, int uh, int lw, int uw, int size);
void _free_vect(char *ptr, int size);
void free_vect(char *ptr, int l, int h, int size);


