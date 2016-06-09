/* Laplacian Uniform Threshold Quantization Routines */

#include <math.h>
#include <un/unMath.h>

#define EPSILON 1e-6

#define M_SQRT2	1.41421356237309504880
#define SQRT2 M_SQRT2

#define TRUE  1
#define FALSE 0

double get_distortion(),get_ecrate(),get_repoff();
double get_bin_frm_dist(),get_bin_frm_rate();
int odd=1;

double get_distortion(delta,sigma)
double delta,sigma;
{
   double dist, theta = exp(-delta*SQRT2/sigma);

   if (odd) {
     dist = delta*delta*(2.0/(log(theta)*log(theta)) - 
                         sqrt(theta)*(0.5-1.0/log(theta))*(0.5-1.0/log(theta)) - 
                         sqrt(theta)*theta/((1.0-theta)*(1.0-theta)));
   } else {
     dist = delta*delta*(1.0/(log(theta)*log(theta)) - 
                              theta/((1.0-theta)*(1.0-theta)));
   }
   return(dist);
}

double get_ecrate(delta,sigma)
double delta,sigma;
{
   double rate, theta = exp(-delta*SQRT2/sigma), p0;

   if (odd) {
     p0 = 1 - sqrt(theta);
     rate = sqrt(theta)*((1.0-theta)*log(1.0-theta)+theta*log(theta))/(theta-1.0)
            - ((1.0-p0)*log(1.0-p0)+p0*log(p0)) + sqrt(theta)*log(2.0);
   } else {
     rate = ((1.0-theta)*log(1.0-theta)+theta*log(theta))/(theta-1.0) + log(2.0);
   }
   rate /= log(2.0);		/* convert to bits from nats */
   return(rate);
}


double get_repoff(delta,sigma)
double delta,sigma;
{
   double theta = exp(-delta*SQRT2/sigma);

   if (odd) {
     return(sigma/SQRT2 - 0.5*delta*(1.0+theta)/(1.0-theta));
   } else {
     return(sigma/SQRT2 - delta/(1.0-theta)); 
   }
}


double get_bin_frm_dist(distortion,sigma)
double distortion,sigma;
{
   double lower=0.0, upper=20*sigma;
   double delta,diff;

   while ((upper-lower) > EPSILON) {
     delta = (lower+upper)/2.0;
     if (fabs(diff=(distortion-get_distortion(delta,sigma))) < EPSILON)
       lower = upper;
     else
       if (diff > 0)
         lower = delta;
       else
         upper = delta;
   }
   return(delta);
}


double get_bin_frm_rate(rate,sigma)
double rate,sigma;
{
   double lower=0.0, upper=20*sigma;
   double delta,diff;

   while ((upper-lower) > EPSILON) {
     delta = (lower+upper)/2.0;
     if (fabs(diff=(rate-get_ecrate(delta,sigma))) < EPSILON)
       lower = upper;
     else
       if (diff > 0)
         upper = delta;
       else
         lower = delta;
   }
   return(delta);
}


