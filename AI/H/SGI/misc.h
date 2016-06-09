
/* misc.h */
void	strtolower(char *str);
void	ctos(unsigned char *cptr, unsigned short *sptr, int n);
void	stoc(unsigned short *sptr, unsigned char *cptr, int n);
void	delay(float secs);
unsigned long getltime(void);
unsigned long waittill(unsigned long t);
float	uptime(void);
float	gettime(void);

void	cleartime(void);
void	timefunc(int (*f )(), int n);
void	tpercentfile(FILE *f);
void	tpercentdone(float p);
void	mallocnoise(int n);
void	malloclimit(int n);
unsigned char *mymalloc(int n);


