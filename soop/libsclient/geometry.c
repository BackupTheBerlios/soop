/* File   : geometry.c
   Info   : useful functions
   Author : Ali Erdem, Yaser Al-Onaizan (some by Gal Kaminka)
   Date   : June 27, 1997 and on
*/

#define  __GEO_EXTERN
#include <geometry.h>
#include <position.h>

Bool estimate_object_pos(double my_x, double my_y, double my_dir, double object_dist, double object_dir, PosState *obj_info)
{


  (obj_info->pos).x = my_x + object_dist * cos(Deg2Rad(my_dir+object_dir));
  (obj_info->pos).y = my_y + object_dist * sin(Deg2Rad(my_dir+object_dir));


  return TRUE;
}

double NormalizeAngle(double angle) {

   double retval;

   /* first make sure |retval| < 360 */
   retval = angle - (int)(angle/360.)*360;
   if (retval > 180.0) {
      retval -= 360.0;
   } else if (retval < -180.0) {
      retval += 360.0;
   }
   return retval;
}

void location(double current_x, double current_y, double dest_x,
	     double dest_y, double angle, 
	     double *turn_angle, int *power)
{
  /* This function computes the angle you need to turn and the approximate power (LOW/HIGH)
     you need to move from the point (current_x,current_y) to the point (dest_x,dest_y) and 
     your heading direction of angle */ 

  double x_diff,y_diff, temp;

#define LOW   40
#define HIGH 100


  x_diff = dest_x - current_x ;
  y_diff = dest_y - current_y ;
  
  /* fprintf(stderr,"xdiff ydiff: %2.1lf %2.1lf\n", x_diff, y_diff); */

  temp = sqrt(x_diff*x_diff + y_diff*y_diff) ;

  if (EPSILON < temp)
    {
      if (temp > 20 ) 
	*power = HIGH ;
      else
	*power = LOW;
      
      *(turn_angle) = ((atan2 (y_diff ,x_diff) * (180.0 / 3.1415)) -  angle) ; 

      if (abs(*turn_angle) > 180.0)
	{   /* angles > 180 is converted to smaller opposite angles */
	  *turn_angle = (360.0 - abs(*turn_angle)) * ((*turn_angle > 0 )? -1 : 1) ;
	}
    }
  else
    {
      *power = 0;
      *turn_angle = 0.0;
    }

   *turn_angle = (double) (int) *turn_angle;

  /* fprintf(stderr,"location says: %2.1lf %d\n",*turn_angle, *power); */

  return ;
}

