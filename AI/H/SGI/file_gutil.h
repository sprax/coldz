
/* file.c */
int sizeoffile(FILE *f);
unsigned char *charfiledata(FILE *f);
int fcopy(FILE *inf, FILE *outf);
char *basename(char *str);
int readfile(FILE *inf, char *buf);
int isfile(char *name);
int findname(char *name, char *expname, char *domain);
int findexec(char *name, char *expname, char *domain);
FILE *gfxopen(char *name, char *mode);
int findfile(char *name);
int isimagefile(char *name);
int readrestab(void);
int findres(char *name);
FILE *res_fopen(char *name, char *mode);
int res_fread(char *buf, int size, int n, FILE *inf);
char *res_fgets(char *buf, int size, FILE *inf);
int res_fclose(FILE *inf);
int res_fseek(FILE *f, int offset, int whence);
int res_ftell(FILE *f);

