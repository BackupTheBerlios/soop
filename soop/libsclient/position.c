/*
 *	Header:
 *	File: position.c
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *      Modified by : Jafar Adibi
 *      Date : 14/3/98
 *	EndHeader:
 */

#include <stdio.h>
#include <math.h>
#include <libsclient.h>
#include <geometry.h>

#define __position_extern_
#include <position.h>

#define calc_angle(my_pos, other_pos, theta)  (NormalizeAngle(Rad2Deg(atan2(other_pos.y-my_pos.y,other_pos.x-my_pos.x))-theta))


double Lines[NUM_LINES] = {
  POS_ERR,  /* LP_Unknown */
  (-PITCH_LENGTH/2.0), /* LP_L */
  (-PITCH_WIDTH/2.0),  /* LP_T */
  (PITCH_WIDTH/2.0),   /* LP_B */
  (PITCH_LENGTH/2.0)  /* LP_R */
};

Pos Points[NUM_POINTS] = 
{
    {POS_ERR, POS_ERR},                                       /* FP_Unknown */
    
    {-PITCH_LENGTH/2.0, -PITCH_WIDTH/2.0},           /* FP_L_T */
    {-PITCH_LENGTH/2.0, PITCH_WIDTH/2.0},           /* FP_L_B */
    {PITCH_LENGTH/2.0, -PITCH_WIDTH/2.0},            /* FP_R_T */
    {PITCH_LENGTH/2.0, PITCH_WIDTH/2.0},             /* FP_R_B */
    {0.0, -PITCH_WIDTH/2.0},                         /* FP_C_T */
    {0.0, PITCH_WIDTH/2.0},                          /* FP_C_B */
    {-PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, -PENALTY_AREA_WIDTH/2.0},  /* FP_P_L_T*/
    {-PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, 0.0},  /* FP_P_L_C*/
    {-PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH,  
     PENALTY_AREA_WIDTH/2.0},                        /* FP_P_L_B*/
    {PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH,   
     -PENALTY_AREA_WIDTH/2.0},                       /* FP_P_R_T*/
    {PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, 0.0},   /* FP_P_R_C */
    {PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH,  
     PENALTY_AREA_WIDTH/2.0},                        /* FP_P_R_B */
    
    {0.0,0.0},    /* Center Mark */                 /* FP_C */
		     
    /* Goal Posts */
    
    {-PITCH_LENGTH/2.0, -GOAL_WIDTH/2.0},   /* FP_G_L_T*/
    {-PITCH_LENGTH/2.0, GOAL_WIDTH/2.0},     /* FP_G_L_B*/
    {PITCH_LENGTH/2.0, -GOAL_WIDTH/2.0},    /* FP_G_R_T */
    {PITCH_LENGTH/2.0, GOAL_WIDTH/2.0},      /* FP_G_R_B */
    
    /* Flags outside of the pitch */
    
    {-10,-PITCH_WIDTH/2.0 - 5},         /* FP_T_L_10 */
    {-20,-PITCH_WIDTH/2.0 - 5},         /* FP_T_L_20 */
    {-30,-PITCH_WIDTH/2.0 - 5},         /* FP_T_L_30 */
    {-40,-PITCH_WIDTH/2.0 - 5},         /* FP_T_L_40 */
    {-50,-PITCH_WIDTH/2.0 - 5},         /* FP_T_L_50 */
    {10,-PITCH_WIDTH/2.0 - 5},          /* FP_T_R_10 */
    {20,-PITCH_WIDTH/2.0 - 5},          /* FP_T_R_20 */
    {30,-PITCH_WIDTH/2.0 - 5},          /* FP_T_R_30 */
    {40,-PITCH_WIDTH/2.0 - 5},          /* FP_T_R_40 */
    {50,-PITCH_WIDTH/2.0 - 5},          /* FP_T_R_50 */
    {-10,PITCH_WIDTH/2.0 + 5},          /* FP_B_L_10 */
    {-20,PITCH_WIDTH/2.0 + 5},          /* FP_B_L_20 */
    {-30,PITCH_WIDTH/2.0 + 5},          /* FP_B_L_30 */
    {-40,PITCH_WIDTH/2.0 + 5},          /* FP_B_L_40 */
    {-50,PITCH_WIDTH/2.0 + 5},          /* FP_B_L_50 */
    {10,PITCH_WIDTH/2.0 + 5},           /* FP_B_R_10 */
    {20,PITCH_WIDTH/2.0 + 5},           /* FP_B_R_20 */
    {30,PITCH_WIDTH/2.0 + 5},           /* FP_B_R_30 */
    {40,PITCH_WIDTH/2.0 + 5},           /* FP_B_R_40 */
    {50,PITCH_WIDTH/2.0 + 5},           /* FP_B_R_50 */
    {-PITCH_LENGTH/2.0 - 5, -10},       /* FP_L_T_10 */
    {-PITCH_LENGTH/2.0 - 5, -20},       /* FP_L_T_20 */
    {-PITCH_LENGTH/2.0 - 5, -30},       /* FP_L_T_30 */
    {-PITCH_LENGTH/2.0 - 5, 10},        /* FP_L_B_10 */
    {-PITCH_LENGTH/2.0 - 5, 20},        /* FP_L_B_20 */
    {-PITCH_LENGTH/2.0 - 5, 30},        /* FP_L_B_30 */
    {PITCH_LENGTH/2.0 + 5, -10},        /* FP_R_T_10 */
    {PITCH_LENGTH/2.0 + 5, -20},        /* FP_R_T_20 */
    {PITCH_LENGTH/2.0 + 5, -30},        /* FP_R_T_30 */
    {PITCH_LENGTH/2.0 + 5, 10},         /* FP_R_B_10 */
    {PITCH_LENGTH/2.0 + 5, 20},         /* FP_R_B_20 */
    {PITCH_LENGTH/2.0 + 5, 30},         /* FP_R_B_30 */
    
    /* Flags located 5 meters outside from the edge of the pitch. */
    
    {0,-PITCH_WIDTH/2.0 - 5},           /* FP_T_0 */
    {0, PITCH_WIDTH/2.0 + 5},           /* FP_B_0 */
    {-PITCH_LENGTH/2.0 - 5, 0},         /* FP_L_0 */
    {PITCH_LENGTH/2.0 + 5, 0},         /* FP_R_0 */
    
    {-PITCH_LENGTH/2.0, 0.0}, /* LEFT_GOAL_ID */
    {PITCH_LENGTH/2.0, 0.0}   /* RIGHT_GOAL_ID */
};


void init_position_knowledge()
{
  seen_points.num_objects = 0;
  seen_lines.num_objects = 0;

  return ;
}


Bool build_position_knowledge(SeenObj object)
{
  if (!object.inViewConeP)
    return FALSE;

  if (OT_Flag == object.type)
    {
      if (  (object.id.fpos != FP_Unknown)
	  &&(object.dist != DIST_ERR)
	  &&(object.dir  != DIR_ERR))
	{
	  (seen_points.objects[seen_points.num_objects]).id = object.id.fpos;
	  (seen_points.objects[seen_points.num_objects]).distance = object.dist;
	  (seen_points.objects[seen_points.num_objects]).angle = object.dir;

	  seen_points.num_objects++;

	  return TRUE;
	}
    }
  else if (OT_Goal == object.type)
    {
      if (  (S_UNKNOWN != object.id.side)
	  &&(object.dist != DIST_ERR)
	  &&(object.dir  != DIR_ERR))
	{
	  (seen_points.objects[seen_points.num_objects]).distance = object.dist;
	  (seen_points.objects[seen_points.num_objects]).angle = object.dir;
	  
	  if (S_LEFT == object.id.side)
	    {
	      (seen_points.objects[seen_points.num_objects]).id = LEFT_GOAL_ID;
	    }
	  else
	    {
	      (seen_points.objects[seen_points.num_objects]).id = RIGHT_GOAL_ID;
	    }
	  seen_points.num_objects++;
	  
	  return TRUE;
	}
    }
  else if (OT_Line == object.type)
    {
      if (  (LP_Unknown != object.id.lpos)
	  &&(object.dist != DIST_ERR)
	  &&(object.dir  != DIR_ERR))
	{
	  (seen_lines.objects[seen_lines.num_objects]).id = object.id.lpos;
	  (seen_lines.objects[seen_lines.num_objects]).distance = object.dist;
	  (seen_lines.objects[seen_lines.num_objects]).angle = object.dir;

	  seen_lines.num_objects++;

	  return TRUE;
	}
    }

  return FALSE;
}

static int compare_estimates(Estimate *a, Estimate *b)
{
  if ((!a) || (!b))
    fprintf(stderr,"compare_estimates(): comparing nulls.\n");

  return (a->distance)-(b->distance);
}


static void sort_position_knowledge()
{
  extern int compare_estimates(Estimate *,Estimate *); 

  qsort(seen_points.objects, seen_points.num_objects,
	sizeof(Estimate), compare_estimates);

  qsort(seen_lines.objects, seen_lines.num_objects,
	sizeof(Estimate), compare_estimates);


  return ;
}


Bool init_points_locations(Side side)
{
  if (S_UNKNOWN == side)
    return FALSE;

  if (S_RIGHT == side)
    {
      int i;

      for (i=0; i<NUM_POINTS; i++)
	{
	  Points[i].x = -Points[i].x;
	  Points[i].y = -Points[i].y;
	}
    }

  return ;
}

int common_sense_disambig(Pos p, Pos q, int inside_guess, PosState *myPos)
{
  /* returning:
       UNKNOWN_INSIDE   for both points in
       UNKNOWN_OUTSIDE  for both points out
       KNOWN_INSIDE/KNOWN_OUTSIDE for successful disambiguation
  */

  if (UNKNOWN_INSIDE == inside_guess)
    {
      if (  reasonable(q.x, q.y)
	  &&(!reasonable(p.x, p.y)))
	{
	  (myPos->pos).x = q.x;
	  (myPos->pos).y = q.y;
	  
	  return (in_field(q.x, q.y) ? KNOWN_INSIDE : KNOWN_OUTSIDE);
	}
      else if (  (reasonable(p.x, p.y))
	       &&(!reasonable(q.x, q.y)))
	{
	  (myPos->pos).x = p.x;
	  (myPos->pos).y = p.y;
	  
	  return (in_field(p.x, p.y) ? KNOWN_INSIDE : KNOWN_OUTSIDE);
	}
    }
  else if (KNOWN_INSIDE == inside_guess)
    {
      if (  (in_field(q.x, q.y))
	  &&(!in_field(p.x, p.y)))
	{
	  (myPos->pos).x = q.x;
	  (myPos->pos).y = q.y;
	  
	  return KNOWN_INSIDE;
	}
      else if (  (in_field(p.x, p.y))
	       &&(!in_field(q.x, q.y)))
	{
	  (myPos->pos).x = p.x;
	  (myPos->pos).y = p.y;

	  return KNOWN_INSIDE;
	}
    }
  else if (KNOWN_OUTSIDE == inside_guess)
    {
      if (!in_field(q.x, q.y))
	{
	  if (  (!reasonable(p.x, p.y))
	      ||(in_field(p.x, p.y)))
	    {
	      (myPos->pos).x = q.x;
	      (myPos->pos).y = q.y;
	      
	      return KNOWN_OUTSIDE;
	    }
	}

      if (!in_field(p.x, p.y))
	{
	  if (  (!reasonable(q.x, q.y))
	      ||(in_field(q.x, q.y)))
	    {
	      (myPos->pos).x = p.x;
	      (myPos->pos).y = p.y;

	      return KNOWN_OUTSIDE;
	    }
	}
    }

  if (  (fabs(p.x-q.x) <= EPSILON)
      &&(fabs(p.y-q.y) <= EPSILON))
    {
      (myPos->pos).x = (p.x+q.x)/2.0;
      (myPos->pos).y = (p.y+q.y)/2.0;

      return (((in_field(p.x, p.y)) && (in_field(q.x, q.y))) 
	      ? KNOWN_INSIDE 
	      : KNOWN_OUTSIDE);
    }      

  return (((in_field(p.x, p.y)) && (in_field(q.x, q.y))) 
	  ? UNKNOWN_INSIDE 
	  : UNKNOWN_OUTSIDE);
}
 



Bool estimate_current_pos(SeeInfo *sinf, Side side, PosState *pstate)
{
  Bool know_position = FALSE;
  int guess_at_inside;
  int i;

  extern Bool triPoint(PosState *myPos);
  extern Bool better_triPoint(PosState *myPos, int); 
  extern Bool two_flags_and_line(PosState *pos, int);
  extern Bool two_flags(PosState *, int, int, int);
  extern Bool one_flag_and_line(PosState *);

  /* XXX this should be really done from within the parser for effiency */
  init_position_knowledge();
  for(i = 0; i < sinf->n_obj; i++) 
    {
      (void)build_position_knowledge(sinf->obj[i]);
    }
  /* XXX */


  switch (seen_lines.num_objects) {
  case 1:
    guess_at_inside = KNOWN_INSIDE;
    break;

  case 2:
  case 0:
    guess_at_inside = KNOWN_OUTSIDE;
    break;

  default:
    fprintf(stderr,"Seen more than 2 lines!\n");
    guess_at_inside = UNKNOWN_INSIDE;
    break;

  }

  /* if didn't see anything, can't help at all */
  if (  (!seen_points.num_objects)
      &&(!seen_lines.num_objects))
    return FALSE;
  
  /* sort by distance */
  sort_position_knowledge();

  /* now infer position */

  if (2 < seen_points.num_objects)
    know_position = informed_triPoint(pstate, guess_at_inside);

  if (!know_position) /* == 3 > seen_points.num_objects */
    {
      if (  (1 < seen_points.num_objects) /* at least two flags */
	  &&(0 < seen_lines.num_objects))     /* and at least one flag */
	{
	  know_position = two_flags_and_line(pstate, guess_at_inside);
	}

      if (!know_position) /* two_flags_and_line failed, or never called */
	{
	  if (1 < seen_points.num_objects)
	    {
	      /* call two_flags using best two flags we have */
	      know_position = two_flags(pstate, guess_at_inside, 0, 1);
	    }

	  if (!know_position)
	    {
	      know_position = one_flag_and_line(pstate);

	      if (!know_position)
		{
		  /* other cases */
		}
	    }

	}
    }

  if (know_position)
    {
      if (seen_points.num_objects)
	{
	  /* turns out if you look at closest object for angle,
	     error increases dramatically.  We should be looking at the
	     object furthest away from us */
	  pstate->dir = calc_angle(pstate->pos, 
				   Points[seen_points.objects[seen_points.num_objects-1].id], 
				   seen_points.objects[seen_points.num_objects-1].angle);
	}
      /* else with line? */
    }

  return know_position;
}





















