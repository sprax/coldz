/*****************************************************************************
 mjPack.h    S.R.Lines    94.08.27
#include      <unTypes.h>
#include      <mj.h>
 *****************************************************************************/
void
mjPackFltGray ( rgst punl l, ppf  src, pDmj D);

void
mjPackUncGray ( rgst punl l, ppu  src, pDmj D);

void
mjPackUncRGBP ( rgst punl l, ppu *src, pDmj D);

void    /* Truncating, not rounding.  For quick screen display only. */
mjPackFltRGBI(rgst unl *l, ppf src, pDmj D);

void
mjPackUncRGBS(rgst unl *l, rgst puc r, rgst puc g, rgst puc b, rgst unt n);



