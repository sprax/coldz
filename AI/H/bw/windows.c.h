#ifndef __Ohmygosh_WINDOWS_C_H_
#define __Ohmygosh_WINDOWS_C_H_
#ifndef   MAC
#include <malloc.h>
#endif
#include "messages.h"

#ifndef MAX
#define MAX(A,B)  ((A) > (B) ? (A) : (B))
#ifndef MIN
#endif
#define MIN(A,B)  ((A) < (B) ? (A) : (B))
#endif
#define UpdateSizes(W) TransformCoords(W,W->relx,W->rely,W->width,W->height);


/***  These are the structures for output and input devices  ***/

struct OutDev {
  int fildes,bits;             /***  NEW BITS PER PIXEL  ***/
  int width,height;            /*** size of frame buffer ***/
  int viswidth,visheight;      /***    visible width     ***/
  unsigned char *buffer;
  unsigned char *control;
};

struct InDev {
  int fildes;
  int width,height;
  int buttons;
};


/***  WINDOW STRUCTURES  ***/

struct Window {
  int stat;                      /*   Is the window installed? (not if it's open) */
  int relx, rely;                /*   position of window within parent            */
  int width, height;             /*   size of window in pixels                    */
  int x1, y1, x2, y2;            /*   window's actual screen coordinates          */
  struct OutDev *display;        /*   screen file descriptor                      */
  struct InDev *input;           /*   mouse or tablet file descriptor             */
  struct List *mask;             /*   list of overlapping windows                 */
  struct Window *parent;         /*   parent window                               */
  struct List *port;             /*   pointer to children windows                 */
  struct Command *commands;      /*   window functions                            */
  char *tag;                     /*   identifying string                          */
};

struct Command {
  int id, numfuncs;              /* integer message id */
  int (**functions)();
  char **arguments; 
  struct Command *next;          /* pointer to next command */
};

struct Stencil {
  struct Window *id; 
  int x1, y1, x2, y2;
  struct Stencil *Behind;
  struct Stencil *Front;
};

struct List {
  struct Window *id;
  struct List *Behind, *Front;
};

struct messsage {
  int id;
  char *data;
};

struct loopstruct {
  int button, trapstat;
  struct Window *Last, *trap;
};


/*** PARAMETER STRUCTURES FOR WINDOWS ***/

struct pickstruct {
  int x, y, button, bnum, character;
  float p;
};

struct trapstruct {
  struct Window *window;
  int until;
};

struct clipstruct {
  int x1, y1, x2, y2;
};

struct Thing {
  struct Thing *Front, *Behind;
  char *id;
};

struct Window *MakeWindow();
struct Command *GetCommand();
struct Thing *MakeThingList(), *AskThing();
char *CopyString();

#endif

