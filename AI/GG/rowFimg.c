/*****************************************************************************
 rowFimg.c    S.R.Lines    95.05.27
 *****************************************************************************/
#include        <stdio.h>
#include        <string.h>
#include        <un/unTypes.h>
#include        <un/unBug.h>
#include        <m/rowMake.h>
#include        <m/rowFile.h>
#include        <sarnoff/imghdr.h>

int
rowReadFimg ( ppf *inp, int iX, int iY, unt iW, unt iH, unt sw, unt sh
           , unt hdrBytes, const char *path, const char *ext, unt flags)
{
char    ipath[256];
FILE    *fp;
ppf     row = NULL;
size_t  count =  iW*iH, rw, rh;
int j;
IMAGE_HEADER shdr;


  if (flags & 1) {
    if (ext != NULL)  sprintf(ipath,"%s.%dx%d.%s",path,sw,sh,ext);
    else              sprintf(ipath,"%s.%dx%d"   ,path,sw,sh);
  } else {
    if (ext != NULL)  sprintf(ipath,"%s.%s",path,ext);
    else              sprintf(ipath,"%s"   ,path    );
  }

  if (! (fp = fopen(ipath,"r"))) {
    warn("rowReadFimg err: fopen(%s,\"r\")",ipath);
    return 0;
  }

  if (fread(&shdr,sizeof(shdr),1,fp) != 1)
    warn("rowReadFimg err: fread hdr, %lu bytes",sizeof(shdr));

  rw = shdr.width;
  rh = shdr.height;

#if 1
  if (sw != rw || sh != rh || shdr.type != FLOAT || shdr.depth != 1) {
    warn("rowReadFimg: mismatch parms:shdr   %d:%d %d:%d  type:%d depth:%d"
        ,sw, rw, sh, rh, shdr.type, shdr.depth);
  }
#endif

  /* prn("rowReadFimg: opened %s for input of %lu data bytes",ipath,rw*rh*sizeof(flt)); */

  if (*inp == NULL)
    row = rowCalFlt(iX,iY,iW,iH);
  else
    row = rowRalFlt(*inp,iX,iY,iW,iH);


  if (flags & 2)  {

    for (j = rh-1; j >= 0; j--) {
      if (flags & 4) {                  /* reverse sign on X-flow */
        int k; flt frow[1024], *dp, *sp;

        if (rw != fread(frow,sizeof(flt),rw,fp)) {
          warn("rowReadFimg err: fread data, row:%d  width:%lu",j,rw);
          rowKillFlt(row,iX,iY);
          return 0;
        }
        for (dp = row[j], sp = frow, k = 0; k < rw; k++)
          dp[k] = -*sp++ ;
      }
      else {
        if (rw != fread(row[j],sizeof(flt),rw,fp)) {
          warn("rowReadFimg err: fread data, row:%d  width:%lu",j,rw);
          rowKillFlt(row,iX,iY);
          return 0;
        }
      }
    }
  } else {
    for (j = 0; j < rh; j++) {
      if (rw != fread(row[j],sizeof(flt),rw,fp)) {
        warn("rowReadFimg err: fread data, row:%d  width:%lu",j,rw);
        rowKillFlt(row,iX,iY);
        return 0;
      }
    }
  }


  fclose(fp);
  *inp = row;   /* So inp only changes on successful read. */
  return count;
}



int
rowWriteFimg ( ppf src, int xi, int yi, unt wide, unt tall
             , const char *path, const char *ext, unt flags)
{
FILE *fout;
char op[256];
size_t rowBytes = (size_t) wide*sizeof(flt);
size_t sz = rowBytes*tall, rvrs = 0;
IMAGE_HEADER shdr;
void *pv;


  memset((char *)&shdr, 0, sizeof(shdr));
  if (flags & 1) {
    if (ext != NULL)  sprintf(op,"%s.%dx%d.%s",path,wide,tall,ext);
    else              sprintf(op,"%s.%dx%d"   ,path,wide,tall);
  } else {
    if (ext != NULL)  sprintf(op,"%s.%s",path,ext);
    else              sprintf(op,"%s"   ,path    );
  }

  if (!(fout = fopen(op,"w"))) {
    warn("rowWriteFimg: err fopen(%s,\"w\")",op);
    sz = 0;
  }
  /* else prn("rowWriteFimg: opened %s for output of %lu bytes",op,sz); */

  shdr.width = wide; shdr.height = tall;  shdr.type = FLOAT;  shdr.depth = 1;

  if (fwrite(&shdr,sizeof(shdr),1,fout) != 1) {
    warn("rowWriteFimg: err fwrite(shdr,%d)",sizeof(shdr));
    sz = 0;
  }

  if (flags & 2)  { int k; flt frow[1024], *pf;    rvrs = 1;

    for (src += (yi + tall - 1); tall; src--, tall--) {
 
      pv = src[xi];

      if (flags & 4) {			/* reverse sign on X-flow */
        for (pv = frow, pf = src[xi], k = 0; k < wide; k++) {
          frow[k] = -*pf++ ;
        }
      }
      if (fwrite(pv,sizeof(flt),wide,fout) != wide) {
        warn("rowWriteFimg: err fwrite(%d)",rowBytes);
        fclose(fout);
        return 0;
      }
    }
  } else {
    for (src += yi; tall; src++, tall--) {
      if (fwrite((void *)(*src+xi),sizeof(flt),wide,fout) != wide) {
        warn("rowWriteFimg: err fwrite(%d)",rowBytes);
        fclose(fout);
        return 0;
      }
    }
  }

  fclose(fout);
  return sz;
}


