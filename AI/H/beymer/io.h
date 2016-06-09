
/*
#include "io_sgi.h"
*/

/* From io.c */
int write_image(image_t *image, char *filename, int format);
image_t *read_image(char *filename);
int write_dump(image_t *image, char *filename);
image_t *read_dump(char *filename);
int write_vis(image_t *image, char *filename);
void write_vis_1bit(image_t *image, FILE *fout);
void write_vis_8bit(image_t *image, FILE *fout);
void write_vis_s32bit(image_t *image, FILE *fout);
void write_vis_float(image_t *image, FILE *fout);
image_t *read_vis(char *filename);
int read_vis_header(FILE *fp, int *width, int *height, int *type);
void read_vis_8bit(image_t *image, FILE *fp);
void read_vis_s32bit(image_t *image, FILE *fp);
void read_vis_float(image_t *image, FILE *fp);
int read_vis_really_a_binary_image(image_t *image);
image_t *read_vis_switch_to_binary_image(image_t *image);
int write_rast(image_t *image, char *filename, int color);
int write_rast_stream(image_t *image, FILE *fout, int color);
image_t *read_rast(char *filename);
image_t *read_grok(char *filename);
void read_grok_1bit(image_t *image, int width, int height, FILE *fp);
void read_grok_8bit(image_t *image, int width, int height, FILE *fp);
int write_grok(image_t *image, char *filename);
image_t *read_raw(char *filename, int width, int height);
int write_raw(image_t *image, char *filename);
int write_pix(image_t *image, char *filename);
image_t *read_pix(char *filename);
int write_postscript(image_t *image, char *filename, int landscape_p, int zoom);
int image_make_space(FILE *fout, int width, int height, int zoom, int landscape_p);
int write_ps_file_comments(FILE *fout, int left, int bottom, int right, int top);
int write_postscript_1bit(FILE *fout, image_t *image, int width, int height);
int write_postscript_u8bit(FILE *fout, image_t *image, int width, int height);
int write_portable(image_t *image, char *filename);
int write_pbm(image_t *image, char *filename);
int write_pgm(image_t *image, char *filename);
image_t *read_jlm(char *filename);
FILE *fopen_image(char *filename);
char *read_image_dir(void);
int write_4bytes(int word, FILE *fout);
int read_4bytes(FILE *fout);
int image_type_string(char *str, image_t *image);
int image_string_type(char *str);
int write_array_mathematica(int *array, int n, char *filename);

