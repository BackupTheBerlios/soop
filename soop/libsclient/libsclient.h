/*
 *	Header:
 *	File: libsclient.h
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *        Modified: March, 1998  Yaser Al-Onzaian  (version 4.00)
 *
 *	EndHeader:
 */

#ifndef __libsclient_h
#define __libsclient_h

#define Bool        int
#define SEENOBJMAX  64
#define BUFSIZE1    2048
#define TEAMNAMEMAX 32 

#define TRUE        1
#define FALSE       0
#define DIST_ERR    -360
#define DIR_ERR     -360
#define CHNG_ERR    1024
#define UNUM_ERR    -1
#define POS_ERR     -100

#define PITCH_LENGTH    105.0
#define PITCH_WIDTH 68.0
#define FIELD_LENGTH    PITCH_LENGTH
#define FIELD_WIDTH     PITCH_WIDTH

#define PENALTY_AREA_LENGTH 16.5
#define PENALTY_AREA_WIDTH  40.32
#define GOAL_WIDTH          14.64

#define Stamina_Error -1000
#define Speed_Error -1000
#define Kick_Error -1
#define Dash_Error -1
#define Turn_Error -1
#define Say_Error  -1


#ifdef MEMSET
#define bzero(a, b) memset(a, 0, b)
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct socket{
		int socketfd  ;
		struct sockaddr_in serv_addr ;
} Socket ;

typedef enum _Side {
		S_UNKNOWN,
        S_LEFT,
        S_RIGHT,
} Side ;

#define PLAYMODE_STRINGS {\
	"before_kick_off",\
	"play_on",\
	"time_over",\
	"kick_off_l",\
	"kick_off_r",\
	"kick_in_l",\
	"kick_in_r",\
	"free_kick_l",\
	"free_kick_r",\
	"corner_kick_l",\
	"corner_kick_r",\
	"goal_kick_l",\
	"goal_kick_r",\
	"goal_l",\
	"goal_r",\
	"offside_l",\
	"offside_r",\
	"foul_l",\
	"foul_r",\
	"half_time",\
	"time_up",\
	"time_extended",\
	NULL\
}

typedef enum _PlayMode {
        PM_BeforeKickOff,
        PM_PlayOn,
        PM_TimeOver,
        PM_KickOff_L,
        PM_KickOff_R,
        PM_KickIn_L,
        PM_KickIn_R,
        PM_FreeKick_L,
        PM_FreeKick_R,
        PM_CornerKick_L,
        PM_CornerKick_R,
        PM_GoalKick_L,
        PM_GoalKick_R,
        PM_Goal_L,
        PM_Goal_R,
        PM_Offside_L,
        PM_Offside_R,
        PM_Foul_L,
        PM_Foul_R,
        PM_HalfTime,
        PM_TimeUp,
        PM_Extend,
        PM_Error,
} PlayMode ;

typedef struct _InitInfo {
        Side side ;
        int unum ;
        char playmodestr[32] ;
        PlayMode playmode ;
} InitInfo ;


typedef enum _ViewWidth {
        VW_Narrow,
        VW_Normal,
        VW_Wide,
	VW_Error,
} ViewWidth ;

typedef enum _ViewQuality {
        VQ_high,
        VQ_low,
	VQ_Error,
} ViewQuality ;

typedef enum _ObjectType {
		OT_Unknown,
        OT_Ball,
        OT_Player,
        OT_Goal,
        OT_Flag,
        OT_Line,
        OT_estBall
} ObjectType ;

#define NUM_FLAGS 54
#define NUM_GOALS  2
#define NUM_LINES  5

typedef enum _FlagPos {
		FP_Unknown,
        FP_L_T,
        FP_L_B,
        FP_R_T,
        FP_R_B,
        FP_C_T,
        FP_C_B,
        FP_P_L_T,
        FP_P_L_C,
        FP_P_L_B,
        FP_P_R_T,
        FP_P_R_C,
        FP_P_R_B,
     /* new Landmarks (flags) For Soccerserver Version 4.0 and higher, 
		   added by YMA  2/25/98 */
		
	FP_C,   /* Center Mark */
		/* Goal Posts */
	FP_G_L_T,
	FP_G_L_B,
	FP_G_R_T,
	FP_G_R_B,
		/* Flags outside of the pitch */
	FP_T_L_10,
	FP_T_L_20,
	FP_T_L_30,
	FP_T_L_40,
	FP_T_L_50,
	FP_T_R_10,
	FP_T_R_20,
	FP_T_R_30,
	FP_T_R_40,
	FP_T_R_50,
	FP_B_L_10,
	FP_B_L_20,
	FP_B_L_30,
	FP_B_L_40,
	FP_B_L_50,
	FP_B_R_10,
	FP_B_R_20,
	FP_B_R_30,
	FP_B_R_40,
	FP_B_R_50,
	FP_L_T_10,
	FP_L_T_20,
	FP_L_T_30,
	FP_L_B_10,
	FP_L_B_20,
	FP_L_B_30,
	FP_R_T_10,
	FP_R_T_20,
	FP_R_T_30,
	FP_R_B_10,
	FP_R_B_20,
	FP_R_B_30,
		/* Flags located 5 meters outside from the edge of the pitch. */
	FP_T_0,
	FP_B_0,
	FP_L_0,
	FP_R_0,
		/* end of flags added by YMA 2/25/98 */
} FlagPos ;

/* New Global Variable Defined by : YMA 3/5/98 */

#define FLAGNAMES_STRINGS {\
	"FP_Unknown",\
        "FP_L_T",\
        "FP_L_B",\
        "FP_R_T",\
        "FP_R_B",\
        "FP_C_T",\
        "FP_C_B",\
        "FP_P_L_T",\
        "FP_P_L_C",\
        "FP_P_L_B",\
        "FP_P_R_T",\
        "FP_P_R_C",\
        "FP_P_R_B",\
     /* new Landmarks (flags) For Soccerserver Version 4.0 */\
	"FP_C",   /* Center Mark */\
     /* Goal Posts */\
	"FP_G_L_T",\
	"FP_G_L_B",\
	"FP_G_R_T",\
	"FP_G_R_B",\
     /* Flags outside of the pitch */\
	"FP_T_L_10",\
	"FP_T_L_20",\
	"FP_T_L_30",\
	"FP_T_L_40",\
	"FP_T_L_50",\
	"FP_T_R_10",\
	"FP_T_R_20",\
	"FP_T_R_30",\
	"FP_T_R_40",\
	"FP_T_R_50",\
	"FP_B_L_10",\
	"FP_B_L_20",\
	"FP_B_L_30",\
	"FP_B_L_40",\
	"FP_B_L_50",\
	"FP_B_R_10",\
	"FP_B_R_20",\
	"FP_B_R_30",\
	"FP_B_R_40",\
	"FP_B_R_50",\
	"FP_L_T_10",\
	"FP_L_T_20",\
	"FP_L_T_30",\
	"FP_L_B_10",\
	"FP_L_B_20",\
	"FP_L_B_30",\
	"FP_R_T_10",\
	"FP_R_T_20",\
	"FP_R_T_30",\
	"FP_R_B_10",\
	"FP_R_B_20",\
	"FP_R_B_30",\
      /* Flags located 5 meters outside from the edge of the pitch. */\
	"FP_T_0",\
	"FP_B_0",\
	"FP_L_0",\
	"FP_R_0",\
	NULL\
}

typedef enum _LinePos {
		LP_Unknown,
        LP_L,
        LP_T,
        LP_B,
        LP_R, 
} LinePos ;

typedef enum _WhichTeam {
        WT_Opponent = -1,
        WT_Unknown = 0,
        WT_Our = 1,
} WhichTeam ;

typedef struct _Ppos {
		WhichTeam team ;
		int unum ;
} PlayerInfo ;


typedef struct _XYPos {
        double x ;
        double y ;
} Pos ;

typedef struct _PosState {
        Pos pos ;
        double dir ;
} PosState ;


typedef struct _SeenObj {
        ObjectType type ;
        Bool inViewConeP ; /* True if the object is in the view cone */
        union {
		PlayerInfo player ;
                Side side ;
                FlagPos fpos ;
                LinePos lpos ;
        } id ;
        double dist ;
        double dir ;
        double dist_chng ;
        double dir_chng ;
        double fac_dir ; /* this is only for players, YMA 3/6/98 */
        Pos abs_pos;
        double probability;
} SeenObj ;

typedef struct _SeeInfo {
        int time ;
        int n_obj ;
        SeenObj obj[SEENOBJMAX] ;
} SeeInfo ;

typedef enum _SenderType {
        ST_Self,
        ST_Referee,
        ST_Player,
} SenderType ;

typedef struct _HearInfo {
        int time ;
        SenderType sender ;
        double direction ;
        char message[BUFSIZE1] ;
} HearInfo ;

/* Begin added type definitions:  by YMA 3/4/98 ------------------------ */

typedef double Stamina ;
typedef double Speed ;

typedef struct _TeamInfo {
  char tname[TEAMNAMEMAX] ;
  int no_players ;
  int score ;
}TeamInfo;
  
typedef struct _BStateInfo {
        int time ;
        ViewQuality quality ;
        ViewWidth   width ;
        Stamina short_stamina ;
        Stamina long_stamina ;
        Speed   speed ;
        int   kick ;
        int   dash ;
        int   turn ;
        int   say  ;
} BStateInfo ;

typedef struct _FStateInfo {
        TeamInfo team_l ;
        TeamInfo team_r ;
        PlayMode pmode ;
} FStateInfo ;

typedef struct _MStateInfo {
        double pitch_length;
        double pitch_width;
        double goal_width;
        double penalty_length;
        double penalty_width;
} MStateInfo ;

/* end of added type definitions: by YMA 3/4/98 ------------------------ */

typedef enum _SensorInfoType {
        SIT_See,
        SIT_Hear,
	SIT_BState, /* added by YMA : 3/4/98 - Body State  */
	SIT_FState, /* added by YMA : 3/4/98 - Field State */
	SIT_MState, /* added by YMA : 3/4/98 - Match State */
} SensorInfoType ;

typedef struct _SensorInfo {
        SensorInfoType type ;
        union {
                SeeInfo see ;
                HearInfo hear ;
	        BStateInfo bstate; /* Added by YMA 3/4/98 : Body State Info */
	        FStateInfo fstate; /* Added by YMA 3/4/98 : Field State Info */
	        MStateInfo mstate; /* Added by YMA 3/4/98 : Match State Info */ 
        } info ;
} SensorInfo ;



	/*** functions in netif.c ***/
extern Socket   init_connection(char *host, int port) ;
extern void     close_connection(Socket sock) ;
extern Bool     send_message(Socket sock, char *buf) ;
extern Bool     receive_message(Socket *sock, char *buf, int size) ;

	/*** functions in basic_com.c ***/
extern InitInfo send_com_init(Socket *sock, char *teamname,int version, int role) ;
extern InitInfo send_com_reconnect(Socket *sock, char *teamname, int unum) ;
extern Bool send_com_move(Socket *sock, double x, double y) ;
extern Bool send_com_turn(Socket *sock, double moment) ;
extern Bool send_com_dash(Socket *sock, double power) ;
extern Bool send_com_kick(Socket *sock, double power, double dir) ;
extern Bool send_com_turn_neck(Socket *sock, double angle) ;
extern Bool send_com_say(Socket *sock, char *message) ;
extern Bool send_com_change_view(Socket *sock, ViewWidth width, ViewQuality quality)
 ;
 extern Bool send_com_catch(Socket *sock, double dir );

 extern Bool send_com_body_state(Socket *sock);
 extern Bool send_com_match_state(Socket *sock);
 extern Bool send_com_field_state(Socket *sock);
 
	/*** functions in sensor_info.c ***/
extern Bool receive_info(Socket *socket, char *buffer, int size) ;
extern Bool scan_info(char *buffer, SensorInfo *sinfo, char *teamname) ;
 /*extern Bool scan_visual_info(char *buffer, SeeInfo *sinfo, char *teamname) ;*/
extern Bool scan_auditorial_info(char *buffer, HearInfo *hinfo) ;

	/*** functions in position.c ***/
/*** extern Bool estimate_current_pos(SeeInfo *sinf, Side side, PosState *pstate) ;***/

	/*** functions in scan_visual.c  ***/
 /*extern Bool scan_info(char *buffer, SensorInfo *sinfo, char *teamname) ;*/
extern Bool scan_visual_info(char *buffer, SeeInfo *sinfo, char *teamname) ;

	/*** functions in scan_state.c  ***/
extern Bool scan_body_state(char * buffer, BStateInfo *binfo);
extern Bool scan_match_state(char * buffer, MStateInfo *minfo);
extern Bool scan_field_state(char * buffer, FStateInfo *finfo);
 

#endif /* __libsclient_h */
