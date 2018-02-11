
#include <time.h>
#include <unistd.h>
#include "movements.h"

typedef int bool;
#define true 1
#define false 0

// globals: see globals.h
const int wheels = 0;
const int steering = 1;
const int left_bmpr = 0;
const int right_bmpr = 1;
const int left_sensor = 0;
const int middle_sensor =1;
const int right_sensor = 2;
int full_turn = 1000;
int half_turn = 300;




//sensor functions
int fetch_bumpers_loop(int seconds);
int on_black(int port);
int on_white(int port);

//main method functions
void wander_around(void);
void follow_line(void);

int main(int argc, char** argv) {	

  motor(wheels, 100); // turn on wheels at 100 percent.
  mav(wheels, 500); // move wheels at 500 ticks per second

  while(true){
      
    if(on_white(left_sensor) && on_white(middle_sensor) && on_black(right_sensor)){
      turn("right", half_turn);
      centering("left");
    }	  

    if(on_black(left_sensor) && on_white(middle_sensor) && on_white(right_sensor)){
      turn("left", half_turn);
      centering("right");
      
    }

    if(on_black(left_sensor) && on_black(middle_sensor) && on_white(right_sensor))
      //left turn
      return 0;

    if(on_white(left_sensor) && on_black(middle_sensor) && on_black(right_sensor))
      //right turn
      return 0;
  }

  ao(); //all off
  move_relative_position(wheels, 1000, 2);

  //wander_around();
  //follow_line();
  return 0;
}

void centering(char* direction){
  bool loop = 1;
  while(loop){
	if(on_white(left_sensor) && on_black(middle_sensor) && on_white(right_sensor)){
	  turn(direction, half_turn);
	  loop = false;
	}
      }
  return;
}

int on_black(int port){
  if(analog(port) > 50){ //sensor left is on line    
    return 1;   
  }
  return 0;  
}
    
int on_white(int port){
  if(analog(port) < 50){ //sensor left is on line    
    return 1;   
  }
  return 0;
}
    
void follow_line(void){
  
  return;
}

void wander_around(void){
  while(true){
    switch(move("forward", 60)){
    case 0: //left_bmpr
      safe_move_back();
      move_turn_90("right");
      break;
    case 1: //right_bmpr
      safe_move_back();
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
