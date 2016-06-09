/*********************************************************
 unFlags.c    S.R.Lines  (spraxlo@{ai|media}, srl@athena)
 Upper case Opt are for developers; lower case for users.
 ********************************************************/
#include	<string.h>
#include	<un/unFlags.h>
void Usage (char *fmt, ...);

void addUnFlags (unsigned long *opts, unsigned long *devo
	, char *argstr, unsigned long flags)
{
int k;
 for (k=strlen(argstr); k > 0 ; argstr++, k--) {
  switch (*argstr) {
    case 'B'	:  *devo |= O_BOTH; break;
    case    'b'	:  *opts |= O_BACK; break;  /* A    */
    case 'C'	:  *devo |= O_CENT; break;
    case    'c'	:  *opts |= O_CLIP; break;  /* DE   */
    case 'D'	:  *devo |= O_BCBC; break;
    case    'd'	:  *opts |= O_BCBC; break;
    case 'E'	:  *devo |= O_EXTR; break;
    case    'e'	:  *opts |= O_EXTR; break;
    case 'F'	:  *devo |= O_FWRD; break;
    case    'f'	:  *opts |= O_FWRD; break;
    case 'G'	:  *devo |= O_GRAY; break;
    case    'g'	:  *opts |= O_GRAY; break;
    case 'H'	:  *devo |= O_HITS; break;
    case    'h'	:  *opts |= O_HELP; break;
    case 'I'	:  *devo |= O_UNCH; break;
    case    'i'	:  *opts |= O_KLUG; break;
    case 'K'	:  *devo |= O_KLUG; break;
    case    'k'	:  *opts |= O_KLUG; break;
    case 'L'	:  *devo |= O_BLNR; break;
    case    'l'	:  *opts |= O_LBLS; break;
    case 'M'	:  *devo |= O_COMB; break;  /* N    */
    case    'm'	:  *opts |= O_MORE; break;
    case 'N'	:  *devo |= O_NONO; break;
    case    'n'	:  *opts |= O_NONO; break;
    case 'O'	:  *devo |= O_ONLY; break;
    case    'o'	:  *opts |= O_ONLY; break;  /* P    */
    case 'P'	:  *devo |= O_PTRS; break;
    case    'p'	:  *opts |= O_PTRS; break;
    case 'Q'	:  *devo |= O_BQDR; break;
    case    'q'	:  *opts |= O_BQDR; break;
    case 'R'	:  *devo |= O_READ; break;
    case    'r'	:  *opts |= O_READ; break;
    case 'S'	:  *devo |= O_STAT; break;
    case    's'	:  *opts |= O_STAT; break;
    case 'T'	:  *devo |= O_TEST; break;
    case    't'	:  *opts |= O_TIME; break;
    case 'U'	:  *devo |= O_UNCH; break;
    case    'u'	:  *opts |= O_UNCH; break;  /* V    */
    case 'V'	:  *devo |= O_WARN; break;
    case    'v'	:  *opts |= O_VERB; break;  /* V    */
    case 'W'	:  *devo |= O_WRIT; break;
    case    'w'	:  *opts |= O_WRIT; break;
    case 'X'	:  *devo |= O_XGUI; break;  /* yz   */
    case    'x'	:  *opts |= O_XGUI; break;  /* YZ   */
    default:	Usage(
      "Unrecognized option:  %c  (ASCII %d)",*argstr,*argstr);
  }
 }
}

