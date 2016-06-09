
/* filtio.proto.h */

int
getpyrfilters(char *filename, FILTER *rf, FILTER *ef);
int
getqmffilters(char *filename, FILTER *rlf, FILTER *rhf, FILTER *elf, FILTER *ehf);
int
get_1d_filter(char *filename, FILT1D *f);
int
get_1d_sym_filter(char *filename, FILTER *f);
int
get_2d_filter(char *filename, FILT2D *f);
int
read_1d_filter(FILT1D *f, FILE *stream);
int
read_1d_sym_filter(FILTER *f, FILE *stream);
int
read_2d_filter(FILT2D *f, FILE *stream);



