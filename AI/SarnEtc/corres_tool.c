#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <sarnoff/imgdec.h>
#include <sarnoff/imghdr.h>
#include <sarnoff/imgmacro.h>
#include "triangulate.h"
/*#include "lists.h"*/
/*#include "label.h"*/

#include <X11/Intrinsic.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <Xm/MainW.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/Frame.h>
#include <Xm/FileSB.h>
#include <Xm/SelectioB.h>
#include <Xm/RowColumn.h>
#include <Xm/LabelG.h>
#include <Xm/MessageB.h>
#include <Xm/Scale.h>

extern float dist();
extern void triangulate_flow();
extern EdgeNode *MakeEdge();
extern TriNode *MakeTriangle();

FILTER rf, ef;
int Image_border = 4;
FIMAGE fimg1, fimg2, dx, dy;
int X1, Y1, X2, Y2, Selected1=0, Selected2=0;
int toplev=1, botlev=0, finlev=0;
Point2D *src_pts, *dest_pts;
EdgeNode *edge_list_top, *edge_list_bot;
TriNode *tri_list_top, *tri_list_bot;
int no_points=0;
int LISTSIZE = 100;
int scale = 1;

XtAppContext app;
Display *display;
Visual *visual;
GC gc;
Pixmap pixmap1, pixmap2;
XImage *img1, *img2;
Widget image1, image2;
Widget toplevel, form;
char *data1, *data2;
int colors[101];
Pixel bg, fg;
XImage *img;
char *data=NULL;
Pixmap pixmap;
int flashcount;
char *FILENAME = NULL;

void Mesg(w, event, params, num_params)
Widget w;
XClientMessageEvent *event;
String *params;
Cardinal *num_params;
{
  XEvent sendevent;

  printf("in Mesg\n");

  if ((flashcount % 2) == 0)  {
    XtVaSetValues(image2, XmNlabelPixmap, pixmap2, NULL);
  }
  else  {
    XtVaSetValues(image2, XmNlabelPixmap, pixmap, NULL);
  }

  flashcount++;

  if (flashcount < 21)  {
    sendevent.type = ColormapNotify;
    sendevent.xcolormap.serial = 0;
    sendevent.xcolormap.send_event = 1;
    sendevent.xcolormap.display = display;
    sendevent.xcolormap.window = event->window;

    XSendEvent(display, event->window, False, ColormapChangeMask, &sendevent);
  }
}

void Click1(w, event, params, num_params)
Widget w;
XButtonEvent *event;
String *params;
Cardinal *num_params;
{
  int x, y, xp, yp;
  int i,j,n, ind, width, height;
  XEvent sendevent;

/* get x,y coords from event struct */
  x = event->x - 2;  /* subtract 2 to account for border that X adds */
  y = event->y - 2;

  printf("Click1: (x,y)=(%d,%d)\n", x/scale, y/scale);

  width = fimg1.w;
  height = fimg1.h;

  if (!Selected1 && !Selected2)  {
    /* draw new point */
    X1 = x;  Y1 = y;
    ind = 100;
    for (i=Y1-1; i<=Y1+1; i++)  {
      for (j=X1-1; j<=X1+1; j++)  {
	data1[i*scale*width+j] = (char)colors[ind];
      }
    }
    Selected1 = 1;
  }
  else if ((Selected1) && (!Selected2))  {
    /* erase old point, draw new point */
    for (i=Y1-1; i<=Y1+1; i++)  {
      for (j=X1-1; j<=X1+1; j++)  {
	ind = (int)(100.0*(fimg1.ptr[i/scale][j/scale]/256.0));
	data1[i*scale*width+j] = (char)colors[ind];
      }
    }
    X1 = x;  Y1 = y;
    ind = 100;
    for (i=Y1-1; i<=Y1+1; i++)  {
      for (j=X1-1; j<=X1+1; j++)  {
	data1[i*scale*width+j] = (char)colors[ind];
      }
    }
  }
  else if (Selected1 && Selected2)  {
    /* save old correspondence, draw new point */
/*    dx.ptr[Y1][X1] = (float)(X2-X1);
    dy.ptr[Y1][X1] = (float)(Y2-Y1);
*/
    src_pts[no_points].x = X1/scale;
    src_pts[no_points].y = Y1/scale;
    dest_pts[no_points].x = X2/scale;
    dest_pts[no_points].y = Y2/scale;
    /* test if no_points == LISTSIZE and realloc if so */
    no_points++;
printf("no_points = %d\n", no_points);

    X1 = x;  Y1 = y;
    ind = 100;
    for (i=Y1-1; i<=Y1+1; i++)  {
      for (j=X1-1; j<=X1+1; j++)  {
	data1[i*scale*width+j] = (char)colors[ind];
      }
    }
    Selected2 = 0;
  }

  XFreePixmap(display, pixmap1);
  pixmap1 = XCreatePixmap(display, RootWindow(display,0), scale*width, scale*height, 8);
  XPutImage(display, pixmap1, gc, img1, 0,0, 0,0, scale*width, scale*height);
  XtVaSetValues(image1, XmNlabelPixmap, pixmap1, NULL);
}

void Click2(w, event, params, num_params)
Widget w;
XButtonEvent *event;
String *params;
Cardinal *num_params;
{
  int x, y, xp, yp, width, height;
  int i,j,n,ind;
  XEvent sendevent;

  width = fimg1.w;
  height = fimg1.h;

/* get x,y coords from event struct */
  x = event->x - 2;  /* subtract 2 to account for border that X adds */
  y = event->y - 2;
  printf("Click2: (x,y)=(%d,%d)\n", x/scale, y/scale);

  if (Selected1 && !Selected2)  {
    /* draw new point */
    X2 = x;  Y2 = y;
    ind = 100;
    for (i=Y2-1; i<=Y2+1; i++)  {
      for (j=X2-1; j<=X2+1; j++)  {
	data2[i*scale*width+j] = (char)colors[ind];
      }
    }
    Selected2 = 1;
  }
  else if (Selected1 && Selected2)  {
    /* erase old point, draw new one */
    for (i=Y2-1; i<=Y2+1; i++)  {
      for (j=X2-1; j<=X2+1; j++)  {
	ind = (int)(100.0*(fimg2.ptr[i/scale][j/scale]/256.0));
	data2[i*scale*width+j] = (char)colors[ind];
      }
    }
    X2 = x;  Y2 = y;
    ind = 100;
    for (i=Y2-1; i<=Y2+1; i++)  {
      for (j=X2-1; j<=X2+1; j++)  {
	data2[i*scale*width+j] = (char)colors[ind];
      }
    }
  }
  XFreePixmap(display, pixmap2);
  pixmap2 = XCreatePixmap(display, RootWindow(display,0), scale*width, scale*height, 8);
  XPutImage(display, pixmap2, gc, img2, 0,0, 0,0, scale*width, scale*height);
  XtVaSetValues(image2, XmNlabelPixmap, pixmap2, NULL);
}

distance_warp2(image, x1,y1,x2,y2,x3,y3,x4,y4,j,i)
FIMAGE image;
int x1, y1, x2, y2, x3, y3, x4, y4, j, i;
{
  float d1, d2, d3, d4, tot1, tot2;
  float w1, w2, w3, w4;

  d1 = dist((float)j,(float)i,(float)x1,(float)y1);
  d2 = dist((float)j,(float)i,(float)x2,(float)y2);
  d3 = dist((float)j,(float)i,(float)x3,(float)y3);
  d4 = dist((float)j,(float)i,(float)x4,(float)y4);

  tot1 = d1+d2+d3+d4;
  tot2 = tot1/d1 + tot1/d2 + tot1/d3 + tot1/d4;
  w1 = tot1/(d1*tot2);
  w2 = tot1/(d2*tot2);
  w3 = tot1/(d3*tot2);
  w4 = tot1/(d4*tot2);

  image.ptr[i][j] = w1 * image.ptr[y1][x1] +
                    w2 * image.ptr[y2][x2] +
		    w3 * image.ptr[y3][x3] +
		    w4 * image.ptr[y4][x4];
}

void copy_image(dest, src)
FIMAGE dest, src;
{
  int i,j;

  for (i=0; i<dest.h; i++)
    for (j=0; j<dest.w; j++)
      dest.ptr[i][j] = src.ptr[i][j];
}

fill(image)
FIMAGE image;
{
  FIMAGE image2;
  int h,w,i,j,k;
  int found, ind;
  int x1,y1,x2,y2,x3,y3,x4,y4;

  h = image.h;
  w = image.w;

  image2.h = h;
  image2.w = w;
  alloc_fimage(&image2);

  if (image.ptr[0][0] < 0.0)
    image.ptr[0][0] = 128.0;
  if (image.ptr[0][w-1] < 0.0)
    image.ptr[0][w-1] = 128.0;
  if (image.ptr[h-1][0] < 0.0)
    image.ptr[h-1][0] = 128.0;
  if (image.ptr[h-1][w-1] < 0.0)
    image.ptr[h-1][w-1] = 128.0;

  /* assign the edges of the image by interpolating in a straight
     line from the corners */
  for (i=1; i<w-1; i++)  {
    image.ptr[0][i] = (1.0 - ((float)i/(float)w))*image.ptr[0][0] +
                   ((float)i/(float)image.w)*image.ptr[0][w-1];
    image.ptr[h-1][i] = (1.0 - ((float)i/(float)w))*image.ptr[h-1][0] +
                   ((float)i/(float)w)*image.ptr[h-1][w-1];
  }
  for (i=1; i<h-1; i++)  {
    image.ptr[i][0] = (1.0 - ((float)i/(float)w))*image.ptr[0][0] +
                   ((float)i/(float)w)*image.ptr[h-1][0];
    image.ptr[i][w-1] = (1.0 - ((float)i/(float)w))*image.ptr[0][w-1] +
                   ((float)i/(float)w)*image.ptr[h-1][w-1];
  }

  for (i=1; i<h-1; i++)  {
    for (j=1; j<w-1; j++)  {
      if (image.ptr[i][j] < 0.0)  {
	/* grey level for pixel i,j is unassigned so interpolate to assign it*/
	/* find top, left point */
	found = 0;
	ind = 0;  x1 = -1;
	while (!found)  {
	  ind--;
	  for (k=0; k>=ind; k--)  {
	    if (image.ptr[i+k][j+ind] >= 0.0)  {
	      x1 = j+ind;  y1 = i+k;
	      k = ind-1;
	      found = 1;
	    }
	  }
	  for (k=0; k>ind; k--)  {
	    if (image.ptr[i+ind][j+k] >= 0.0)  {
	      if (x1 > -1)  {
		if (dist((float)i, (float)j, (float)y1, (float)x1) >
		    dist((float)i, (float)j, (float)(i+ind), (float)(j+k)))  {
		  x1 = j+k;  y1 = i+ind;
		  k = ind;
		  found = 1;
		}
	      }
	      else  {
		x1 = j+k;  y1 = i+ind;
		k = ind;
		found = 1;
	      }
	    }
	  }
	}
	if ((i == 1) && (j == 1))
	  fprintf(stderr, "x1=%d, y1=%d\n", x1, y1);

	/* find top, right point */
	found = 0;
	ind = 0; x2 = -1;
	while (!found)  {
	  ind++;
	  for (k=0; k<=ind; k++)  {
	    if (image.ptr[i-k][j+ind] >= 0.0)  {
	      x2 = j+ind;  y2 = i-k;
	      k = ind+1;
	      found = 1;
	    }
	  }
	  for (k=0; k<ind; k++)  {
	    if (image.ptr[i-ind][j+k] >= 0.0)  {
	      if (x2 > -1)  {
		if (dist((float)i, (float)j, (float)y2, (float)x2) >
		    dist((float)i, (float)j, (float)(i-ind), (float)(j+k)))  {
		  x2 = j+k;  y2 = i-ind;
		  k = ind;
		  found = 1;
		}
	      }
	      else  {
		x2 = j+k;  y2 = i-ind;
		k = ind;
		found = 1;
	      }
	    }
	  }
	}

	/* find bottom, left point */
	found = 0;
	ind = 0;  x3 = -1;
	while (!found)  {
	  ind++;
	  for (k=0; k<=ind; k++)  {
	    if (image.ptr[i+k][j-ind] >= 0.0)  {
	      x3 = j-ind;  y3 = i+k;
	      k = ind+1;
	      found = 1;
	    }
	  }
	  for (k=0; k<ind; k++)  {
	    if (image.ptr[i+ind][j-k] >= 0.0)  {
	      if (x3 > -1)  {
		if (dist((float)i, (float)j, (float)y3, (float)x3) >
		    dist((float)i, (float)j, (float)(i+ind), (float)(j-k)))  {
		  x3 = j-k;  y3 = i+ind;
		  k = ind;
		  found = 1;
		}
	      }
	      else  {
		x3 = j-k;  y3 = i+ind;
		k = ind;
		found = 1;
	      }
	    }
	  }
	}

	/* find bottom, right point */
	found = 0;
	ind = 0;  x4 = -1;
	while (!found)  {
	  ind++;
	  for (k=0; k<=ind; k++)  {
	    if (image.ptr[i+k][j+ind] >= 0.0)  {
	      x4 = j+ind;  y4 = i+k;
	      k = ind+1;
	      found = 1;
	    }
	  }
	  for (k=0; k<ind; k++)  {
	    if (image.ptr[i+ind][j+k] >= 0.0)  {
	      if (x4 > -1)  {
		if (dist((float)i, (float)j, (float)y4, (float)x4) >
		    dist((float)i, (float)j, (float)(i+ind), (float)(j+k)))  {
		  x4 = j+k;  y4 = i+ind;
		  k = ind;
		  found = 1;
		}
	      }
	      else  {
		x4 = j+k;  y4 = i+ind;
		k = ind;
		found = 1;
	      }
	    }
	  }
	}

	distance_warp2(image, x1,y1,x2,y2,x3,y3,x4,y4,j,i);
image2.ptr[i][j] = image.ptr[i][j];
image.ptr[i][j] = -1.0;

      }
    }
  }

  for (i=1; i<h-1; i++) {
    for (j=1; j<w-1; j++)  {
      if (image.ptr[i][j] < 0.0)  {
	image.ptr[i][j] = image2.ptr[i][j];
      }
    }
  }
}

void my_warp_fwd(warped, image, dx, dy)
FIMAGE warped, image, dx, dy;
{
  int i,j,x,y;

fprintf(stderr, "In my_warp_fwd, h=%d, w=%d\n", dx.h, dx.w);

  for (i=0; i<dx.h; i++)  {
    for (j=0; j<dx.w; j++)  {
      warped.ptr[i][j] = 255.0;
    }
  }

  for (i=0; i<dx.h; i++)  {
    for (j=0; j<dx.w; j++)  {
      x = j + (int)rint((double)dx.ptr[i][j]);
      y = i + (int)rint((double)dy.ptr[i][j]);
      if ((x < image.w) && (x >= 0) && (y < image.h) && (y >= 0))  {
	if (image.ptr[i][j] < warped.ptr[y][x])
	  warped.ptr[y][x] = image.ptr[i][j];
      }
    }
  }
}

void my_warp_fwd_grey(image, warped, dx, dy, wt)
FIMAGE *image, *warped, *dx, *dy;
float wt;
{
  int i,j,x,y, xp, yp;

  for (i=0; i<dx->h; i++)  {
    for (j=0; j<dx->w; j++)  {
      warped->ptr[i][j] = -1.0;
    }
  }

  for (i=0; i<dx->h; i++)  {
    for (j=0; j<dx->w; j++)  {
      x = j + (int)rint((double)dx->ptr[i][j]);
      y = i + (int)rint((double)dy->ptr[i][j]);
      xp = j + (int)rint((double)(wt * dx->ptr[i][j]));
      yp = i + (int)rint((double)(wt * dy->ptr[i][j]));

      if ((xp < image->w) && (xp >= 0) && (yp < image->h) && (yp >= 0))  {
/*	warped->ptr[yp][xp] = (1.0-wt)*image->ptr[i][j] + wt*image2->ptr[y][x];*/
	warped->ptr[yp][xp] = image->ptr[i][j];
      }
    }
  }
}

void message_box(str)
char *str;
{
  Widget dialog;
  XmString t;

  printf("%s\n", str);

  t = XmStringCreateSimple(str);
  dialog = XmCreateMessageDialog(toplevel, "message", NULL, 0);
  XtVaSetValues(dialog,
		XmNdialogType, XmDIALOG_MESSAGE,
		XmNmessageString, t, NULL);
  XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
  XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
/*  XtSetSensitive(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON), False);*/
  XtAddCallback(dialog, XmNokCallback, XtDestroyWidget, NULL);
  XmStringFree(t);
  XtManageChild(dialog);
  XtPopup(XtParent(dialog), XtGrabNone);
}

void get_file(fs, client_data, cbs)
Widget fs;
XtPointer client_data;
XmFileSelectionBoxCallbackStruct *cbs;
{
  int i, j, w, h, ind;
  float **p;
  char tmpstr[100];

  if (!XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &FILENAME)) {
    FILENAME = NULL;
    return;
  }
  if (! *FILENAME)  { /*nothing typed*/
    XtFree(FILENAME);
    FILENAME = NULL;
    return;
  }
  printf("FILENAME = %s\n", FILENAME);
  if (! (FILENAME == NULL))  {
    /* Do some error checking on this read!!! */
    read_imageP5(FILENAME, &fimg1);
  }
  XtUnmanageChild(fs);

  p = fimg1.ptr;
  w = fimg1.w;
  h = fimg1.h;
  XDestroyImage(img);
  data = (char *)malloc(w*h*sizeof(char));

  for (i=0; i<h; i++)
    for (j=0; j<w; j++)  {
      ind = (int)(100.0*(p[i][j]/256.0));
      data[i*w+j] = (char)colors[ind];
    }

  img = XCreateImage(display, visual, 8, ZPixmap, 0, data, w, h, 8, 0);
  XFreePixmap(display, pixmap);
  pixmap = XCreatePixmap(display, RootWindow(display,0), w, h, 8);
  XPutImage(display, pixmap, gc, img, 0,0, 0,0, w, h);
  XtVaSetValues(image1, XmNlabelPixmap, pixmap, NULL);
}

double_images()
{
  int i,j, old_scale;
  char *tmp_data1;

  old_scale = scale;
  scale *= 2;
printf("scale = %d\n", scale);

  tmp_data1 = (char *)malloc(scale*(fimg1.h)*scale*(fimg1.w)*sizeof(char));

  for (i=0; i<old_scale*fimg1.h; i++)  {
    for (j=0; j<old_scale*fimg1.w; j++)  {
      tmp_data1[i*2*scale*fimg1.w + 2*j] = data1[i*old_scale*fimg1.w + j];
      tmp_data1[i*2*scale*fimg1.w + 2*j+1] = data1[i*old_scale*fimg1.w + j];
      tmp_data1[(i*2+1)*scale*fimg1.w + 2*j] = data1[i*old_scale*fimg1.w + j];
      tmp_data1[(i*2+1)*scale*fimg1.w + 2*j+1] = data1[i*old_scale*fimg1.w + j];
    }
  }

  XDestroyImage(img1);

  data1 = tmp_data1;

  img1 = XCreateImage(display, visual, 8, ZPixmap, 0, tmp_data1, scale*fimg1.w, scale*fimg1.h, 8, 0);

  img1->data=tmp_data1;

  XFreePixmap(display, pixmap1);
  pixmap1 = XCreatePixmap(display, RootWindow(display,0), scale*fimg1.w, scale*fimg1.h, 8);
  XPutImage(display, pixmap1, gc, img1, 0,0, 0,0, scale*fimg1.w, scale*fimg2.h);
  XtVaSetValues(image1, XmNlabelPixmap, pixmap1, NULL);

  tmp_data1 = (char *)malloc(scale*(fimg2.h)*scale*(fimg2.w)*sizeof(char));
  for (i=0; i<old_scale*fimg2.h; i++)  {
    for (j=0; j<old_scale*fimg2.w; j++)  {
      tmp_data1[i*2*scale*fimg2.w + 2*j] = data2[i*old_scale*fimg2.w + j];
      tmp_data1[i*2*scale*fimg2.w + 2*j+1] = data2[i*old_scale*fimg2.w + j];
      tmp_data1[(i*2+1)*scale*fimg2.w + 2*j] = data2[i*old_scale*fimg2.w + j];
      tmp_data1[(i*2+1)*scale*fimg2.w + 2*j+1] = data2[i*old_scale*fimg2.w + j];
    }
  }
  XDestroyImage(img2);
  data2 = tmp_data1;
  img2 = XCreateImage(display, visual, 8, ZPixmap, 0, data2, scale*fimg2.w, scale*fimg2.h, 8, 0);

  img2->data=data2;

  pixmap2 = XCreatePixmap(display, RootWindow(display,0), scale*fimg2.w, scale*fimg2.h, 8);
  XPutImage(display, pixmap2, gc, img2, 0,0, 0,0, scale*fimg2.w, scale*fimg2.h);
  XtVaSetValues(image2, XmNlabelPixmap, pixmap2, NULL);
}

void file_cb(menu_item, item_no, cbs)
Widget menu_item;
int item_no;
XmAnyCallbackStruct *cbs;
{
  char fname[50];
  Widget dialog;
  extern void exit();

  printf("File: item_no = %d\n", item_no);
  switch (item_no)  {
  case 0:  /* Load base image */
    dialog = XmCreateFileSelectionDialog(toplevel, "filesb", NULL, 0);
    XtVaSetValues(dialog, XmNbackground, bg, XtVaTypedArg, XmNpattern, XmRString, "*.vector", 9, NULL);
    XtAddCallback(dialog, XmNcancelCallback, XtUnmanageChild, NULL);
    XtAddCallback(dialog, XmNokCallback, get_file, NULL);
    XtManageChild(dialog);
    XtPopup(XtParent(dialog), XtGrabNone);
    XMapRaised(XtDisplay(dialog), XtWindow(XtParent(dialog)));
    break;
  case 1:  /* Load pgm image */
    message_box("This function not yet implemented");
    break;
  case 2:  /* Save flow */
    write_fimage("flow.dx", dx);
    write_fimage("flow.dy", dy);
    break;
  case 3:  /* Quit */
    exit(0);
  }
}

void operations_cb(menu_item, item_no, cbs)
Widget menu_item;
int item_no;
XmAnyCallbackStruct *cbs;
{
  int i,j,k, no_outputs_read;
  static int index = 0;
  char tmp_fname[100], tmpstr[100];
  FIMAGE warped, tmp_img1, tmp_img2;
  TriNode *tri_ptr;

  warped.w = fimg1.w;  warped.h = fimg1.h;
  alloc_fimage(&warped);
  
  switch (item_no)  {
  case 0:  /* Interpolate */
    {
      FIMAGE tmp_dx, tmp_dy;
      tmp_dx.w = dx.w;  tmp_dx.h = dx.h;
      tmp_dy.w = dy.w;  tmp_dy.h = dy.h;
      alloc_fimage(&tmp_dx);
      alloc_fimage(&tmp_dy);
/*
      interp_flow(&dx, &dy, &dx, &dy);
*/
      triangulate_flow();

      tri_ptr = tri_list_top;
      while (tri_ptr != NULL)  {
	printf("tri: %d %d %d %d\n", tri_ptr->e1->pt1, tri_ptr->e1->pt2, tri_ptr->e2->pt1, tri_ptr->e2->pt2);
	tri_ptr = tri_ptr->next;
      }
printf("dx=\n");
for (i=0; i<fimg1.h; i++)  {
  for (j=0; j<fimg1.w; j++)  {
    printf("%1.1f ", dx.ptr[i][j]);
  }
  printf("\n");
}
printf("dy=\n");
for (i=2; i<=11; i++)  {
  for (j=2; j<=11; j++)  {
    printf("%2.2f ", dy.ptr[i][j]);
  }
  printf("\n");
}
      my_warp_fwd(warped, fimg1, dx, dy);
/*      my_warp_fwd_grey(&fimg1, &warped, &dx, &dy, 1.0);
      fill(warped);*/
      write_imageP5("test2.pgm", warped.ptr, warped.h, warped.w);
      write_fimage("flow.dx", dx);
      write_fimage("flow.dy", dy);

      tmp_img1.w = fimg1.w;  tmp_img1.h = fimg1.h;
      tmp_img2.w = fimg1.w;  tmp_img2.h = fimg1.h;
      alloc_fimage(&tmp_img1);
      alloc_fimage(&tmp_img2);
      copy_image(tmp_img1, warped);
      copy_image(tmp_img2, fimg2);

      lkflow(tmp_img2, tmp_img1, &tmp_dx, &tmp_dy, botlev, finlev, toplev);

      /* add interpolated field (dx, dy) and correction
	 field (tmp_dx, tmp_dy) to get best field */
      for (i=0; i<dx.h; i++)
	for (j=0; j<dy.w; j++)  {
	  dx.ptr[i][j] += tmp_dx.ptr[i][j];
	  dy.ptr[i][j] += tmp_dy.ptr[i][j];
	}

      /* compute warp of image according to velocity field */
/*      bilinear_warp(fimg2,warped,dx,dy,1.0);*/
      my_warp_fwd(warped, fimg1, dx, dy);
/*    my_warp_fwd_grey(&fimg1, &warped, &dx, &dy, 1.0);
      fill(warped);*/

      write_imageP5("test.pgm", warped.ptr, warped.h, warped.w);
/*      system("xv test.pgm &");*/

      free_fimage(tmp_dx);
      free_fimage(tmp_dy);

      break;
    }
  case 1:  /* Display warped */
    message_box("This function not yet implemented");
    break;
  case 2:  /* Delete last corres */
    message_box("This function not yet implemented");
    break;
  case 3:  /* Double */
    double_images();
    break;
  case 4:  /* Half */
    message_box("This function not yet implemented");
    break;
  }

  free_fimage(warped);
}

main(argc, argv)
int argc;
char *argv[];
{
  Widget main_w;
  Widget menubar;
  int i,j, ind, sl;
  Colormap cmap;
  XColor colorcell, unused;
  char colorname[10], fname[100];
  XtActionsRec actions[] = {{"Click1", Click1}, {"Click2", Click2}, {"Mesg", Mesg}};
  char defaultTranslations1[] = "<Btn1Down>: Click1()\n\
                                <Clrmap>: Mesg()";
  char defaultTranslations2[] = "<Btn1Down>: Click2()\n\
                                <Clrmap>: Mesg()";
  XtTranslations mytranslations1, mytranslations2;
  XmString str1, str2, str3, str4, str5;
  XmString str1_acc, str2_acc, str3_acc, str4_acc, str5_acc;
  EdgeNode *edge1, *edge2, *edge3;
  TriNode *tri;
  char string1[5];

  if (argc < 3)  {
    fprintf(stderr, "Usage: corres_tool base.pgm image.pgm\n");
    exit(0);
  }

  printf("arg1=%s, arg2=%s\n", argv[1], argv[2]);
  sl = strlen(argv[1]);
  string1[0] = argv[1][sl-3];
  string1[1] = argv[1][sl-2];
  string1[2] = argv[1][sl-1];
  string1[3] = '\0';
printf("str1=%s, sl=%d\n", string1, sl);

  if (!strcmp(".pgm", string1))
    read_imageP5(argv[1], &fimg1);
  else
    read_imageP4(argv[1], &fimg1);

  sl = strlen(argv[2]);
  string1[0] = argv[2][sl-3];
  string1[1] = argv[2][sl-2];
  string1[2] = argv[2][sl-1];
  string1[3] = '\0';
  if (!strcmp(".pgm", string1))
    read_imageP5(argv[2], &fimg2);
  else
    read_imageP4(argv[2], &fimg2);

  dx.h = fimg1.h;
  dx.w = fimg1.w;
  dy.h = fimg1.h;
  dy.w = fimg1.w;
  alloc_fimage(&dx);
  alloc_fimage(&dy);

  for (i=0; i<dx.h; i++)
    for (j=0; j<dx.w; j++)  {
      dx.ptr[i][j] = 200.0;
      dy.ptr[i][j] = 200.0;
    }

  no_points = 0;
  src_pts = (Point2D *)malloc(LISTSIZE*sizeof(Point2D));
  dest_pts = (Point2D *)malloc(LISTSIZE*sizeof(Point2D));
  edge_list_top = edge_list_bot = NULL;
  tri_list_top = tri_list_top = NULL;

/* init 4 corners */
  src_pts[0].x = 0;  src_pts[0].y = 0;
  dest_pts[0].x = 0; dest_pts[0].y = 0;
  src_pts[1].x = fimg1.w-1;  src_pts[1].y = 0;
  dest_pts[1].x = fimg1.w-1; dest_pts[1].y = 0;
  src_pts[2].x = 0;  src_pts[2].y = fimg1.h-1;
  dest_pts[2].x = 0; dest_pts[2].y = fimg1.h-1;
  src_pts[3].x = fimg1.w-1;  src_pts[3].y = fimg1.h-1;
  dest_pts[3].x = fimg1.w-1; dest_pts[3].y = fimg1.h-1;
  no_points = 4;

  toplevel = XtVaAppInitialize(&app, "Corres", NULL, 0, &argc, argv,
			       NULL, NULL);

  display = XtDisplay(toplevel);

  cmap = DefaultColormap(display, 0);
  visual = DefaultVisual(display,0);
  gc = XtGetGC(toplevel, 0, NULL);

  XtAppAddActions(app, actions, XtNumber(actions));
  mytranslations1 = XtParseTranslationTable(defaultTranslations1);
  mytranslations2 = XtParseTranslationTable(defaultTranslations2);

  XAllocNamedColor(display, cmap, "black", &colorcell, &unused);
  fg = colorcell.pixel;
  XAllocNamedColor(display, cmap, "green", &colorcell, &unused);
  bg = colorcell.pixel;

  XtVaSetValues(toplevel, XmNbackground, bg, NULL);
  XtVaSetValues(toplevel, XmNforeground, fg, NULL);

  main_w = XtVaCreateManagedWidget("main_window",
				   xmMainWindowWidgetClass, toplevel,
				   XmNscrollBarDisplayPolicy, XmAS_NEEDED,
				   XmNscrollingPolicy, XmAUTOMATIC,
				   XmNheight, fimg1.h+40,
				   XmNwidth, 2*fimg1.w+12,
				   XmNbackground, bg,
				   XmNforeground, fg,
				   NULL);

  str1 = XmStringCreateSimple("File");
  str2 = XmStringCreateSimple("Operations");
  menubar = XmVaCreateSimpleMenuBar(main_w, "menubar",
				    XmVaCASCADEBUTTON, str1, 'F',
				    XmVaCASCADEBUTTON, str2, 'O',
				    XmNbackground, bg,
				    XmNforeground, fg,
				    NULL);
  XmStringFree(str1);
  XmStringFree(str2);

  str1 = XmStringCreateSimple("Load base image");
  str1_acc = XmStringCreateSimple("Ctrl+b");
  str2 = XmStringCreateSimple("Load second image");
  str2_acc = XmStringCreateSimple("Ctrl+i");
  str3 = XmStringCreateSimple("Save flow");
  str3_acc = XmStringCreateSimple("Ctrl+s");  
  str4 = XmStringCreateSimple("Quit");
  str4_acc = XmStringCreateSimple("Ctrl+c");
  XmVaCreateSimplePulldownMenu(menubar, "file_menu", 0, file_cb,
			   XmVaPUSHBUTTON, str1, 'b', "Ctrl<key>b", str1_acc,
			   XmVaPUSHBUTTON, str2, 'i', "Ctrl<key>i", str2_acc,
			   XmVaPUSHBUTTON, str3, 'S', "Ctrl<key>s", str3_acc,
			   XmVaSEPARATOR,
			   XmVaPUSHBUTTON, str4, 'Q', "Ctrl<Key>c", str4_acc,
			   XmNbackground, bg,
			   XmNforeground, fg,
			   NULL);
  XmStringFree(str1);
  XmStringFree(str2);
  XmStringFree(str3);
  XmStringFree(str4);
  XmStringFree(str1_acc);
  XmStringFree(str2_acc);
  XmStringFree(str3_acc);
  XmStringFree(str4_acc);

  str1 = XmStringCreateSimple("Interpolate sparse flow");
  str1_acc = XmStringCreateSimple("Ctrl+f");
  str2 = XmStringCreateSimple("Display warped image");
  str2_acc = XmStringCreateSimple("Ctrl+w");
  str3 = XmStringCreateSimple("Delete last correspondence");
  str3_acc = XmStringCreateSimple("Ctrl+d");
  str4 = XmStringCreateSimple("Double size");
  str4_acc = XmStringCreateSimple("Ctrl+o");
  str5 = XmStringCreateSimple("Half size");
  str5_acc = XmStringCreateSimple("Ctrl+h");

  XmVaCreateSimplePulldownMenu(menubar, "operations_menu", 1, operations_cb,
			   XmVaPUSHBUTTON, str1, 'f', "Ctrl<key>f", str1_acc,
			   XmVaPUSHBUTTON, str2, 'w', "Ctrl<key>w", str2_acc,
			   XmVaPUSHBUTTON, str3, 'D', "Ctrl<Key>d", str3_acc,
			   XmVaPUSHBUTTON, str4, 'o', "Ctrl<Key>o", str4_acc,
			   XmVaPUSHBUTTON, str5, 'H', "Ctrl<Key>h", str5_acc,
			   XmNbackground, bg,
			   XmNforeground, fg,
			   NULL);
  XmStringFree(str1);
  XmStringFree(str2);
  XmStringFree(str3);
  XmStringFree(str4);
  XmStringFree(str5);
  XmStringFree(str1_acc);
  XmStringFree(str2_acc);
  XmStringFree(str3_acc);
  XmStringFree(str4_acc);
  XmStringFree(str5_acc);

  XtManageChild(menubar);

  form = XtVaCreateManagedWidget("form", xmFormWidgetClass, main_w,
				 XmNbackground, bg, XmNforeground, fg,
				 NULL);

  for (i=0; i<100; i++)  {
    sprintf(colorname, "gray%d", i);
    if (!XParseColor(display, cmap, colorname, &colorcell))  {
      printf("color name %s not in database\n", colorname);
      exit(0);
    }
    if (!XAllocColor(display, cmap, &colorcell))  {
      printf("All color cells allocated\n");
      exit(0);
    }
    colors[i] = colorcell.pixel;
  }

  data1 = (char *)malloc((fimg1.h)*(fimg1.w)*sizeof(char));

  img1 = XCreateImage(display, visual, 8, ZPixmap, 0, data1, fimg1.w, fimg1.h, 8, 0);
  for (i=0; i<fimg1.h; i++)
    for (j=0; j<fimg1.w; j++)  {
      ind = (int)(100.0*(fimg1.ptr[i][j]/256.0));
      data1[i*fimg1.w + j] = (char)colors[ind];
    }
  img1->data=data1;

  pixmap1 = XCreatePixmap(display, RootWindow(display,0), fimg1.w, fimg1.h, 8);
  XPutImage(display, pixmap1, gc, img1, 0,0, 0,0, fimg1.w, fimg2.h);

  data2 = (char *)malloc((fimg2.h)*(fimg2.w)*sizeof(char));

  img2 = XCreateImage(display, visual, 8, ZPixmap, 0, data2, fimg2.w, fimg2.h, 8, 0);
  for (i=0; i<fimg2.h; i++)
    for (j=0; j<fimg2.w; j++)  {
      ind = (int)(100.0*(fimg2.ptr[i][j]/256.0));
      data2[i*fimg2.w + j] = colors[ind];
    }
  img2->data=data2;

  pixmap2 = XCreatePixmap(display, RootWindow(display,0), fimg2.w, fimg2.h, 8);
  XPutImage(display, pixmap2, gc, img2, 0,0, 0,0, fimg2.w, fimg2.h);

  image1 = XtVaCreateManagedWidget("image1",
				  xmLabelWidgetClass, form,
				  XmNtopAttachment, XmATTACH_FORM,
				  XmNleftAttachment, XmATTACH_FORM,
				  XmNalignment, XmALIGNMENT_BEGINNING,
				  XmNlabelType, XmPIXMAP,
				  XmNlabelPixmap, pixmap1,
				  XmNbackground, bg,
				  NULL);

  XtOverrideTranslations(image1, mytranslations1);

  image2 = XtVaCreateManagedWidget("image2",
				  xmLabelWidgetClass, form,
				  XmNtopAttachment, XmATTACH_FORM,
				  XmNleftAttachment, XmATTACH_WIDGET,
				  XmNleftWidget, image1,
				  XmNlabelType, XmPIXMAP,
				  XmNlabelPixmap, pixmap2,
				  XmNbackground, bg,
				  NULL);

  XtOverrideTranslations(image2, mytranslations2);

  XtVaSetValues(main_w, XmNmenuBar, menubar, XmNworkWindow, form, NULL);
  XtRealizeWidget(toplevel);

  XtAppMainLoop(app);
}








