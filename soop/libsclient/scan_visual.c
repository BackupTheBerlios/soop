#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libsclient.h"

Bool get_msg_info(char *buffer, char *msg_type, char *time);
Bool get_next_obj_info(char *buffer, int *from,int *to,int *pos);
Bool parse_obj_info(char *obj_info, SeenObj *obj, char *teamname);
Bool get_obj_type(char *obj_info, char * obj_type);
Bool get_obj_param(char *obj_info, char * obj_param);
Bool parse_ball_info(char *ball_info, SeenObj *obj);
Bool parse_flag_info(char *flag_info, SeenObj *obj);
Bool parse_goal_info(char *goal_info, SeenObj *obj);
Bool parse_line_info(char *line_info, SeenObj *obj);
Bool parse_player_info(char *player_info, SeenObj *obj,char *teamname);
Bool parse_obj_param(char *obj_param, SeenObj *obj);


void substr(char *str1, char *str2, int from, int to);
void display_visual_info(FILE *fh , SeeInfo *sinfo);

Bool scan_visual_info(char *buffer, SeeInfo *sinfo, char *teamname)
{
  char temp_buff[BUFSIZE1],obj_info[BUFSIZE1],b[BUFSIZE1],*p ;
  int i , j , l , time , from , to , pos,obj_count = 0 ;
  char ch = '\0';

  sinfo->n_obj = 0;
  l = strlen(buffer);
  /*  fprintf(stderr,"Buffer size *********** : \n%d %d\n", l, BUFSIZE1);
  fprintf(stderr,"Buffer : \n%s\n",buffer); */
  sscanf(buffer,"(see %d %c",&(sinfo->time),&ch);
  if ((p =  strchr(buffer+1,'(')) == NULL){
    if (ch == ')') /* see message is terminated with ')' */
      {
	return(TRUE);
      }
    else {
      fprintf(stderr,"scan_visual.c (scan_visual_info): ill-formated see message:%s\n",buffer);
      return(FALSE);
    }
  }
  /* now chop message tag from buffer */
  substr(temp_buff,p,0,strlen(p)-2);
  pos = 0 ;
  while (get_next_obj_info(temp_buff,&from,&to,&pos)){
    substr(obj_info,temp_buff,from,to);
    if (parse_obj_info(obj_info,&(sinfo->obj[obj_count]),teamname)){
      obj_count++;
    }
    else {
      fprintf(stderr,"Failed parsing object info %s\n",obj_info);
    }      
  }
  sinfo->n_obj = obj_count ;
  
  return TRUE ;
}

Bool get_next_obj_info(char *buffer,int *from, int *to, int *pos)
{
  int i = *pos+1,lp=1,rp=0,l=strlen(buffer) ;
  if (buffer[*pos] != '('){
    return FALSE ;
  }
  while((i < l-1) && (rp <  lp)){
    if (buffer[i] == '(' )
      lp++ ;
    else if (buffer[i] == ')')
      rp++;
    i++;
  }
  if (i >= l) return FALSE ;
  *from = *pos ;
  *to = i - 1;
  while ((i < l) && (isspace(buffer[i])))
    {
      i++ ;
    }
  *pos = i ;
  return TRUE ;
  
}


Bool parse_obj_info(char *obj_info,SeenObj *obj, char *teamname)
{ 
  char obj_type[BUFSIZE1],obj_param[BUFSIZE1] ;
  Bool flag = TRUE ;
  int l = strlen(obj_info);


  if (!(get_obj_type(obj_info,obj_type) &&( get_obj_param(obj_info,obj_param)))){
    flag = FALSE ;
  }
  else {
    if (parse_obj_param(obj_param,obj) )
      {	  
	if ((obj_type[0] == 'b') || (obj_type[0] == 'B')) {
	  flag = parse_ball_info(obj_type,obj);
	}
	else if ((obj_type[0] == 'f') || (obj_type[0] == 'F')) {
	  flag = parse_flag_info(obj_type,obj);
	}
	else if ((obj_type[0] == 'g') || (obj_type[0] == 'G')) {
	  flag = parse_goal_info(obj_type,obj);
	}
	else if ((obj_type[0] == 'l') || (obj_type[0] == 'L')) {
	  flag = parse_line_info(obj_type,obj);
	}
	else if ((obj_type[0] == 'p') || (obj_type[0] == 'P')) {
	  flag = parse_player_info(obj_type,obj,teamname);
	}
	else{
	  printf(stderr,"Unreconizable object information: %s\n",obj_type);
	  flag = FALSE ;
	}
      }
    else {
      printf(stderr,"Unreconizable object parameters: %s\n",obj_param);
      flag = FALSE ;
    }    
  }
  return(flag) ;
}



Bool get_obj_type(char *obj_info, char * obj_type)
{
/* obj_info is of the form: "player moh 1) 8.2 5 -0 -0 -61)" 
    "player moh 1" is what to be extracted as obj_type */

  char *p ;
  
  p =  strchr(obj_info,')') ;
  if (p == NULL) { 
    fprintf(stderr,"scan_visual.c:(get_obj_type): Failed to parse: %s\n", obj_info);
    return(FALSE) ;
  }
  substr(obj_type,obj_info,2,p-obj_info-1);
  return TRUE ;
}

Bool get_obj_param(char *obj_info, char * obj_param)
{
/* obj_info is of the form: "player moh 1) 8.2 5 -0 -0 -61)" 
    "8.2 5 -0 -0 -6" is what to be extracted as obj_param */

  char *p ;
  int l = strlen(obj_info);
  
  p =  strchr(obj_info,')') ;
  if (p == NULL) { 
    fprintf(stderr,"scan_visual.c:(get_obj_param): Failed to parse: %s\n",obj_info);
    return(FALSE) ;
  }
  p++;
  while ( (isspace(*p)) && (p <obj_info+l-1)){/*skip white spaces */
    p++;
  } 
  if (p > obj_info + l )
    return FALSE ;
  substr(obj_param,p,0,strlen(p)-2); /* exclude ')' at end of string */
  return TRUE ;
}

  

Bool parse_ball_info(char *ball_info, SeenObj *obj)
{
  obj->type = OT_Ball ;
  if (ball_info[0] == 'b'){
    obj->inViewConeP = TRUE ;
  }
  else if (ball_info[0] == 'B'){
    obj->inViewConeP = FALSE ;
  }
  else {
    return(FALSE);
  }
  return(TRUE);  
}

Bool parse_flag_info(char *flag_info, SeenObj *obj)
{
  char s1[10],s2[10],s3[10],s4[10];
  int n = 0 ;

  s1[0] = s2[0] = s3[0] = s4[0] = '\0' ;
  
  obj->type = OT_Flag;
  if (flag_info[0] == 'f'){
    obj->inViewConeP = TRUE ;
    n = sscanf(flag_info,"%s %s %s %s",s1,s2,s3,s4); /* n : number of parameters+1 */
    obj->id.fpos = FP_Unknown ; /* flag is intially assigned to unknown, so that if
				   if it is not reconized it'll remain unknown */  
    if (n >= 2 ){
      if (s2[0] == 'c'){ /* (flag c ....) */
	if (n == 2){ /* center mark (flag c) */
	  obj->id.fpos = FP_C ;
	}
	else if (s3[0] == 't'){ /* (flag c t) */
	  obj->id.fpos = FP_C_T ;
	}
	else if (s3[0] == 'b'){ /* (flag c b) */
	  obj->id.fpos = FP_C_B ;
	}
      }
      else if (s2[0] == 'g'){ /* goal post flags : (flag g ....) */
	if (n == 4) {
	  if (s3[0] == 'l') { /* (flag g l ..) */
	    if (s4[0] == 't'){ /* (flag g l t) */
	      obj->id.fpos = FP_G_L_T ;
	    }
	    else if (s4[0] == 'b') { /* (flag g l b) */ 
	      obj->id.fpos = FP_G_L_B ;
	    }
	  }
	  else if (s3[0] == 'r') { /* (flag g r ..) */
	    if (s4[0] == 't'){ /* (flag g r t) */
	      obj->id.fpos = FP_G_R_T ;
	    }
	    else if (s4[0] == 'b') { /* (flag g r b) */ 
	      obj->id.fpos = FP_G_R_B ;
	    }
	  }
	}
      }
      else if (s2[0] == 'p'){ /* (flag p .. ..) */
	if (n == 4){
	  if (s3[0] == 'l') { /* (flag p l ..) */
	    if (s4[0] == 't'){ /* (flag p l t) */
	      obj->id.fpos = FP_P_L_T ;
	    }
	    else if (s4[0] == 'b') { /* (flag p l b) */ 
	      obj->id.fpos = FP_P_L_B ;
	    }
	    else if (s4[0] == 'c') { /* (flag p l c) */
	      obj->id.fpos = FP_P_L_C ;
	    }
	  }
	  else if (s3[0] == 'r') { /* (flag p r ..) */
	    if (s4[0] ==  't'){ /* (flag p r t) */
	      obj->id.fpos = FP_P_R_T ;
	    }
	    else if (s4[0] == 'b') { /* (flag p r b) */ 
	      obj->id.fpos = FP_P_R_B ;
	    }
	    else if (s4[0] == 'c'){ /* (flag p r c)*/
	      obj->id.fpos = FP_P_R_C ;
	    }
	  }
	}
      }
      else if (s2[0] == 'l'){ /* (flag l ....) */
	if ((n == 3) || (n == 4)){
	  if (s3[0] == 't') { /* (flag l t ..) */
	    if (n == 3) { /* (flag l t) */
	      obj->id.fpos = FP_L_T ;
	    }
	    else if (s4[0] == '1') { /* (flag l t 10) */
	      obj->id.fpos = FP_L_T_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag l t 20) */
	      obj->id.fpos = FP_L_T_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag l t 30) */
	      obj->id.fpos = FP_L_T_30 ;
	    }
	  }
	  else if (s3[0] == 'b') { /* (flag l b ..) */
	    if (n == 3) { /* (flag l b) */
	      obj->id.fpos = FP_L_B ;
	    }
	    else if (s4[0] == '1') { /* (flag l b 10) */
	      obj->id.fpos = FP_L_B_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag l b 20) */
	      obj->id.fpos = FP_L_B_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag l b 30) */
	      obj->id.fpos = FP_L_B_30 ;
	    }
	  }
	  else if (s3[0] == '0') { /* (flag l 0) */
	    obj->id.fpos = FP_L_0 ;
	  }
	}
      }
      else if (s2[0] == 'r'){ /* (flag r ....) */
	if ((n == 3) || (n == 4)){
	  if (s3[0] == 't') { /* (flag r t ..) */
	    if (n == 3) { /* (flag r t) */
	      obj->id.fpos = FP_R_T ;
	    }
	    else if (s4 [0] == '1') { /* (flag r t 10) */
	      obj->id.fpos = FP_R_T_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag r t 20) */
	      obj->id.fpos = FP_R_T_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag r t 30) */
	      obj->id.fpos = FP_R_T_30 ;
	    }
	  }
	  else if (s3[0] == 'b') { /* (flag r b ..) */
	    if (n == 3) { /* (flag r b) */
	      obj->id.fpos = FP_R_B ;
	    }
	    else if (s4[0] == '1') { /* (flag r b 10) */
	      obj->id.fpos = FP_R_B_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag r b 20) */
	      obj->id.fpos = FP_R_B_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag r b 30) */
	      obj->id.fpos = FP_R_B_30 ;
	    }
	  }
	  else if (s3[0] == '0') { /* (flag r 0) */
	    obj->id.fpos = FP_R_0 ;
	  }
	}
      }
      else if (s2[0] == 't'){ /* (flag t ....) */
	if (n == 3) {
	  if (s3[0] == '0') { /* (flag t 0) */
	    obj->id.fpos = FP_T_0 ;
	  }
	}
	else if (n == 4){
	  if (s3[0] == 'l') { /* (flag t l ..) */
	    if (s4[0] == '1') { /* (flag t l 10) */
	      obj->id.fpos = FP_T_L_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag t l 20) */
	      obj->id.fpos = FP_T_L_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag t l 30) */
	      obj->id.fpos = FP_T_L_30 ;
	    }
	    else if (s4[0] == '4') { /* (flag t l 40) */
	      obj->id.fpos = FP_T_L_40 ;
	    }
	    else if (s4[0] == '5') { /* (flag t l 50) */
	      obj->id.fpos = FP_T_L_50 ;
	    }
	  }
	  else if (s3[0] == 'r') { /* (flag t  r ..) */
	    if (s4[0] == '1') { /* (flag t r 10) */
	      obj->id.fpos = FP_T_R_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag t r 20) */
	      obj->id.fpos = FP_T_R_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag t r 30) */
	      obj->id.fpos = FP_T_R_30 ;
	    }
	    else if (s4[0] == '4') { /* (flag t r 40) */
	      obj->id.fpos = FP_T_R_40 ;
	    }
	    else if (s4[0] == '5') { /* (flag t r 50) */
	      obj->id.fpos = FP_T_R_50 ;
	    }
	  }
	}
      }
      else if (s2[0] == 'b'){ /* (flag b ....) */
	if (n == 3) {
	  if (s3[0] == '0') { /* (flag b 0) */
	    obj->id.fpos = FP_B_0 ;
	  }
	}
	else if (n == 4){
	  if (s3[0] == 'l') { /* (flag b l ..) */
	    if (s4[0] == '1') { /* (flag b l 10) */
	      obj->id.fpos = FP_B_L_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag t l 20) */
	      obj->id.fpos = FP_B_L_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag t l 30) */
	      obj->id.fpos = FP_B_L_30 ;
	    }
	    else if (s4[0] == '4') { /* (flag t l 40) */
	      obj->id.fpos = FP_B_L_40 ;
	    }
	    else if (s4[0] == '5') { /* (flag t l 50) */
	      obj->id.fpos = FP_B_L_50 ;
	    }
	  }
	  else if (s3[0] == 'r') { /* (flag b  r ..) */
	    if (s4[0] == '1') { /* (flag b r 10) */
	      obj->id.fpos = FP_B_R_10 ;
	    }
	    else if (s4[0] == '2') { /* (flag b r 20) */
	      obj->id.fpos = FP_B_R_20 ;
	    }
	    else if (s4[0] == '3') { /* (flag b r 30) */
	      obj->id.fpos = FP_B_R_30 ;
	    }
	    else if (s4[0] == '4') { /* (flag b r 40) */
	      obj->id.fpos = FP_B_R_40 ;
	    }
	    else if (s4[0] == '5') { /* (flag b r 50) */
	      obj->id.fpos = FP_B_R_50 ;
	    }
	  }
	}
      }
    }
  }
  else if (flag_info[0] == 'F'){
    obj->inViewConeP = FALSE ;
    obj->id.fpos = FP_Unknown ;
  }
  else {
    return(FALSE);
  }
  return(TRUE);  
}

Bool parse_goal_info(char *goal_info, SeenObj *obj)
{
  char s1[10],s2[10] ;
  s1[0] = s2[0] = '\0' ;
  
  obj->type = OT_Goal ;
  if (goal_info[0] == 'g'){
    obj->inViewConeP = TRUE ;
    sscanf(goal_info,"%s %s",s1,s2);
    if (s2[0] == 'l')
      obj->id.side = S_LEFT;
    else if (s2[0] == 'r')
      obj->id.side = S_RIGHT;
    else 
      obj->id.side = S_UNKNOWN;
  }
  else if (goal_info[0] == 'G'){
    obj->inViewConeP = FALSE ;
    obj->id.side = S_UNKNOWN;
  }
  else {
    return(FALSE);
  }
  return(TRUE);  
}

Bool parse_line_info(char *line_info, SeenObj *obj)
{
  char s1[10],s2[10] ;
  s1[0] = s2[0] = '\0' ;
  
  obj->type = OT_Line ;
  if (line_info[0] == 'l'){
    obj->inViewConeP = TRUE ;
    sscanf(line_info,"%s %s",s1,s2);
    if (s2[0] == 'l'){
      obj->id.lpos = LP_L;
    }
    else if (s2[0] == 'r'){
      obj->id.lpos = LP_R;
    }
    else if (s2[0] == 't'){
      obj->id.lpos = LP_T;
    }
    else if (s2[0] == 'b'){
      obj->id.lpos = LP_B;
    }
    else {
      obj->id.lpos = LP_Unknown;
    }
  }
  else if (line_info[0] == 'L'){
    obj->inViewConeP = FALSE ;
    obj->id.lpos = LP_Unknown;
  }
  else {
    return(FALSE);
  }
  return(TRUE);  
}

Bool parse_player_info(char *player_info, SeenObj *obj,char *teamname)
{
  char s1[10],s2[TEAMNAMEMAX],s3[10] ;
  s1[0] = s2[0] = s3[0] = '\0' ;
  
  obj->type = OT_Player ; 
  if (player_info[0] == 'p'){
    obj->inViewConeP = TRUE ;
    sscanf(player_info,"%s %s %s",s1,s2,s3);
    obj->id.player.team = WT_Unknown ;   
    if (s2[0] != '\0'){
      if(!strcmp(s2,teamname)){
	obj->id.player.team = WT_Our ;
      }
      else {
	obj->id.player.team = WT_Opponent ;
      }
    }
    if (s3[0] == '\0'){
      obj->id.player.unum = UNUM_ERR ;
    }
    else {
      obj->id.player.unum = atoi(s3);
    }
  }
  else if (player_info[0] == 'P'){
    obj->inViewConeP = FALSE ;
    obj->id.player.team = WT_Unknown;
    obj->id.player.unum = UNUM_ERR ;
  }
  else {
    return(FALSE);
  }
  return(TRUE);  
}


Bool parse_obj_param(char *obj_param, SeenObj *obj)
{
  char s1[10],s2[10],s3[10],s4[10],s5[10];
  s1[0] = s2[0] = s3[0] = s4[0] = s5[0] = '\0';
  
  sscanf(obj_param,"%s %s %s %s %s",s1,s2,s3,s4,s5);
  if (s1[0] == '\0'){
    obj->dist = DIST_ERR ;
  }
  else {
    obj->dist = atof(s1);
  }
  if (s2[0] == '\0'){
    obj->dir = DIR_ERR ;
  }
  else {
    obj->dir = atof(s2);
  }
  if (s3[0] == '\0'){
    obj->dist_chng = CHNG_ERR ;
  }
  else {
    obj->dist_chng = atof(s3);
  }
  if (s4[0] == '\0'){
    obj->dir_chng = CHNG_ERR ;
  }
  else {
    obj->dir_chng = atof(s4);
  }
  if (s5[0] == '\0'){ /* facing direction is only for players */
    obj->fac_dir = DIR_ERR ;
  }
  else {
    obj->fac_dir = atof(s5);
  }
  return(TRUE);
}


void substr(char *str1, char *str2, int from, int to)
{
  strncpy(str1,str2+from,to-from+1);
  str1[to-from+1] = '\0';
  
}

void display_visual_info(FILE *fh , SeeInfo *sinfo)
{
  int i ;
  char *FlagNames[] = FLAGNAMES_STRINGS;

  fprintf(fh,"\n----------------------- Visual Information ---------------------\n");
  fprintf(fh,"Time: %d\nNumber of Objects: %d\n\n",sinfo->time,sinfo->n_obj);
  fprintf(fh,"Obj # \t Dist \t Dir \t Dist_chng\tDir_chng Inview Object Description\n\n");
  
  for(i= 0 ; i < sinfo->n_obj ; i++){
    fprintf(fh,"%2d  %8.3lf %8.3lf",i,sinfo->obj[i].dist,sinfo->obj[i].dir);
    
	    
    fprintf(fh,"    %8.3lf\t%8.3lf ",sinfo->obj[i].dist_chng,sinfo->obj[i].dir_chng);
    fprintf(fh,"  %1d",sinfo->obj[i].inViewConeP);
    
    fprintf(fh,"   ");
    if (sinfo->obj[i].type == OT_Unknown){
      fprintf(fh,"Unknown Object Type");
    }
    else if (sinfo->obj[i].type == OT_Ball){
      fprintf(fh,"Ball");
    }
    else if (sinfo->obj[i].type == OT_Player){
      fprintf(fh,"P[T: %d, U: %d, F_D: %5.2lf]",sinfo->obj[i].id.player.team
	      ,sinfo->obj[i].id.player.unum,sinfo->obj[i].fac_dir);
    }
    else if (sinfo->obj[i].type == OT_Goal){
      fprintf(fh,"Goal: %d",sinfo->obj[i].id.side);
    }
    else if (sinfo->obj[i].type == OT_Flag){
      fprintf(fh,"Flag: %s",FlagNames[sinfo->obj[i].id.fpos]);
    }
    else if (sinfo->obj[i].type == OT_Line){
      fprintf(fh,"Line: %d",sinfo->obj[i].id.lpos);
    }
    else {
      fprintf(fh,"Wrong Info Here ");
    }
    fprintf(fh,"\n");
  }
  fprintf(fh,"-------------------------------------------------------------------\n");
}



