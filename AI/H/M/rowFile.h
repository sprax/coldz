/*****************************************************************************
 rowFile.c    S.R.Lines (spraxlo@ai.mit.edu)    93.7.23
 *****************************************************************************/
        
#ifndef im_rowFile_h__
#define im_rowFile_h__

typedef	struct RowHdr	{
	puc	*row;
	void	*hdr;
}	RowHdr;


int rowReadFlt ( ppf *inp, int iX, int iY, unt iW, unt iH
               , unt hdrBytes, void *header, const char *path
               , const char *ext, unt flags);

int rowWriteFlt ( ppf src, int xi, int yi, unt wide, unt tall
                , unt hdrBytes, void *header, const char *path
                , const char *ext, unt flags);

void *rowRead ( int iX, int iY, unt iW, unt iH
              , size_t eltSize, unt hdrBytes, const char *path);

void rowWrite ( ppu row, int xi, int yi, unt wide, unt tall
              , size_t eltsize, unt hdrBytes, const char *path, const char *ext);


unc **rowReadUnc (const char *path, unt wide, unt tall, unt hdrBytes);

#endif  /* im_rowFile_h__ */


