/*
 *	Header:
 *	File: position.h
 *	Author: Yaser Al-Onaizan, Jafar Adibi, Jonatha Zqiu, GalK
 *	Date: 1997/04/1
 *      Modified by : Jafar Adibi
 *      Date : 14/3/98
 *	EndHeader:
 */


#ifndef __position_h
#define __position_h

#ifndef __position_extern_
#define __position_extern_ extern
#endif

#define NUM_POINTS (NUM_FLAGS+NUM_GOALS)

typedef struct {
  int id;   /* What did we see? */
  double distance;
  double angle;
} Estimate;

#define LEFT_GOAL_ID     NUM_FLAGS
#define RIGHT_GOAL_ID  (NUM_FLAGS+1)

__position_extern_ struct {
  Estimate objects[NUM_POINTS];  /* +2 for goals */
  int num_objects;
} seen_points;

__position_extern_ struct {
  Estimate objects[NUM_LINES];
  int num_objects;
} seen_lines;
  
#define UNKNOWN_POSITION   0
#define KNOWN_INSIDE       1
#define UNKNOWN_INSIDE     2
#define KNOWN_OUTSIDE     -1
#define UNKNOWN_OUTSIDE   -2

#define reasonable_x(x)    (((x)<((PITCH_LENGTH/2.0) + 5.0)) && ((x)>((-PITCH_LENGTH/2.0) - 5.0)))
#define reasonable_y(x)    (((x)<((PITCH_WIDTH/2.0) + 5.0)) && ((x)>((-PITCH_WIDTH/2.0) - 5.0)))

#define in_field_x(x)      (((x)<=(PITCH_LENGTH/2.0)) && ((x)>=(-PITCH_LENGTH/2.0)))
#define in_field_y(x)      (((x)<=(PITCH_WIDTH/2.0)) && ((x)>=(-PITCH_WIDTH/2.0)))

#define in_field(x,y)      (in_field_x(x) && in_field_y(y))
#define reasonable(x,y)    (reasonable_x(x) && reasonable_y(y))

#define EPSILON   3.0
#define semi_equal(n1,n2,epsilon)  (fabs((n1)-(n2)) < (epsilon))

__position_extern_ int triDistance (double, double, double, double, double, double, double *, double *, double *, double *);

__position_extern_ Pos Points[];
__position_extern_ double Lines[];

__position_extern_ void init_position_knowledge(void);
__position_extern_ Bool build_position_knowledge(SeenObj);
__position_extern_ Bool init_points_locations(Side);
__position_extern_ Bool estimate_current_pos(SeeInfo *, Side, PosState *);
__position_extern_ int common_sense_disambig(Pos, Pos, int, PosState *);



#endif
















