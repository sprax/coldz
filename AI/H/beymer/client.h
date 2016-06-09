/* 
 * Simple interface for xtube clients.
 */


/* Define some colors to do grahics with
 *
 * Also defined in xtube.c 
 */

#define XTUBE_RED     120
#define XTUBE_GREEN   121
#define XTUBE_BLUE    122
#define XTUBE_MAGENTA 123
#define XTUBE_YELLOW  124
#define XTUBE_CYAN    125
#define XTUBE_BLACK   126
#define XTUBE_WHITE   127





/* client.c */

#ifdef _NO_PROTO

int xtube_setup_client();
int xtube_shutdown_client();
int xtube_draw_line(/*  x1,  y1,  x2,  y2,  color */);
int xtube_draw_point(/*  x1,  y1,  color */);
int xtube_draw_string(/* x, y, char *string, color */);
int xtube_draw_image(/* width, height, bytes-per-line, unsigned char *data */);
int xtube_flush();

#else

int xtube_setup_client(void);
int xtube_shutdown_client(void);
int xtube_set_mag(int mag);
int xtube_draw_line(int x1, int y1, int x2, int y2, int color);
int xtube_draw_point(int x1, int y1, int color);
int xtube_draw_string(int x, int y, char *string, int color);
int xtube_draw_image(image_t *image, int x, int y);
int xtube_draw_image2(image_t *image, int x, int y);
int draw_image_no_translate(int width, int height, int bytes_per_line, unsigned char *data, int start_x, int start_y);
int xtube_clear_area(int x, int y, int width, int height);
int xtube_flush(void);
int read_wid_from_file(void);

#endif

