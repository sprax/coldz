/* From libgutil(?): vcolor.c */

long rgb(float fr, float fg, float fb);
long rgbpack(unsigned long c);
long rgb3f(float c[3 ]);
long rgbi(int r, int g, int b);
long hsv(float h, float s, float v);
long hsvi(int h, int s, int v);
long grey(float fg);
long greyi(int g);
void	cpacktovect(unsigned long c, vect *v);
void	usecolor(long c);
void	pseudorgb(void);
void	colorinit(void);


