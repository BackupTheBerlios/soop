/*
 *	Header:
 *	File: sensor_info.c
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *	EndHeader:
 */

#include <stdio.h>
#include <stdlib.h>

#include "libsclient.h"

/*** functions in netif.c ***/
extern Bool    send_message(Socket sock, char *buf) ;
extern Bool    receive_message(Socket *sock, char *buf, int size) ;

Bool receive_info(Socket* socket, char* buffer, int size)
{
	int ret = FALSE ;
	char save = '\0';

	while(TRUE) {
		if (receive_message(socket, buffer, size)) {
			ret = TRUE ;
			save = buffer[0] ;
		}
		else
			break ;
	}

	buffer[0] = save ;
	return ret ;
}


Bool scan_info(char *buffer, SensorInfo *sinfo, char *teamname)
{
	Bool scan_visual_info(char *buffer, SeeInfo *sinfo, char *teamname) ;
	Bool scan_auditorial_info(char *buffer, HearInfo *hinfo) ;
	char com[16] ;
	/*	fprintf(stderr,"sensor_info: buffer length %d\n %s\n",strlen(buffer),buffer); *//* YMADEBUG */
	
	sscanf(buffer, "(%s ", com) ;

	if (!strcmp(com, "see")) {
		sinfo->type = SIT_See ;
		return scan_visual_info(buffer, &(sinfo->info.see), teamname) ;
	}
	else if (!strcmp(com, "hear")) {
		sinfo->type = SIT_Hear ;
		return scan_auditorial_info(buffer, &(sinfo->info.hear)) ;
	}
	else if (!strcmp(com, "sense_body")){ /* Added by YMA 3/6/98 */
	  sinfo->type = SIT_BState ;
	  return scan_body_state(buffer,&(sinfo->info.bstate));
	}
	else if (!strcmp(com, "sense_match")){ /* Added by YMA 3/6/98 */
	  sinfo->type = SIT_MState ;
	  return scan_match_state(buffer,&(sinfo->info.mstate));
	}
	else if (!strcmp(com, "sense_field")){ /* Added by YMA 3/6/98 */
	  sinfo->type = SIT_FState ;
	  return scan_field_state(buffer,&(sinfo->info.fstate));
	}
	else{
//	  fprintf(stderr,"Unknown message type:\n%s\n",buffer);
	  return FALSE ;
	}
	
	return TRUE ;
}


static char *next_token(char *buf)
{
	int i = 0 ;

	while (TRUE) {
		if (*buf == '(')
			i++ ;
		else if (*buf == ')') {
			if (i == 0) {
				*buf = '\0' ;
				return NULL ;
			}
			i-- ;
		}
		else if (*buf == ' ' && i == 0)
			break ;
		else if (*buf == '\0')
			return NULL ;
		buf++ ;
	}

	*buf = '\0' ;
	buf++ ;

	return buf ;
}


Bool scan_auditorial_info(char *buffer, HearInfo *hinfo)
{
	static char *next_token(char *buf) ;
	char tmp[64] ;
	int i ;

	sscanf(buffer, "(hear %d %s", &(hinfo->time), tmp) ;
	buffer = next_token(buffer + 1) ;
	buffer = next_token(buffer) ;
	buffer = next_token(buffer) ;

	for(i = strlen(buffer); i >= 0; i--)
		if (buffer[i] == ')') {
			buffer[i] = '\0' ;
			break ;
		}

	strcpy(hinfo->message, buffer) ;

	if (!strcmp(tmp, "referee")) {
		hinfo->sender = ST_Referee ;
		hinfo->direction = 0.0 ;
	}
	else if (!strcmp(tmp, "self")) {
		hinfo->sender = ST_Self ;
		hinfo->direction = 0.0 ;
	}
	else {
		hinfo->sender = ST_Player ;
		hinfo->direction = atof(tmp) ;
	}

	return TRUE ;
}
