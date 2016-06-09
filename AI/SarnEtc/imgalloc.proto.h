
/* image allocation routines */

void **_alloc_image(int w, int h, int size);
void **alloc_image(int w, int h, int size, int border);
void _free_image(char **ptr, int size);
void free_image(char **ptr, int size, int border);


