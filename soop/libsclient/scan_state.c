#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libsclient.h"

Bool get_next_body_info(char * buffer,char *token);
void display_body_info( FILE *fh,BStateInfo *bsinfo);
Bool parse_body_info(char * body_info, BStateInfo *bsinfo);
Bool scan_field_state(char * buffer, FStateInfo *fsinfo);
void display_field_info( FILE *fh,FStateInfo *fsinfo);
Bool scan_match_state(char * buffer, MStateInfo *msinfo);
void display_match_info( FILE *fh,MStateInfo *msinfo);

Bool scan_body_state(char * buffer, BStateInfo *bsinfo)
{

  char token[BUFSIZE1],temp_buff[BUFSIZE1],*p ;
  int  l = strlen(buffer);

  sscanf(buffer,"(sense_body %d",&(bsinfo->time));
  /* now chop message tag from buffer */
  if ((p =  strchr(buffer+1,'(') ) == NULL ){
    fprintf(stderr,"scan_state.c: (scan_body_info) ill-formated message:\n %s\n",buffer);
    return(FALSE);
  }
  substr(temp_buff,p,0,strlen(p)-2);
  while ((temp_buff[0] != '\0') && (get_next_body_info(temp_buff,token))){
    if (parse_body_info(token,bsinfo)){
      /*      fprintf(stderr,"Body Info: %s, was parsed correctly\n",token);*/
    }
    else {
      /*      fprintf(stderr,"Body Info: %s, was not parsed correctly\n",token);*/
    }      
  }
  return TRUE ;
}

Bool get_next_body_info(char * buffer,char *token)
{
  char temp[BUFSIZE1],*p;
  
  token[0] = '\0' ;
  if ((p = strchr(buffer,')')) == NULL) {
    buffer = NULL ;
    return(FALSE);
  }
  substr(token,buffer,0,p-buffer);
  if ((p = strchr(buffer+1,'(')) == NULL){
    buffer[0] = '\0' ;
  }
  else {
    strcpy(temp,p) ;
    temp[strlen(p)] = '\0' ;
    strcpy(buffer,temp);
  }
  return(TRUE);
}

Bool parse_body_info(char * body_info, BStateInfo *bsinfo)
{
  char s1[10],s2[10],s3[10],temp[BUFSIZE1];


  s1[0] = s2[0] = s3[0] = temp[0] = '\0' ; 
  substr(temp,body_info,1,strlen(body_info)-2);
  sscanf(temp,"%s %s %s",s1,s2,s3);
  if (s1[0] == '\0'){
    return FALSE ;
  }
  if(!strcmp(s1,"view_mode")){
    if (s2[0] == 'h') /* high */
      bsinfo->quality = VQ_high ;
    else if (s2[0] == 'l') /* low */
      bsinfo->quality = VQ_low ;
    else
      bsinfo->quality = VQ_Error ;
    if (s3[0] == 'n' ){ /* normal or narrow */
      if (s3[1] == 'o') /* normal */
	bsinfo->width = VW_Normal ;
      else if (s3[1] == 'a')
	bsinfo->width = VW_Narrow ;
      else 
	bsinfo->width = VW_Error ;
    }
    else if (s3[0] == 'w') /* wide */
      bsinfo->width = VW_Wide ;
    else 
      bsinfo->width = VW_Error ;
  } 
  else if(!strcmp(s1,"stamina")){
    if (s2[0] == '\0')
      bsinfo->short_stamina = Stamina_Error ;
    else 
      bsinfo->short_stamina = atof(s2);
    if (s3[0] == '\0')
      bsinfo->long_stamina = Stamina_Error ;
    else 
      bsinfo->long_stamina = atof(s3);
  }
else if (!strcmp(s1,"speed")){
    if (s2[0] == '\0')
      bsinfo->speed = Speed_Error ;
    else 
      bsinfo->speed = atof(s2) ;
  }
  else if (!strcmp(s1,"kick")){
    if (s2[0] == '\0')
      bsinfo->kick = Kick_Error ;
    else 
      bsinfo->kick = atoi(s2) ;
  }
  else if (!strcmp(s1,"dash")){
    if (s2[0] == '\0')
      bsinfo->dash = Kick_Error ;
    else 
      bsinfo->dash = atoi(s2) ;
  }
  else if (!strcmp(s1,"turn")){
    if (s2[0] == '\0')
      bsinfo->turn = Turn_Error ;
    else 
      bsinfo->turn = atoi(s2) ;
  }
  else if (!strcmp(s1,"say")){
    if (s2[0] == '\0')
      bsinfo->say = Say_Error ;
    else 
      bsinfo->say = atoi(s2) ;
  }
  else 
    return FALSE ;
  return TRUE ;
}

void display_body_info( FILE *fh,BStateInfo *bsinfo)
{
  fprintf(fh,"---------------- Body Information ---------------------\n");
  fprintf(fh,"Time: %d\n",bsinfo->time);
  fprintf(fh,"Speed: %lf\n\n",bsinfo->speed);
  fprintf(fh,"View_Mode:\tQuality:\t%1d\t\tWidth:\t%1d\n",bsinfo->quality,bsinfo->width);
  fprintf(fh,"Stamina:\tShort Term:\t%7.3lf\tLong Term:\t%7.3lf\n\n",
	  bsinfo->short_stamina,bsinfo->long_stamina);
  fprintf(fh,"Number of Commands Sent So Far:\n\n");
  fprintf(fh,"\t\tKick: %d\n",bsinfo->kick);
  fprintf(fh,"\t\tDash: %d\n",bsinfo->dash);
  fprintf(fh,"\t\tTurn: %d\n",bsinfo->turn);
  fprintf(fh,"\t\tSay:  %d\n",bsinfo->say);
  fprintf(fh,"-----------------------------------------------------------\n");
}



Bool scan_match_state(char * buffer, MStateInfo *msinfo)
{
  /* not yet implemented : YMA 3/16/98 */
  return FALSE ;
}

void display_match_info( FILE *fh,MStateInfo *msinfo)
{
  fprintf(fh,"display_match_info: Not yet implemented\n");
}

Bool scan_field_state(char * buffer, FStateInfo *fsinfo)
{
  /* not yet implemented : YMA 3/16/98 */
  return FALSE ;
}

void display_field_info( FILE *fh,FStateInfo *fsinfo)
{
  fprintf(fh,"display_field_info: Not yet implemented\n");
}
