/* Rate computation functions */

#include "imgdec.h"
#include "imgmacro.h"

/****************************************************************************/
/*                            Rate computation routines                     */
/****************************************************************************/

double get_0table_huffman_rate(h)
HISTOGRAM h;
{
  double fasthuff();

  return(fasthuff(h.v,h.minval,h.maxval));
}


double get_0table_rate(h)
HISTOGRAM h;
{
  double getrate();

  return(getrate(h.v,h.minval,h.maxval));
}



double get_1table_huffman_rate(h)
HISTOGRAM h;
{
  int prefix;
  double rate,fasthuff();

  prefix = *h.v; *h.v = 0;	/* save zero run prefix occurences */
  rate = fasthuff(h.h,1,h.maxrun+h.maxval-h.minval+1);
  *h.v = prefix;		/* restore zero run prefix occurences */
  return(rate);
}


double get_1table_rate(h)
HISTOGRAM h;
{
  int prefix;
  double rate,getrate();

  prefix = *h.v; *h.v = 0;	/* save zero run prefix occurences */
  rate = getrate(h.h,1,h.maxrun+h.maxval-h.minval+1);
  *h.v = prefix;		/* restore zero run prefix occurences */
  return(rate);
}



double get_2table_huffman_rate(h)
HISTOGRAM h;
{
  double rate,fasthuff();

  rate = fasthuff(h.zr,1,h.maxrun);
  rate += fasthuff(h.v,h.minval,h.maxval);
  return(rate);
}


double get_2table_rate(h)
HISTOGRAM h;
{
  double rate,getrate();

  rate = getrate(h.zr,1,h.maxrun);
  rate += getrate(h.v,h.minval,h.maxval);
  return(rate);
}
  


double get_3table_huffman_rate(h)
HISTOGRAM h;
{
  double rate,fasthuff();

  rate = fasthuff(h.zr,0,h.maxrun);
  rate += fasthuff(h.v,h.minval,h.maxval);
  rate += fasthuff(h.nzr,0,h.maxrun);
  return(rate);
}


double get_3table_rate(h)
HISTOGRAM h;
{
  double rate,getrate();

  rate = getrate(h.zr,0,h.maxrun);
  rate += getrate(h.v,h.minval,h.maxval);
  rate += getrate(h.nzr,0,h.maxrun);
  return(rate);
}



double getrate(h,lb,ub)
int *h,lb,ub;
{
  double rate,entropy;
  int k,nk,nsym;

  nsym = 0; entropy = 0.0;
  for (k=lb; k <= ub; k++)
    if ((nk = h[k]) > 0) {
      nsym += nk;
      entropy -= nk*log10((double) nk);
    }
  if (nsym > 0)
    entropy = (entropy/nsym + log10((double) nsym))/log10(2.0);
  rate = entropy*nsym;
  return(rate);
}

