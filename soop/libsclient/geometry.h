

#ifndef __GEO_H
#define __GEO_H

#ifndef __GEO_EXTERN
#define __GEO_EXTERN extern
#endif

#include <math.h>
#include <libsclient.h>

#ifndef M_PI /* in solaris */
#define M_PI (3.1415927)
#endif

#define Rad2Deg(x)  ((double)(x) * 180.0 / M_PI)
#define Deg2Rad(x)  ((double)(x) * M_PI / 180.0)

#define Abs(x)  (((x) < 0.0)?(-(x)):(x))
#define Pow(x)  ((x)*(x))

#define min(a,b)  ((a) > (b) ? (b) : (a))
#define max(a,b)  ((a) < (b) ? (b) : (a))

#define abs(a)    Abs(a)

/* This function caculates the perpendicular distance (projection) 
   from my position to a line, the given angle is the angle between 
   my view cone and the line, the distance is the distance from my
   postion to the line along my view cone  */

#define perpend_dist_from_line(dist, angle)   (dist * (cos(Deg2Rad(90.0-fabs(angle)))))


__GEO_EXTERN Bool estimate_object_pos(double, double, double, double, double, PosState *);
__GEO_EXTERN double NormalizeAngle(double);
__GEO_EXTERN void location(double, double, double, double, double, double *, int *);


#endif
