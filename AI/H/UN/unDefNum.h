/*****************************************************************************
 unDefNum.h -- numerical constants    S.R.Lines    93.6.28
 Supplements/replaces numerical constants defined in generic Unix math.h
 *****************************************************************************/
#ifndef unDefNum_h__
#define unDefNum_h__

#define UN_SQRT2	(1.414213562373095049)
#define UN_SQRT2_INV	(0.707106781186547524)
#define UN_SQRT3	(1.73205080756887720)
#define UN_SQRT3_INV	(0.57735026918962584)
#define UN_COS_30	(8.66025403784439e-1)		/* sqrt(3)/2 = sin60 */

#define UN_LOG2  	(0.69314718056)

#define UN_PI		(3.14159265358979323846)	/* p = pi	     */
#define UN_PI_HALF	(1.57079632679489661923)	/* p/2/3	     */
#define UN_PI_THIRD	(UN_PI/3.00000000000000)	/* p/3		     */
#define UN_PI_SQR	(9.86960440108035861881)	/* p^2 OR p*p	     */
#define UN_PI_CUB	(3.10062766802998201755e1)	/* p^3 OR p*p*p      */
#define UN_PI_TOE	(2.24591577183610454734e1)	/* p^e		     */
#define UN_PI_SQRT	(1.77245385090551602730)	/* sqrt(p)	     */
#define UN_PI_CBRT	(1.46459188756152326302)	/* cubrt(p) = p^(1/3)*/
#define UN_PI_INV	(3.18309886183790671538e-1)	/* 1.0/p	     */
#define UN_PI_TOE_INV	(4.45252672269229061514e-2)	/* 1.0/UN_PI_TOE     */
#define UN_PI_LN	(1.14472988584940017414)	/* ln(p) (nat. log.) */
#define UN_PI_LOG	(4.97149872694133854351e-1)	/* log(p) (log. 10)  */
#define UN_PI_LOG2	(UN_PI_LN/UN_LOG2)		/* log(p) (log. 10)  */


#define UN_E		(2.71828182845904523536)	/* base of nat. log. */
#define UN_E_SQR	(7.38905609893065022723)
#define UN_E_CUB	(2.00855369231876677409e1)

#define UN_EULER_C	(5.77215664901532860607e-1)


/***************************************************
#define UN_EULER_C	5.77215 66490 15328 60607e-1
****************************************************/


#endif	/* unDefNum_h__ */


