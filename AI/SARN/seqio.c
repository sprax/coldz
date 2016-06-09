/* Sequence I/O routines */

#include "imgdec.h"
#include "imghdr.h"

sopen(filename,type,s)
char *filename;
char *type;
SEQUENCE *s;
{
  if (strcmp(type,"r") == 0) {	/* Open sequence file for reading */
    if ((s->stream = fopen(filename,type)) == NULL) {
      fprintf(stderr,"sopen: can't open %s\n",filename);
      exit(-1);
    }
    read_seqhdr(s);
  } else if (strcmp(type,"w") == 0) {
    if ((s->stream = fopen(filename,type)) == NULL) {
      fprintf(stderr,"sopen: can't open %s\n",filename);
      exit(-1);
    }
    write_seqhdr(*s);
  } else {
    fprintf(stderr,"sopen: illegal access mode %s\n",type);
    exit(-1);
  }
}

sclose(s)
SEQUENCE s;
{
  fclose(s.stream);
}

sseek(s,x,y,t)
SEQUENCE s;
int x,y,t;
{
  if ((x >= s.w) || (y >= s.h) || (t >= s.d)) {
    fprintf(stderr,"sseek: invalid position for sequence file: %d %d %d\n",x,y,t);
    exit(-1);
  }
  if (fseek(s.stream,(long) ((t*s.h+y)*s.w+x+sizeof(IMAGE_HEADER)), 0) < 0) {
    fprintf(stderr,"sseek: error encountered while seeking\n");
    exit(-1);
  }
}

seekfnum(s,fnum)
SEQUENCE s;
int fnum;
{
  sseek(s,0,0,fnum);
}

read_seqhdr(s)
SEQUENCE *s;
{
  IMAGE_HEADER hdr;

  rewind(s->stream);
  if (fread(&hdr,sizeof(hdr),1,s->stream) != 1)
    die("read_seqhdr: error reading sequence header\n");
  s->w = hdr.width;  s->h = hdr.height;  s->d = hdr.depth;
  if ((s->type = hdr.type) != UBYTE) 
    die("read_seqhdr: Only unsigned byte sequence files supported at present\n");
}

write_seqhdr(s)
SEQUENCE s;
{
  IMAGE_HEADER hdr;

  rewind(s.stream);
  hdr.width = s.w;  hdr.height = s.h;  hdr.depth = s.d;
  if ((hdr.type = s.type) != UBYTE) 
    die("write_seqhdr: Only unsigned byte sequence files supported at present\n");
  if (fwrite(&hdr,sizeof(hdr),1,s.stream) != 1)
    die("write_seqhdr: error writing sequence header\n");
}

read_ffield(s,fnum,img)
SEQUENCE s;
int fnum;
FIMAGE img;
{
  BIMAGE b;

  b.w = img.w;   b.h = img.h;
  alloc_bimage(&b);
  read_bfield(s,fnum,b);
  btof(img,b);
  free_bimage(b);
}

read_bfield(s,fnum,img)
SEQUENCE s;
int fnum;
BIMAGE img;
{
  seekfnum(s,fnum);
  _read_field(s,img);
}

_read_field(s,img)
SEQUENCE s;
BIMAGE img;
{
  register byte **p=img.ptr,**fp;

  if ((s.w != img.w) || (s.h != img.h)) {
    fprintf(stderr,"_read_field: sequence and image sizes do not match\n");
    exit(-1);
  }
  for (fp=p+img.h; p < fp; p++)
    if (fread(*p,sizeof(**p),img.w,s.stream) != img.w) {
      fprintf(stderr,"_read_field: error reading field\n");
      exit(-1);
    }
}

write_ffield(s,fnum,img)
SEQUENCE s;
int fnum;
FIMAGE img;
{
  BIMAGE b;

  b.w = img.w;   b.h = img.h;
  alloc_bimage(&b);
  ftob(b,img);
  write_bfield(s,fnum,b);
  free_bimage(b);
}

write_bfield(s,fnum,img)
SEQUENCE s;
int fnum;
BIMAGE img;
{
  seekfnum(s,fnum);
  _write_field(s,img);
}

_write_field(s,img)
SEQUENCE s;
BIMAGE img;
{
  register byte **p=img.ptr,**fp;

  if ((s.w != img.w) || (s.h != img.h)) {
    fprintf(stderr,"_write_field: sequence and image sizes do not match\n");
    exit(-1);
  }
  for (fp=p+img.h; p < fp; p++)
    if (fwrite(*p,sizeof(**p),img.w,s.stream) != img.w) {
      fprintf(stderr,"_write_field: error writing field\n");
      exit(-1);
    }
}

