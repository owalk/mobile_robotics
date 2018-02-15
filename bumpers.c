#include "bumpers.h"

// bumper ports
const int left_bmpr = 0;
const int right_bmpr = 1;

void wander_around(void){
    while(true){
    switch(move("forward", 60)){
    case 0: //left bmpr
      safe_move_back(1000);
      move_turn_90("right");
      break;
    case 1: //right_bmpr
      safe_move_back(1000);
      move_turn_90("left");
      sleep(1);
      break;
    case -1:
      //bumper not hit :)
      break;
    }
  }
  return;
}


/***
    loops and checks for bumper collision
    stuck in loop for designated time or
    until collision.

    @var milliseconds: how many milliseconds
    in multiples of 100

    @returns which bumper has been hit
    -1 means no collision
 **/
int fetch_bumpers_loop(int seconds){
    bool loop = true;
    time_t start,now;

    start = time(NULL);

    while(loop){

        if (digital(left_bmpr)){
            move_relative_position(wheels,-800,5000);
            return left_bmpr;
        }
        if (digital(right_bmpr)){
            move_relative_position(wheels,-800,5000);
            return right_bmpr;
        }
        now = time(NULL);

        int val = difftime(now, start);
        if( val >= seconds){ //probably running into issue  here on second move
            printf("comparison of time is val=%d", val);
            return -1;
        }
    }
    return -1;
}