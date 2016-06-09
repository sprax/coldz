/*****************************************************************************
 unFlags.c    S.R.Lines  (spraxlo@{ai|media}, srl@athena)    93.8.18
 Upper case Opt are for developers; lower case for users.
 *****************************************************************************/
#include	<string.h>
#include	<unFlags.h>

void Usage (char *fmt, ...);

void addUnFlag (unsigned long *options, int letter)
{
  switch (letter) {
    case 'b':	*options |= O_BLNR;	break;
    case 'c':	*options |= O_BCBC;	break;
    case 'e':	*options |= O_EXTR;	break;
    case 'f':	*options |= O_FLOT;	break;
    case 'h':	*options |= O_HELP;	break;
    case 'l':	*options |= O_SLIN;	break;
    case 'm':	*options |= O_MORE;	break;
    case 'n':	*options |= O_NONO;	break;
    case 'o':	*options |= O_HOLE;	break;
    case 'p':	*options |= O_PTRS;	break;
    case 'q':	*options |= O_BQDR;	break;
    case 's':	*options |= O_STAT;	break;
    case 'T':	*options |= O_TEST;	break;
    case 'u':	*options |= O_UNCH;	break;
    case 'v':	*options |= O_VERB;	break;
//  case 'V':	*options |= O_VIEW;	break;
    case 'w':	*options |= O_WARN;	break;
    case 'X':	*options |= O_XGUI;	break;
    case 'x':	*options |= O_XGUI;	break;
    default:	break;
  }
}


void addUnFlags (unsigned long *opts, unsigned long *devo
	, char *argstr)
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
//  case    'l'	:  *opts |= O_LBLS; break;
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

