/***************************************************************************
 *   Copyright (C) 2004 by Miguel Tavares                                  *
 *   mtavares@student.dei.uc.pt                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <libsclient.h>

/*#define MAXMESG 512 YMA 3/3/98 */
#define MAXMESG 1024


#define MESSAGE(msg) \
  fprintf (stdout, msg);

#ifndef null
#define null 0
#endif

#ifndef bool
#define bool int
#endif

#ifndef false
#define false 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef true
#define true 1
#endif

#ifndef TRUE
#define TRUE 1
#endif
#define TEAM_NAME "SOOP"


/* To keep a idea of how the environment is evolving the agent uses a
 * process similar to the ways the movies works. Movies are composed
 * of frames, and the agent's environment is composed of dynamic
 * objects each of them performing a movie wich we record, but only while
 * the agent can see them, and it only keeps a small record of past frames.
 *
 * The similarity with movies can be used to describe the structures
 * FramePosition and MoviePosition. */
 
typedef struct FramePositionStruct FramePosition;
struct FramePositionStruct {
    unsigned int time;
    double xPos;
    double yPos;
    FramePosition* nextFrame;
};

struct MoviePositionStruct {
  unsigned int numOfFrames;
  FramePosition* frames;
};
typedef struct MoviePositionStruct MoviePosition;

/* The Dynamic part of the environment is composed of the team players,
 * the other team players (the enemy!!!) and the ball. */
struct EnvironmentStruct {
  unsigned int time;
  MoviePosition teamPlayers[10];
  MoviePosition otherTeamPlayers[11];
  MoviePosition ball;
};
typedef struct EnvironmentStruct Environment;

struct PlayerDataStruct {
    Socket serverSocket;
    char* serverName;
    int serverPort;
    InitInfo initInfo;
    SensorInfo sensorInfo;
    Environment environment;
};
typedef struct PlayerDataStruct PlayerData;

/* initializes a movie (of a object) frame */
FramePosition* initFrame (unsigned int time, double x, double y) {
  FramePosition* tmp;
  tmp = (FramePosition*) malloc (sizeof (FramePosition));
  if (tmp == (FramePosition*) null) {
    return ((FramePosition*) null);
  }
  tmp->time = time;
  tmp->xPos = x;
  tmp->yPos = y;
  tmp->nextFrame = (FramePosition*) null;
  return (tmp);
}

/* insert a frame in a movie
 * Important: do not free the frame after inserting it.
 * @param frame is a FramePosition pointer to new frame
 * @param movie is a MoviePosition pointer to the movie where the frame is to be added
 * @return true is the frame is added, false otherwise */
bool insertFrame (FramePosition* frame, MoviePosition* movie) {
  if (frame == (FramePosition*) null)
    return false;
  if (movie == (MoviePosition*) null)
    return false;
  frame->nextFrame = movie->frames;
  movie->frames = frame;
  movie->numOfFrames++;
  return true;
}

/* removes the last frame of a movie
 * @param movie is a pointer to a MoviePosition
 * @return true if a frame was removed, false otherwise */
bool removeLastFrame (MoviePosition* movie) {
  FramePosition* frame;
  
  if (movie == (MoviePosition*) null)
    return false;
  frame = movie->frames;
  if (frame == (FramePosition*) null)
    return false;
  if (frame->nextFrame == (FramePosition*) null) {
    free ((void*) movie->frames);
    movie->frames = (FramePosition*) null;
    movie->numOfFrames = 0;
    return true;
  }
  /* go to the previous to last frame */
  while (frame->nextFrame->nextFrame != (FramePosition*) null) {
    frame = frame->nextFrame;
  }
  free ((void*) frame->nextFrame);
  frame->nextFrame = (FramePosition*) null;
  movie->numOfFrames--;
  return true;
}

/* parse what the agent sees and construct a "image" of the environment
 * bearing in mind that this is a non deterministic, mostly inaccessible
 * and dynamic environment (apart from the flags and goals, wich are
 * static). */
void buildEnvironmentImage (PlayerData* player, SensorInfo* info) {
}

int player (char* serverName, int serverPort) {
    PlayerData data;
    char comBuffer[MAXMESG]; //comunication buffer

    /* begin data Initializing */
    data.serverSocket.socketfd = 0;
    //data.serverSocket.serv_addr = (sockaddr_in) 0;
    data.serverName = (char *) null;
    data.serverPort = 0;
    data.initInfo.side = S_UNKNOWN;
    data.initInfo.unum = 0;
    data.initInfo.playmodestr[0] = (char) null;
    data.initInfo.playmode = PM_Error;

    data.sensorInfo.info.fstate.pmode = PM_Error;
    /* end of data initializing */

    /* let's try to connect to the server */
    data.serverSocket = init_connection (serverName, serverPort);
    if (data.serverSocket.socketfd == -1) {
        perror ("Unable to connect to server.");
        return (-1);
    }

    /* say Hello to server */
    data.initInfo = send_com_init (&data.serverSocket, TEAM_NAME, 4, 1);

    printf("\ninit!: %d, %d, %s, %d\n\n",
           data.initInfo.side, data.initInfo.unum, data.initInfo.playmodestr, data.initInfo.playmode) ;

    /* move to start position */
    send_com_move (&data.serverSocket, (double) -20.0, (double) 0.0);

    if (data.initInfo.playmode == PM_BeforeKickOff) {
        MESSAGE ("Game hasn't started yet!\n");
        if (data.initInfo.side == S_LEFT) {
            MESSAGE ("My team is the one who starts the game.\n");
        } else {
            MESSAGE ("Got to wait for the next team to start...\n");
        }
    } else {
        MESSAGE ("Game has started.\n");
    }

    do {
        receive_info (&data.serverSocket, comBuffer, sizeof(comBuffer));
        scan_info (comBuffer, &data.sensorInfo, TEAM_NAME);
        //printf ("%s\n", comBuffer);
        switch (data.sensorInfo.type) {
        case SIT_See:
            printf ("at [%d] click I see %d objects\n", data.sensorInfo.info.see.time, 
                                                        data.sensorInfo.info.see.n_obj);
            display_visual_info (stdout, &data.sensorInfo.info.see);
            //buildEnvironmentImage (comBuffer, &data);
            break;
        case SIT_Hear:
            MESSAGE ("Hear info\n");
            break;
        case SIT_BState:
            MESSAGE ("Body info\n");
            break;
        case SIT_FState:
            MESSAGE ("Field info\n");
            break;
        case SIT_MState:
            MESSAGE ("Match info\n");
            break;
        default:
            MESSAGE ("Wrong sensor info type\n");
        }
        sleep (1);
    } while (TRUE);

    /* close connection to server */
    close_connection (data.serverSocket);


    return (0);
}


int main(int argc, char *argv[]) {
    Socket sock ;
    char *server ;
    int port ;
    int version,role ; /* YMA 2/25/98 */
    InitInfo info ;
    char tname[16] ;

    int playerResult = 0;

    MESSAGE ("Socer Objective Oriented Player\n");
    playerResult = player ("localhost", 6000);
    if (playerResult == -1) {
        MESSAGE ("Player was unable to connect to server.\n");
        exit (playerResult);
    }
    MESSAGE ("Player out.\n");
    exit (playerResult);
}
