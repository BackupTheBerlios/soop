
#include <math.h>

#include <libsclient.h>
#include <geometry.h>
#include <position.h>

#define line_eq(l)                 (Lines[l])
#define is_vertical(l)             (((l) == LP_L) || ((l) == LP_R))

#define inside(p)   (in_field(p.x, p.y))

Bool one_flag_and_line(PosState *pstate, int guess_inside)
{
  /* (f_x,f_y) are the xy-coordinates of the flag that I see
     d is the distances to the flag,  and f_angle is the angle
     We see one line only (so we can conclude that we are inside
     the field */
  
  Pos p1,p2;
  static int l1;
  static int temp_result;
  static double dist,d ; /* distance variables */
  double f_x, f_y, f_d, f_angle,angle,x,y,temp;
  /* we know there is at least one flag.  Use nearest one */
  f_x=Points[seen_points.objects[0].id].x;
  f_y=Points[seen_points.objects[0].id].y;
  f_d=seen_points.objects[0].distance;
  f_angle=seen_points.objects[0].angle;
  dist  = (seen_lines.objects[0]).distance;
  angle = (seen_lines.objects[0]).angle;
  l1 = (seen_lines.objects[0]).id;
  /* now calculate the perpendicular distance to l1 */  
  d = perpend_dist_from_line(dist,angle);
  if (is_vertical(l1)) /* if the line we see is vertical then we can find 
			  the x-coordinate */
    {
      if (line_eq(l1) > 0) /* choose the inside point */
	x = line_eq(l1) - d ;
      else x = line_eq(l1) + d ;
      p1.x = p2.x = x ;
      temp = (f_d*f_d) - ((x-f_x)*(x-f_x)) ;
      if ((temp < 0)&&(temp > -1))
	temp = 0 ;
      if (temp < 0)
	return(FALSE);
      p1.y = f_y + sqrt(temp);
      p2.y = f_y - sqrt(temp);
      temp_result = common_sense_disambig(p1, p2, guess_inside, pstate);
      if (  (KNOWN_INSIDE   == temp_result))
	  return TRUE;
    }
  else  /* if the line we see is horizontal then we can find 
	   the y-coordinate */
    {
      if (line_eq(l1) > 0) /* choose the inside point */
	y = line_eq(l1) - d ;
      else y = line_eq(l1) + d ;
      p1.y = p2.y = y ;
      temp = (f_d*f_d) - ((y-f_y)*(y-f_y));
      if ((temp < 0)&&(temp > -1))
	temp = 0 ;
      if (temp < 0)
	return(FALSE);
      p1.x = f_x + sqrt(temp);
      p2.x = f_x - sqrt(temp);
      temp_result = common_sense_disambig(p1, p2, guess_inside, pstate);
      if (  (KNOWN_INSIDE   == temp_result))
	return TRUE;
    }  
  return FALSE;
}



