#define ALIVE       01
#define DEAD        00
#define DEFAULT     00

#define DRAW        100
#define DO          101
#define MAKE        102
#define EORDRAW     712

#define PICK        150
#define TRAP        151

#define JUSTDOWN    1
#define JUSTUP      2
#define BEENDOWN    4
#define BEENUP      8
#define ECHO        204
#define NOECHO      205

#define INT         0
#undef  FLOAT            /* FLOAT is defined as 5 in <sys/param.h>, included by <limits.h> -srl */
#define FLOAT       1
#define CHAR        2

#define OPENED      1
#define CLOSED      0

#define HORIZONTAL  0
#define VERTICAL    1

#define START  0
#define END    1

#define UP     0
#define DOWN   1
#define LEFT   2
#define RIGHT  3

#define NONE        0
#define VDARK       51     /* WHY IS THIS 30?  SHOULDN'T IT BE 51? Changed by Bob Sept 16 '91 */
#define DARK        102
#define LIGHT       153
#define VLIGHT      204
#define FULL        255

#define TOGGLE  1201
#define TURN_ON 1202
#define TURN_OFF 1203
#define ON 1
#define OFF 0

#define UP_AND_LEFT    0   /* SL  11/9/91  */
#define UP_AND_RIGHT   1
#define DOWN_AND_RIGHT 2
#define DOWN_AND_LEFT  3


#define SWITCHTOMARKER 1102
#define SWITCHTOPENCIL 1107
#define SAVE 1108
#define LOAD 1109

#define EXIT 1112
#define CLEAR 1113
#define OPEN 1114
#define MAKEWINDOW 1120
#define PALLETTE 1121


#define PENCIL 0
#define MARKER 1
#define MINIMAL 0
#define MAXIMAL 1

#define MOVEWIN 1130
#define FILTER 1131
#define BEGIN 2
#define BOTH 3


/* Messages for window return values */

#define DROP 1
#define STOP -999    

