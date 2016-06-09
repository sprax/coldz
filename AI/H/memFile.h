/*****************************************************************************
 memFile.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.23
 *****************************************************************************/
        
#ifndef im_memFile_h__
#define im_memFile_h__

typedef	struct MemHdr	{
	unc	*mem;
	void	*hdr;
}	MemHdr;

MemHdr *MemHdrRead ( const char *path, unt wide, unt tall
                   , size_t deep, size_t hdrBytes);

puc memRead(const char *path, unt wide, unt tall, unt eltSize, unt hdrBytes);

int memWriteUnc ( puc mem, unt wide, unt tall, unt deep, unt hdrBytes
                , const char *path, const char *ext);


#if 00
extern  int  Print3x3 (puc pc, int cols, int w);
extern  int  Print5x5 (puc pc, int cols, int w);
extern  void MoreF (float *src, size_t size, int nLines, int nPerLine, int space, int prec, char *label);
extern  void MoreU (puc src, size_t size, int nLines, int nPerLine, int space, char *label);
extern  int  LoadBitmapRGBI (char *path, unt wide, unt tall, unc **iptr);
extern  int  LoadImagesDaveG (char *path,int *maxw,int *maxt,puc cip24,puc gip8,puc red8,puc grn8, puc blu8);
extern  int  LoadImagesGRGBB (FILE *fp,int wide,int tall,puc rgbi,puc gray,puc red,puc grn, puc blu);
extern  int  LoadImagesRGBB (FILE *fp, unt wide, unt tall, puc clr24, puc red, puc grn, puc blu);
extern  int  LoadRGBImagesDave (char *path,int *maxw,int *maxt,puc cip24,puc red8,puc grn8,puc blu8);
extern  int  WriteImageDave (rgst puc  source, char *name, char *ext, int cols, int rows);
extern  int  WriteImageDave3 (puc red,puc grn,puc blu,char *name,char *ext,int cols,int rows);
extern  int  WriteImageRaw (puc src, char *name, char *ext, int wide, int  tall, int nChan);
extern  size_t  Header2LGet (FILE *fp, int *maxw, int *maxt);
extern  size_t  Header2LPut (FILE *fp, unt wide, unt tall);
extern  puc  GetGrayRaw (void);
extern  puc  LoadImageDave (char *path, int *wide, int *tall);
extern  puc  LoadImageRaw (char *path, unt wide, unt tall, int nChan, long headerBytes); 
extern  puc  LoadImageRGBB (FILE *fp, unt wide, unt tall);
extern  puc  LoadImageRGBBasGray (char *path, int *maxw, int *maxt);
#endif

#endif  /* im_memFile_h__ */


