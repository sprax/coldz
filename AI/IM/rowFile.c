/*****************************************************************************
rowFile.c (< aaFwrite/aaFread)    S.R.Lines (spraxlo@ai.mit.edu)    93.7.24
*****************************************************************************/
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#ifndef   WIN32
#ifndef   MAC
#ifndef   NeXT
#include	<unistd.h>
#endif
#endif
#endif
#include	<un/unMem.h>
#define		UNBUG	1
#include	<un/unBug.h>
#include	<un/unTypes.h>
#include	<imagmem.h>
#include	<memMake.h>
#include	<rowMake.h>
#include	<rowFile.h>
#define   SLEN   128

int
rowReadFlt(ppf *inp, int iX, int iY, unt iW, unt iH
    , unt hdrBytes, void *header, const char *path
    , const char *ext, unt flags)
{
    char    ipath[256];
    FILE    *fp;
    ppf     row = NULL;
    size_t  count = iW*iH;

    if (flags) {
        if (ext != NULL)  sprintf_s(ipath, 256, "%s.%dx%d.%s", path, iW, iH, ext);
        else              sprintf_s(ipath, 256, "%s.%dx%d", path, iW, iH);
    }
    else {
        if (ext != NULL)  sprintf_s(ipath, 256, "%s.%s", path, ext);
        else              sprintf_s(ipath, 256, "%s", path);
    }

    errno_t err = fopen(ipath, "r");
    if (err) {
    warn("rowReadFlt err: fopen(%s,\"r\")", ipath);
    return 0;
  }

  /*
    prn("rowReadFlt: opened %s for input of %lu bytes"
          ,ipath,iW*iH*sizeof(flt)+hdrBytes);
  */

  if (*inp == NULL)
      row = rowMalFlt(iX, iY, iW, iH);
  else
      row = rowRalFlt(*inp, iX, iY, iW, iH);

  if (hdrBytes > 0 && 1 != fread(header, hdrBytes, 1, fp))
      warn("rowReadFlt err: fread header, %lu bytes", hdrBytes);

  if (count != fread(row[iY] + iX, sizeof(flt), count, fp)) {
      warn("rowReadFlt err: fread %lu bytes", count * sizeof(flt));
      rowKillFlt(row, iX, iY);
      return 0;
  }
  fclose(fp);
  *inp = row;	/* So inp only changes on successful read. */
  return count;
}


int
rowWriteFlt(ppf src, int xi, int yi, unt wide, unt tall
    , unt hdrBytes, void *header, const char *path
    , const char *ext, unt flags)
{
    FILE *fout;
    char op[SLEN];
    size_t rowBytes = (size_t)wide * sizeof(flt);
    size_t sz = rowBytes*tall + hdrBytes;

    if (flags) {
        if (ext != NULL)  sprintf_s(op, "%s.%dx%d.%s", path, wide, tall, ext);
        else              sprintf_s(op, "%s.%dx%d", path, wide, tall);
    }
    else {
        if (ext != NULL)  sprintf_s(op, "%s.%s", path, ext);
        else              sprintf_s(op, "%s", path);
    }

    if (!(fout = fopen(op, "w"))) {
        warn("rowWriteFlt: err fopen(%s,\"w\")", op);
        return 0;
    }

    /*
      prn("rowWriteFlt: opened %s for output of %lu bytes",op,sz);
    */

    if (hdrBytes > 0) {
        if (fwrite((void *)header, sizeof(unc), hdrBytes, fout) != hdrBytes) {
            warn("rowWriteFlt: err fwrite(hdr,%d)", hdrBytes);
            return 0;
        }
    }

    for (src += yi; tall; src++, tall--) {
        if (fwrite((void *)(*src + xi), sizeof(unc), rowBytes, fout) != rowBytes) {
            warn("rowWriteFlt: err fwrite(%d)", rowBytes);
            return 0;
        }
    }
    fclose(fout);
    return sz;
}



#if 0
void *rowRead(int iX, int iY, unt iW, unt iH
    , size_t eltSize, unt hdrBytes, const char *path)
{
    FILE *fp;
    void **row = NULL;
    size_t count = iW*iH;

    if (!(fp = fopen(path, "r")))
        warn("rowRead err: fopen(%s,\"r\")", path);
    else {
        row = rowMalVoid(iX, iY, iW, iH, eltSize);
        if (fseek(fp, hdrBytes, SEEK_SET) == -1 ||
            count != fread(row[iY] + iX, eltSize, count, fp)) {
            warn("rowRead err: fseek or fread, %lu bytes", count*eltSize);
            /* rowFreeVoid(row,iX,iY);       ** frees and nullifies row */
        }
        fclose(fp);
    }
    return row;
}
#endif




void rowWrite(ppu src, int xi, int yi, unt wide, unt tall
    , size_t eltsize, unt hdrBytes, const char *path, const char *ext)
{
    FILE *fout;
    char op[SLEN], ox[SLEN];  puc hdr;
    size_t rowBytes = (size_t)wide*eltsize;

    assert(rowBytes > 0);
    if (eltsize > 1)  sprintf_s(ox, SLEN, "%dx%dx%d", wide, tall, eltsize);
    else              sprintf_s(ox, SLEN, "%dx%d", wide, tall);
    if (ext != NULL)  sprintf_s(op, SLEN, "%s.%s.", path, ext);
    else              sprintf_s(op, SLEN, "%s.", path);
    strcat_s(op, SLEN, ox);
    errno_t err = fopen_s(&fout, op, "w");
    if (err) {
        die("rowWrite err: fopen(%s,\"w\")\n", op);
    }
    WQQ3("rowWrite: opened %s for output of %lu bytes.\n", op, rowBytes*tall + hdrBytes);
    if (hdrBytes > 0) {
        callocAss(hdr, unc, hdrBytes);
        if (fwrite((void *)hdr, sizeof(unc), hdrBytes, fout) != hdrBytes)
            die("rowWrite: fwrite(hdr,%d)\n", hdrBytes);
    }
    for (src += yi; tall; src++, tall--)
        if (fwrite((void *)(*src + xi), sizeof(unc), rowBytes, fout) != rowBytes)
            die("rowWrite: fwrite(%d)\n", rowBytes);
    fclose(fout);
}


unc **rowReadUnc(const char *path, unt wide, unt tall, unt hdrBytes)
{
    FILE *fp;
    ppu row;
    size_t count = wide*tall;

    errno_t err = fopen_s(&fp, path, "r");    
    if (err) {
        warn("rowRead err: fopen(%s,\"r\")", path);
    }
    else {
        row = rowMalUnc(0, 0, wide, tall);
        if (fseek(fp, hdrBytes, SEEK_SET) == -1 ||
            count != fread(*row, sizeof(unc), count, fp)) {
            warn("rowRead err: fseek or fread, %lu bytes", count);
            rowFree((void*)row);	/* frees and nullifies row */
        }
        fclose(fp);
    }
    return row;
}

#if 0
MemHdr *MemHdrRead(const char *path, unt wide, unt tall
    , size_t eltsize, size_t hdrBytes)
{
    FILE *fp;
    MemHdr *mh;
    size_t rowBytes = (size_t)wide*tall*eltsize;

    if (!(fp = fopen(path, "r")))
        warn("rowRead err: fopen(%s,\"r\")", path);
    else {
        callocAss(mh, MemHdr, 1);
        if (hdrBytes > 0)
            mallocAss(mh->hdr, unc, hdrBytes);
        mallocAss(mh->row, unc, rowBytes);
        if ((hdrBytes > 0
            && hdrBytes != fread(mh->hdr, sizeof(unc), hdrBytes, fp))
            || rowBytes != fread(mh->row, sizeof(unc), rowBytes, fp)) {
            warn("rowRead err: fread(%lu+%lu) bytes", hdrBytes, rowBytes);
            rowFree(mh->row);
            rowFree(mh->hdr);	/* if (row) free and nullify row */
            free(mh);
        }
        fclose(fp);
    }
    return mh;
}




#endif

#if 0000 /*____________________________________________________________________*/


//#include "aa/aaConvert.h"


puc GetGrayRaw()
{
    FILE *fi;
    char path[100];
    puc gip8 = (puc)NULL;
    size_t count, nread;
    int j, cols, rows;
    for (path[0] = 0; !path[0] || !(fi = fopen(path, "r")); ) {
        printf("\n  Name of input file?  ");	gets(path);
    }
    printf("Opened %s\n", path);
    printf("\tEnter width & height (or <return> for 160,120):  ");
    for (j = 0; j != 2; ) {
        gets(path);
        j = sscanf(path, " %d %d", &cols, &rows);
        printf("\tGot cols, rows  =  %d  %d\n", cols, rows);
    }
    count = (size_t)rows * cols;
    gip8 = (puc)malloc(count);
    printf("GI_calloc successful");
    count = fread((void *)gip8, sizeof(unc), count, fi);
    if ((!nread) && feof(fi))    fprintf(stderr, "Unexpected end of input file\n");
    if (nread != count && ferror(fi))  fprintf(stderr, "Error reading after byte %lu\n", nread);
    fclose(fi);
    return(gip8);
}



/** LoadBitmapRGBI should be regarded as obsolete.  **/
int LoadBitmapRGBI(path, wide, tall, iptr)
char *path; int wide, tall; unc **iptr;
{   FILE *fp;
unc *buffer;
int stat = 0;
size_t count = wide * tall * 3;

/*  path = (char *)Compress_Check(path); */

if (fp = fopen(path, "r")) {
    if (*iptr = buffer = (puc)malloc(count * sizeof(unc)))
        if (count == fread(buffer, sizeof(unc), count, fp))
            stat = 1;
    fclose(fp);
}
return(stat);
}

puc LoadImageDave(path, wide, tall)
char *path; int *wide, *tall;
{   puc LoadImageRGBB();
FILE *fp;
puc  rgbi = NULL;
if (fp = fopen(path, "r")) {
    if (Header2LGet(fp, wide, tall)) {
        if ((rgbi = LoadImageRGBB(fp, *wide, *tall)) == NULL)
            fprintf(stderr, "LoadImageRGBB failed\n");
    }
    else fprintf(stderr, "Header2LGet failed\n");
    fclose(fp);
}
else printf("fopen failed\n");
return(rgbi);
}

puc LoadImageRGBB(fp, wide, tall)
FILE *fp; int wide, tall;
{   puc rgbi = (puc)NULL;
register unl k, q, wid3, size;
wid3 = (unl)wide * 3;
size = (unl)wid3 * tall;
if (!(rgbi = (puc)malloc(size * sizeof(unc))))
fprintf(stderr, "malloc failed\n");

for (q = 0; q != size; q += wid3)
for (k = 0; k != wid3; k += 3)
*(rgbi + q + k) = (unc)getc(fp);
for (q = 0; q != size; q += wid3)
for (k = 1; k != wid3 + 1; k += 3)
*(rgbi + q + k) = (unc)getc(fp);
for (q = 0; q != size; q += wid3)
for (k = 2; k != wid3 + 2; k += 3)
*(rgbi + q + k) = (unc)getc(fp);
return(rgbi);
}

int LoadRGBImagesDave(path, maxw, maxt, cip24, red8, grn8, blu8)
char *path; int *maxw, *maxt; puc cip24, red8, grn8, blu8;
{   int LoadImagesRGBB(), wide, tall, status = -1;
FILE *fp;
size_t nread, count = 2;
long header[2];

printf("LoadImagesDave: %s, ", path);
if (!(fp = fopen(path, "r"))) {
    printf("Failed to open for reading.\n");
    return(status);
}
nread = fread((void *)header, sizeof(long), count, fp);
wide = (int)header[0];
tall = (int)header[1];
if (nread != count || wide > *maxw || tall > *maxt) {
    printf("INVALID nread,wide,tall = %d %d %d\n", nread, wide, tall);
    printf("Assuming ASCII header of format \" %%d %%d\"\n");
    rewind(fp);
    nread = (size_t)fscanf(fp, "%ld %ld\n", &wide, &tall);
}
if (nread != count || wide > *maxw || tall > *maxt) {
    printf("INVALID nread,wide,tall = %d %d %d\n", nread, wide, tall);
    printf("Assuming no header:\n");
    rewind(fp);
    for (status = 1, nread = 0; nread != count && status < 4; status++) {
        printf("Manually enter width & height (cols & rows):  ");
        nread = (size_t)scanf(" %d %d", &wide, &tall);
        printf("\tGot wide, tall  =  %d  %d\n", wide, tall);
    }
}
if (nread != count || wide > *maxw || tall > *maxt) {
    printf("Three tries and you're out!\n");
}
else {
    *maxw = wide;  *maxt = tall;
    printf("size %d x %d ...", wide, tall);
    status = LoadImagesRGBB(fp, wide, tall, cip24, red8, grn8, blu8);
    printf("%s, status %d\n", (status ? "Failed" : "Done"), status);
}
fclose(fp);
return(status);
}


/** LoadImagesRGBB: Get color, & r,g,b arrays from Non-interleaved
 ** image stored in file where fp currently points, i.e., fp must point to
 ** the first byte after the header of an open file.
 **/
int LoadImagesRGBB(fp, wide, tall, clr24, red, grn, blu)
FILE *fp; int wide, tall; puc clr24, red, grn, blu;
{
    unl k, q, wid3, size;
    if (wide < 1 || tall < 1)  return(-1);

    wid3 = (unl)wide * 3;
    size = (unl)wid3 * tall;

    for (q = 0; q != size; q += wid3)
        for (k = 0; k != wid3; k += 3)
            *red++ = *(clr24 + q + k) = (unc)getc(fp);

    for (q = 0; q != size; q += wid3)
        for (k = 1; k != wid3 + 1; k += 3)
            *grn++ = *(clr24 + q + k) = (unc)getc(fp);

    for (q = 0; q != size; q += wid3)
        for (k = 2; k != wid3 + 2; k += 3)
            *blu++ = *(clr24 + q + k) = (unc)getc(fp);

    return(0);
}



puc LoadImageRGBBasGray(path, maxw, maxt)
char *path; int *maxw, *maxt;
{   FILE *fp;
size_t pixels;
puc gip, gbuf = NULL;
printf("LoadImageRGBBasGray: %s\n", path);
if (fp = fopen(path, "r")) {
    pixels = Header2LGet(fp, maxw, maxt);
    /** printf("  wide, tall, pixels = %d %d %lu\n",*maxw,*maxt,pixels);  **/
    if (gbuf = (puc)malloc(pixels * sizeof(unc))) {
        register size_t  q;
        for (gip = gbuf, q = pixels; q; q--)  *gip++ = (unc)(REDI * getc(fp));
        for (gip = gbuf, q = pixels; q; q--)  *gip++ += (unc)(GRNI * getc(fp));
        for (gip = gbuf, q = pixels; q; q--)  *gip++ += (unc)(BLUI * getc(fp));
    }
    else fprintf(stderr, "  malloc failed on %lu bytes\n", pixels);
    fclose(fp);
}
else fprintf(stderr, "  fopen failed\n");
return(gbuf);
}



/** LoadImagesDaveG: RGB + GRAY: for use on Macintosh: image space already allocated **/
int LoadImagesDaveG(path, maxw, maxt, cip24, gip8, red8, grn8, blu8)
char *path; int *maxw, *maxt; puc cip24, gip8, red8, grn8, blu8;
{   FILE *fp;  int stat = -1;
fprintf(stderr, "LoadImagesDaveG: %s, ", path);
if (fp = fopen(path, "r")) {
    Header2LGet(fp, maxw, maxt);
    printf("size %d x %d\n", *maxw, *maxt);
    stat = LoadImagesGRGBB(fp, *maxw, *maxt, cip24, gip8, red8, grn8, blu8);
    fclose(fp);
}
else fprintf(stderr, "open failed.\n");
return(stat);
}

/** LoadImagesGRGBB: Get color, gray, & r,g,b arrays from Non-interleaved
 ** image stored in file where fp currently points, i.e., fp must point to
 ** the first byte after the header of an open file.  **/
int LoadImagesGRGBB(fp, wide, tall, rgbi, gray, red, grn, blu)
FILE *fp; int wide, tall; puc rgbi, gray, red, grn, blu;
{   puc gbuf;
unl k, q, wid3, size;

if (wide < 1 || tall < 1)  return(-1);

wid3 = (unl)wide * 3;
size = (unl)wid3 * tall;

for (gbuf = gray, q = 0; q != size; q += wid3)
for (k = 0; k != wid3; k += 3)
*gbuf++ = (unc)(REDI * (*red++ = *(rgbi + q + k) = (unc)getc(fp)));

for (gbuf = gray, q = 0; q != size; q += wid3)
for (k = 1; k != wid3 + 1; k += 3)
*gbuf++ += (unc)(GRNI * (*grn++ = *(rgbi + q + k) = (unc)getc(fp)));

for (gbuf = gray, q = 0; q != size; q += wid3)
for (k = 2; k != wid3 + 2; k += 3)
*gbuf++ += (unc)(BLUI * (*blu++ = *(rgbi + q + k) = (unc)getc(fp)));

return(0);
}



size_t Header2LGet(fp, maxw, maxt)      /* 8-byte header is two 32-bit ints. */
FILE *fp; int *maxw, *maxt;
{   char header[8]; int  wide, tall;
if (fread((void *)header, 4, 2, fp) != 2) {
    fprintf(stderr, "Header2LGet: fread != 2\n");
    return(0L);
}
#if (INT_MAX == 2147483647)
wide = (int) *((int *)&header[0]);
tall = (int) *((int *)&header[4]);
if (wide < 0 || wide > *maxw || tall < 0 || tall > *maxt) {
    fprintf(stderr, "Header2LGet: INVALID wide,tall = %d %d\n", wide, tall);
    fprintf(stderr, "Assuming ASCII header of format \" %%d %%d\"\n");
    rewind(fp);
    if (fscanf(fp, "%d %d\n", &wide, &tall) != 2
        || wide < 0 || wide > *maxw || tall < 0 || tall > *maxt) {
        printf("Bad header; assuming wide,tall = %d, %d:\n", *maxw, *maxt);
        wide = *maxw, tall = *maxt;
        rewind(fp);
    }
}
#else
#if (LONG_MAX == 2147483647)
wide = (int) *((long *)&header[0]);
tall = (int) *((long *)&header[4]);
if (wide < 0 || wide > *maxw || tall < 0 || tall > *maxt) {
    long lwide, ltall;
    fprintf(stderr, "Header2LGet: INVALID wide,tall = %d %d\n", wide, tall);
    printf("Assuming ASCII header of format \" %%ld %%ld\"\n");
    rewind(fp);
    if (fscanf(fp, "%ld %ld\n", &lwide, &ltall) != 2
        || (wide = (int)lwide) < 0 || wide > *maxw || (tall = (int)ltall) < 0 || tall > *maxt) {
        printf("Bad header; assuming wide,tall = %d, %d:\n", *maxw, *maxt);
        wide = *maxw, tall = *maxt;
        rewind(fp);
    }
}
#else
do it yourself preprocessor error "Neither int nor long type size is 4 bytes"
#endif
#endif
*maxw = wide;  *maxt = tall;
return((unl)wide*tall);
}



/** WriteImageDave may output only rgb images. **/
int WriteImageDave(src, name, ext, cols, rows)
register   puc  src; char *name, *ext; int cols, rows;
{   long wide = (long)cols, tall = (long)rows;
char oname[128];
FILE *fout;
register unl q, k;
register unc uc;
unl wid3 = (unl)wide * 3, size = (unl)wid3 * tall;

sprintf_s(oname, "%s.%s.%dx%dx3b", name, ext, wide, tall);
if (!(fout = fopen(oname, "w"))) {
    fprintf(stderr, "WriteImageRaw: Error opening file %s\n", oname);
    return(-1);
}
printf("Opened %s for output of %lu bytes.\n", oname, (unl)wide*tall);
Header2LPut(fout, wide, tall);

for (q = 0; q != size; q += wid3)
for (k = 0; k != wid3; k += 3)
uc = *(src + q + k), putc(uc, fout);

for (q = 0; q != size; q += wid3)
for (k = 1; k != wid3 + 1; k += 3)
uc = *(src + q + k), putc(uc, fout);

for (q = 0; q != size; q += wid3)
for (k = 2; k != wid3 + 2; k += 3)
uc = *(src + q + k), putc(uc, fout);
fclose(fout);
return(0);
}


/** WriteImageDave3 takes separate sources for rgb channels. **/
int WriteImageDave3(red, grn, blu, name, ext, cols, rows)
puc  red, grn, blu; char *name, *ext; int cols, rows;
{   long wide = (long)cols, tall = (long)rows;
char oname[128];
FILE *fout;
size_t count;
puc Src, src[3];
int j, chan;
src[0] = red;
src[1] = grn;
src[2] = blu;

sprintf_s(oname, "%s.%s.%dx%dx3b", name, ext, wide, tall);
if (!(fout = fopen(oname, "w"))) {
    fprintf(stderr, "WriteImageRaw: Error opening file %s\n", oname);
    return(-1);
}
printf("Opened %s for output of %lu bytes.\n", oname, (unl)wide*tall);
Header2LPut(fout, wide, tall);
for (count = (size_t)wide, chan = 0; chan != 3; chan++) {
    Src = src[chan];
    for (j = tall; j; j--, Src += wide) {
        if (fwrite((void *)Src, (size_t)sizeof(unc), count, fout) != count) {
            fprintf(stderr, "Error fwriting row %d\n", (chan + 1)*(tall - j));
            return(-1);
        }
    }
}
fclose(fout);
return(0);
}


size_t Header2LPut(fp, wide, tall)
FILE *fp; int wide, tall;
{
#if   (INT_MAX  == 2147483647)
    int head[2];
    head[0] = wide; head[1] = tall;
    return (fwrite((void *)head, sizeof(int), 2, fp));
#else
#if (LONG_MAX == 2147483647)
    long head[2];
    head[0] = (long)wide; head[1] = (long)tall;
    return (fwrite((void *)head, sizeof(long), 2, fp));
#else
    /* #error "Neither int nor long type size is 4 bytes" */
    preprocessor error "Neither int nor long type size is 4 bytes"
#endif
#endif
}


#endif	/* 0000 */

