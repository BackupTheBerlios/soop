
#include <math.h>

#include <libsclient.h>
#include <geometry.h>
#include <position.h>

#define line_eq(l)                 (Lines[l])
#define is_vertical(l)             (((l) == LP_L) || ((l) == LP_R))

static Bool line_equation_of(int l1, double d1, int l2,double d2, double *line)
     /* This function finds the line equation that satisfy the following
	constraint:
	it is of distance d1 from l1, and d2 from l2 ;
	note that l1&l2 are supposed to be parallel */
     
{
  double line1,line2,line3,line4 ;
  
  line1 = line_eq(l1) - d1 ;
  line2 = line_eq(l1) + d1 ;
  line3 = line_eq(l2) - d2 ;
  line4 = line_eq(l2) + d2 ;
  
  if (semi_equal(line1,line3,EPSILON) ||
      semi_equal(line1,line4,EPSILON))
    *line = line1;
  else if (semi_equal(line2,line3,EPSILON) ||
	   semi_equal(line2,line4,EPSILON))
    *line = line2;
  else
    return FALSE;

  return TRUE;
}


static Bool is_perpend(int l1, int l2)
{
  /* This function checks if the l1 & l2 are perpendicular -
     l1,l2 arenames of lines (e.g. For LP_T & LP_B function return FALSE )*/
  
 if (((l1 == LP_T || l1 == LP_B) &&
      (l1 == LP_T || l1 == LP_B)) ||
     ((l2 == LP_L || l1 == LP_R) &&
      (l2 == LP_L || l1 == LP_R)))
   return(TRUE);
 else return(FALSE);
}



static Bool collide(double x, double l, double d)
{
  /* This function checks if the line parallel to l and of distance d
     (notice that there are two possible lines of distance d from l)
     has the collides with the line x (i.e they are the same line within 
     a certain EPSILON*/
  
  
  if (semi_equal(x,(l+d),EPSILON) ||
      semi_equal(x,(l-d),EPSILON))
    return(TRUE);
  else return(FALSE);
}

  
static void get_four_points(int l1, double d1, int l2, double d2,
		     Pos *p1, Pos *p2, Pos *p3, Pos *p4)
{
  /* This function takes two lines parallel two l1 and of distance d1 from l1,
     and two lines parallel two l2 and of distance d2 of l2 
     The function returns the four intersection points of these lines (i.e.
     the heads of the triangle */

  double x1,x2,y1,y2 ;
  if (is_vertical(l1)) /* of l1 is vertical then it should give you the x-coord */
    {
      x1 = line_eq(l1) - d1 ;
      x2 = line_eq(l1) + d1 ;
      y1 = line_eq(l2) - d2 ;
      y2 = line_eq(l2) + d2 ;
    }
  else {
      y1 = line_eq(l1) - d1 ;
      y2 = line_eq(l1) + d1 ;
      x1 = line_eq(l2) - d2 ;
      x2 = line_eq(l2) + d2 ;
  }
  
  p1->x = x1 ;
  p1->y = y1 ;
  p2->x = x1 ;
  p2->y = y2 ;
  p3->x = x2 ;
  p3->y = y1 ;
  p4->x = x2 ;
  p4->y = y2 ;

  return ;
}

Bool two_flags_and_line(PosState *pstate, int guess_at_inside)
{
  /* (f1_x,f1_y) & (f2_x,f2_y) are the xy-coordinates of the flag that I see
     d1,d2 are the distances to the flags angle1,angle2 are the two angles to 
     the flags  */
  
  Pos p,p1,p2,four_points[4] ;
  int f1=0,f2=0,i=0;
  int num_lines = seen_lines.num_objects;
  static double dist1, dist2;
  static int l1, l2;
  static double d1,d2,d ; /* distance variables */
  static double d_vert,d_horiz ; /* Vertical and horizontal distances */
  static double l_horiz,l_vert ; /* vertical and horizontal line equations */
  static double l;
  double x, y;
  double f1_x, f1_y, f_d1, angle1, f2_x, f2_y, f_d2, angle2;

  /* we know there are at least two flags.  Use nearest ones */

  f1_x=Points[seen_points.objects[0].id].x;
  f1_y=Points[seen_points.objects[0].id].y;
  f_d1=seen_points.objects[0].distance;
  angle1=seen_points.objects[0].angle;

  f2_x=Points[seen_points.objects[1].id].x;
  f2_y=Points[seen_points.objects[1].id].y;
  f_d2=seen_points.objects[1].distance;
  angle2=seen_points.objects[1].angle;

  /* from two flags you can find two possible points: 
     First calculate P1:(x1,y1 & P2:(x2,y2) then use 
     any lines that you can see to dismbigute */
  
  /* Calculate P1 & P2 here  ...... */
  triDistance(f1_x,f1_y,f_d1,f2_x,f2_y,f_d2,&p1.x,&p1.y,&p2.x,&p2.y);

  /* l1 used as temp here */
  l1 = common_sense_disambig(p1, p2, guess_at_inside, pstate);

  if (  (KNOWN_INSIDE == l1)
      ||(KNOWN_OUTSIDE == l1))
    {
      return TRUE;
    }

  /* Now I have P1&P2 but I want to disambiguate */
  
  if (num_lines == 2) /* I can see two lines l1 & l2 */
    {
      /* set distance to l1 is dist1, and to l2 is dist2 
	 and the angle to l1 is angle1, and to l2 is angle2 */

      dist1  = (seen_lines.objects[0]).distance;
      dist2  = (seen_lines.objects[1]).distance;
      angle1 = (seen_lines.objects[0]).angle;
      angle2 = (seen_lines.objects[1]).angle;
      l1 = (seen_lines.objects[0]).id;
      l2 = (seen_lines.objects[1]).id;
      d1,d2,d ; /* distance variables */
      d_vert,d_horiz ; /* Vertical and horizontal distances */
      l_horiz,l_vert ; /* vertical and horizontal line equations */

      /* now calculate the perpendicular distance to l1 and l2 */  
      d1 = perpend_dist_from_line(dist1,angle1);
      d2 = perpend_dist_from_line(dist2,angle2);

      if (is_perpend(l1,l2)) /* the two lines are perpendicular to each other.
				the info from these two lines give four possi-
				ble points, hopefully only one will intersect
				with the other two that were calculated */
	{
	  get_four_points(l1,d1,l2,d2,
			  &(four_points[0]),
			  &(four_points[1]),
			  &(four_points[2]),
			  &(four_points[3]));

	  for (i = 0 ; i < 4  ; i++) {
	    if (semi_equal(four_points[i].x,p1.x,EPSILON)&&
		semi_equal(four_points[i].y,p1.y,EPSILON))
	      f1++;
	    if (semi_equal(four_points[i].x,p2.x,EPSILON)&&
		semi_equal(four_points[i].y,p2.y,EPSILON))
	      f2++;
	  }
	  if (f1) /* there is an intersection between the four_points and P1 */
	    if (f2) /* check if there is intersection with P2 also */
	      return(FALSE); /* if both P1&P2 intersects - then I can't disambiguate */
	    else { /* only P1 intersects */
	      x = p1.x;
	      y = p1.y;
	    }
	  else if (f2) { /* only P2 intersects */
	    x = p2.x;
	    y = p2.y;
	  }
	  else return(FALSE); /* none intersects */
	}
      else /* l1 & l2 are parallel */
	{
	  if (!line_equation_of(l1,d1,l2,d2,&l))
	    return FALSE;

	  if (is_vertical(l1)) /* both lines are vertical lines */
	    {
	      if (semi_equal(p1.x,l,EPSILON)) /* P1 is on line l */
		if (semi_equal(p2.x,l,EPSILON)) /* but also P2 is on line l */
		  /* then both P1 & P2 are possible - what can I do? 
		     For now just return FALSE */ 
		  return(FALSE);
		else { /* p1 is on l, but p2 is not, then pick p1 */
		  x = p1.x ;
		  y = p1.y ;
		}
	      else /* P1 is not on line l */
		if (semi_equal(p2.x,l,EPSILON)) /* but P2 is on line l */
		  { /* p1 is not on l but p2 is, then pick P2 */
		    x = p2.x ;
		    y = p2.y ;
		  }
		else /* both p1&p2 are not on l */
		  return(FALSE);
	    }
	  else /* both lines are vertical */ 
	    {
	      if (semi_equal(p1.y,l,EPSILON)) /* P1 is on line l */
		if (semi_equal(p2.y,l,EPSILON)) /* but also P2 is on line l */
		  /* then both P1 & P2 are possible - what can I do? 
		     For now just return FALSE */ 
		  return(FALSE);
		else { /* p1 is on l, but p2 is not, then pick p1 */
		  x = p1.x ;
		  y = p1.y ;
		}
	      else /* P1 is not on line l */
		if (semi_equal(p2.y,l,EPSILON)) /* but P2 is on line l */
		  { /* p1 is not on l but p2 is, then pick P2 */
		    x = p2.x ;
		    y = p2.y ;
		  }
		else /* both p1&p2 are not on l */
		  return(FALSE);
	    } /* end of both lines are vertical */	  
	} /* end of that lines are parallel */
    } /* end of num_lines == 2 */
  else if (num_lines == 1) /* I can see only one line */
    {
      d1  = (seen_lines.objects[0]).distance;
      angle1 = (seen_lines.objects[0]).angle;
      l1 = (seen_lines.objects[0]).id;
      
      if (is_vertical(l1)) /* is l1 a vertical line */
	{
	  if (collide(p1.x,line_eq(l1),d1)) /* P1  lies on line of distance d1 from l1 */
	    if (collide(p2.x,line_eq(l1),d1)) /* also P2 lies there */
	      /* both P1 & P2 are possible - For now return FALSE */
	      return(FALSE);
	    else  /* only P1 is possible */
	      {
		x = p1.x ;
		y = p1.y ;
	      }
	  else /* P1 is no possible */
	    if (collide(p2.x,line_eq(l1),d1)) /*  P2 lies there */
	      {
		x = p2.x ;
		y = p2.y ;
	      }
	    else /* both P1 & P2 do not lie on l1 - then both are not possible*/
	      return(FALSE);
	} /* end of l1 vertical */
      else /* l1 is horizontal */
	{
	  if (collide(p1.y,line_eq(l1),d1)) /* P1  lies on line of distance d1 from l1 */
	    if (collide(p2.y,line_eq(l1),d1)) /* also P2 lies there */
	      /* both P1 & P2 are possible - For now return FALSE */
	      return(FALSE);
	    else  /* only P1 is possible */
	      {
		x = p1.x ;
		y = p1.y ;
	      }
	  else /* P1 is no possible */
	    if (collide(p2.y,line_eq(l1),d1)) /*  P2 lies there */
	      {
		x = p2.x ;
		y = p2.y ;
	      }
	    else /* both P1 & P2 do not lie on l1 - then both are not possible*/
	      return(FALSE);
	} /* end of l1 horizontal */
    } /* end of num_lines == 1 */
  else { /* num_lines seen == 0 */
    return(FALSE);
  }

  (pstate->pos).x = x;
  (pstate->pos).y = y;

  return TRUE;
}


Bool two_flags(PosState *myPos, int inside_guess, int flag1_index, int flag2_index) 
{
  static double x1,x2,y1,y2,x3,y3,dis1,dis2,dis3;
  static Pos p, q;
  static int i;
  static int id1,id2,id3;
  static int maxPoints;

  if (  (flag1_index >= seen_points.num_objects)
      ||(flag2_index >= seen_points.num_objects))
    return FALSE;

  id1=seen_points.objects[flag1_index].id;
  x1=Points[id1].x;
  y1=Points[id1].y;
  dis1=seen_points.objects[flag1_index].distance;

  id2=seen_points.objects[flag2_index].id;
  x2=Points[id2].x;
  y2=Points[id2].y;
  dis2=seen_points.objects[flag2_index].distance;
  
  triDistance(x1,y1,dis1,x2,y2,dis2,&(p.x),&(p.y), &(q.x), &(q.y));

  i = common_sense_disambig(p, q, inside_guess, myPos);

  if (  (KNOWN_INSIDE  == i)
      ||(KNOWN_OUTSIDE == i))
    return TRUE;

  return FALSE;
}

