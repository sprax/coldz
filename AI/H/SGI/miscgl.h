/* From miscgl.c */

float fgetmousex(void);
float fgetmousey(void);
int 	redraw(int (*f )());
int 	shiftdown(void);
int 	optiondown(void);
int 	spacedown(void);
long	getmousex(void);
long	getmousey(void);
void	drawfunc(int (*f )());
void	deltawin(int x, int y);
void	qmouse(void);
void	printsize(void);
void	printorigin(void);
void	subviewport(long left, long right, long bottom, long top);
void	subport(float left, float right, float bottom, float top);
void	fillrect(float x1, float y1, float x2, float y2);
void	drawrect(float x1, float y1, float x2, float y2);
void	fillrecti(int x1, int y1, int x2, int y2);
void	drawrecti(int x1, int y1, int x2, int y2);
void	fillcirc(float x, float y, float rad);
void	drawcirc(float x, float y, float rad);
void	circsides(int n);


