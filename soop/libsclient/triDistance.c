/*
 *	Header:
 *	File: triDistance.c
 *	Author: Jafar Adibi
 *	Date: 5/14/98
 *	EndHeader:
 */

#include <stdio.h>
#include <math.h>
#include <libsclient.h>

#define CLOSE 3


/* This function computes the global position of (x,y) based on the distnace to
 * (x1,y1)=d1 and (x2,y2) = d2 , it returns 2 points (x,y) and (p,q)
 * I use of a rotation of x2  around x1  and a relation to get to the x */

int triDistance (double x1, double y1,double d1,double x2, double y2,double d2,double *x,double *y,double *p,double *q)

{
  double cos_1, sin_1, cos_2, sin_2, dd, d, temp, xt1, xt2, yt1, yt2;    /* dd = d*d */

   
  /*   fprintf(stdout,"triDistance function \n");*/
  dd = (x2-x1)* (x2-x1) + (y2-y1) * (y2-y1);
 
  d = sqrt(dd);
 
  if (d == 0 ){
    return(FALSE);
  }

  else if( d1 == 0 ) {
    *x = x1;
    *y = y1;
    
    *p = x1;
    *q = y1;
  }

  else if (d2 == 0) {
    *x = x2;
    *y = y2;
    
    *p = x2;
    *q = y2;
  }
  
  else{
    
    cos_1 = (d1*d1 + dd - d2*d2)/(2.0 * d * d1);
    if (cos_1 > 1) {
      cos_1 = 1;
    }
    else if (cos_1 < -1) {
      cos_1 = -1;
    }
    
 
    sin_1 = sqrt( 1 - (cos_1 * cos_1));
 
 
    
    x2 = x2 - x1;  /* change the center to x1 */
    y2 = y2 - y1;
    
    
    
    xt1 = (d1 / d) * ((x2 * cos_1) + (y2 * sin_1));
    yt1 =  (d1 / d) * ((-x2 * sin_1) + (y2 * cos_1));

    
    xt2 = (d1 / d) * ((x2 * cos_1) + (-y2 * sin_1));
    yt2 =  (d1 / d) * ((x2 * sin_1) + (y2 * cos_1));
 
    xt1 = xt1 + x1;
    yt1 = yt1 + y1;

    xt2 = xt2 + x1;
    yt2 = yt2 + y1;


    *x = xt1;
    *y = yt1;
    
    *p = xt2;
    *q = yt2; 
    

  }

    return(TRUE);
 
}








/*
void main()
{

  double x, y, p, q, x1, y1, x2, y2, d1, d2;
  
  for(; ; ){
      printf("\n x1 =");
    scanf("%lf", &x1);
    
    printf("\n y1 =");
    scanf("%lf", &y1);  

    printf("\n d1 =");
    scanf("%lf", &d1);


    printf("\n x2 =");
    scanf("%lf", &x2);

    printf("\n y2 =");
    scanf("%lf", &y2);

    printf("\n d2 =");
    scanf("%lf", &d2);

    
    
    if (triDistance (x1, y1, d1, x2, y2, d2,  &x, &y, &p, &q) == TRUE){
      printf("\n X-1 = %lf Y-1 = %lf  \n",x,y);
      printf("\n X-2 = %lf Y-2 = %lf  \n",p,q);
      
    }
    else printf("\n Failed to compute it ...\n");
    
  }
}


*/

















