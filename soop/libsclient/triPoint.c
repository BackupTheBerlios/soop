#include <libsclient.h>
#include <geometry.h>
#include <position.h>

/* This function computes the 2 closest points out of four points*/

static Bool bestPoint (double x1, double y1,double p1,double q1,double x2,double y2,double p2,double q2, double *x, double *y)

{
  static double dist[4];
  static double min;
  static int i, item;

  dist[0] = sqrt((x2-x1) * (x2-x1) + (y2 - y1)* (y2-y1));

  dist[1] = sqrt((x2-p1) * (x2-p1) + (y2 - q1)* (y2-q1));

  dist[2] = sqrt((p2-x1) * (p2-x1) + (q2 - y1)* (q2-y1));
  
  dist[3] = sqrt((p2-p1) * (p2-p1) + (q2 - q1)* (q2-q1));

  item = 0;
  min = dist[0];
  for (i=1;
       i< 4; 
       i++)
    {
      if (dist[i] <= min)
	{
	  item = i;
	  min = dist[i];
	}
    }

  if( item == 0 || item == 2) {
    *x = x1;
    *y = y1;
  }

  else if (item == 1 || item ==3) {
    *x = p1;
    *y = q1;
  }

  return(TRUE);
}  


Bool triPoint(PosState *myPos) 
{
  static double x1,x2,y1,y2,x3,y3,dis1,dis2,dis3;
  static double x[2],y[2],p[2],q[2];
  static int i;
  static int id1,id2,id3;
  static int maxPoints;

  maxPoints = seen_points.num_objects;

  id1=seen_points.objects[0].id;
  x1=Points[id1].x;
  y1=Points[id1].y;
  dis1=seen_points.objects[0].distance;

  id2=seen_points.objects[1].id;
  x2=Points[id2].x;
  y2=Points[id2].y;
  dis2=seen_points.objects[1].distance;
  
  for (i=2; i<maxPoints ; i++) {

    id3=seen_points.objects[i].id;
    x3=Points[id3].x;
    y3=Points[id3].y;
    dis3=seen_points.objects[i].distance;

    if (y1==y2) {
      if (y1!=y3) break;
    }
    else if ((x1-x2)*(y1-y3)/(y1-y2) != (x1-x3)) break;
  }

  if (i>=maxPoints) return FALSE;
  
  
  triDistance(x1,y1,dis1,x2,y2,dis2,&x[0],&y[0],&x[1],&y[1]);
  triDistance(x1,y1,dis1,x3,y3,dis3,&p[0],&q[0],&p[1],&q[1]);

  bestPoint(x[0],y[0],x[1],y[1],p[0],q[0],p[1],q[1],&(myPos->pos.x),&(myPos->pos.y));

  return TRUE;
}


Bool better_triPoint(PosState *myPos) 
{
  static double x1,x2,y1,y2,x3,y3,dis1,dis2,dis3;
  static double x[2],y[2],p[2],q[2];
  static Pos estPos1,estPos2;
  static int i;
  static int id1,id2,id3;
  static int maxPoints;

  maxPoints = seen_points.num_objects;

  /* First estimation */

  id1=seen_points.objects[0].id;
  x1=Points[id1].x;
  y1=Points[id1].y;
  dis1=seen_points.objects[0].distance;

  id2=seen_points.objects[1].id;
  x2=Points[id2].x;
  y2=Points[id2].y;
  dis2=seen_points.objects[1].distance;
  
  for (i=2; i<maxPoints ; i++) {

    id3=seen_points.objects[i].id;
    x3=Points[id3].x;
    y3=Points[id3].y;
    dis3=seen_points.objects[i].distance;

    if (y1==y2) {
      if (y1!=y3) break;
    }
    else if ((x1-x2)*(y1-y3)/(y1-y2) != (x1-x3)) break;
  }

  if (i>=maxPoints) return FALSE;
  
  
  triDistance(x1,y1,dis1,x2,y2,dis2,&x[0],&y[0],&x[1],&y[1]);
  triDistance(x1,y1,dis1,x3,y3,dis3,&p[0],&q[0],&p[1],&q[1]);

  bestPoint(x[0],y[0],x[1],y[1],p[0],q[0],p[1],q[1],&estPos1.x,&estPos1.y);


  /* Second estimation */

  id2=seen_points.objects[2].id;
  x2=Points[id2].x;
  y2=Points[id2].y;
  dis2=seen_points.objects[2].distance;
  
  for (i=3; i<maxPoints ; i++) {

    id3=seen_points.objects[i].id;
    x3=Points[id3].x;
    y3=Points[id3].y;
    dis3=seen_points.objects[i].distance;

    if (y1==y2) {
      if (y1!=y3) break;
    }
    else if ((x1-x2)*(y1-y3)/(y1-y2) != (x1-x3)) break;
  }

  if (i>=maxPoints) {
    myPos->pos.x=estPos1.x;
    myPos->pos.y=estPos1.y;
    return TRUE;
  }
  
  triDistance(x1,y1,dis1,x2,y2,dis2,&x[0],&y[0],&x[1],&y[1]);
  triDistance(x1,y1,dis1,x3,y3,dis3,&p[0],&q[0],&p[1],&q[1]);

  bestPoint(x[0],y[0],x[1],y[1],p[0],q[0],p[1],q[1],&estPos2.x,&estPos2.y);

  myPos->pos.x=(estPos1.x+estPos2.x)/2;
  myPos->pos.y=(estPos1.y+estPos2.y)/2;
  
  return TRUE;
}


Bool informed_triPoint(PosState *myPos, int guess_at_inside) 
{
  static double x1,x2,y1,y2,x3,y3,dis1,dis2,dis3;
  static double x[2],y[2],p[2],q[2];
  static Pos estPos1,estPos2;
  static int i;
  static int id1,id2,id3;
  static int maxPoints;

  maxPoints = seen_points.num_objects;

  /* First estimation */

  id1=seen_points.objects[0].id;
  x1=Points[id1].x;
  y1=Points[id1].y;
  dis1=seen_points.objects[0].distance;

  id2=seen_points.objects[1].id;
  x2=Points[id2].x;
  y2=Points[id2].y;
  dis2=seen_points.objects[1].distance;
  
  for (i=2; i<maxPoints ; i++) {

    id3=seen_points.objects[i].id;
    x3=Points[id3].x;
    y3=Points[id3].y;
    dis3=seen_points.objects[i].distance;

    if (y1==y2) {
      if (y1!=y3) break;
    }
    else if ((x1-x2)*(y1-y3)/(y1-y2) != (x1-x3)) break;
  }

  if (i>=maxPoints) return FALSE;
  
  
  triDistance(x1,y1,dis1,x2,y2,dis2,&x[0],&y[0],&x[1],&y[1]);
  triDistance(x1,y1,dis1,x3,y3,dis3,&p[0],&q[0],&p[1],&q[1]);

  bestPoint(x[0],y[0],x[1],y[1],p[0],q[0],p[1],q[1],&estPos1.x,&estPos1.y);


  /* Second estimation */

  id2=seen_points.objects[2].id;
  x2=Points[id2].x;
  y2=Points[id2].y;
  dis2=seen_points.objects[2].distance;
  
  for (i=3; i<maxPoints ; i++) {

    id3=seen_points.objects[i].id;
    x3=Points[id3].x;
    y3=Points[id3].y;
    dis3=seen_points.objects[i].distance;

    if (y1==y2) {
      if (y1!=y3) break;
    }
    else if ((x1-x2)*(y1-y3)/(y1-y2) != (x1-x3)) break;
  }

  if (i>=maxPoints) {
    myPos->pos.x=estPos1.x;
    myPos->pos.y=estPos1.y;
    return TRUE;
  }
  
  triDistance(x1,y1,dis1,x2,y2,dis2,&x[0],&y[0],&x[1],&y[1]);
  triDistance(x1,y1,dis1,x3,y3,dis3,&p[0],&q[0],&p[1],&q[1]);

  bestPoint(x[0],y[0],x[1],y[1],p[0],q[0],p[1],q[1],&estPos2.x,&estPos2.y);

  i = common_sense_disambig(estPos1, estPos2, guess_at_inside, myPos);

  if (  (UNKNOWN_INSIDE == i)
      ||(UNKNOWN_OUTSIDE == i))
    {
      myPos->pos.x=(estPos1.x+estPos2.x)/2;
      myPos->pos.y=(estPos1.y+estPos2.y)/2;
    }

  return TRUE;
}
