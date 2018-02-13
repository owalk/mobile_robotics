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
const int left_outer_sensor = 3;
const int right_outer_Sensor = 4;
int full_turn = 800;
int half_turn = 300;




//sensor functions
int fetch_bumpers_loop(int seconds);
int on_black(int port);
int on_white(int port);

//main method functions
void wander_around(void);
void follow_line(void);
void centering(char* direction);
void hard_turn(char* direction);

int main(int argc, char** argv) {

  //wander_around();
  follow_line();
  return 0;
}

/***
    Has the robot follow a black line around a obsticle course
 **/
void follow_line(void){
    motor(wheels, 100); // turn on wheels at 100 percent.
    mav(wheels, 450); // move wheels at 500 ticks per second

    while(true){

        // robot hits a hard turn in these 2 checks
        if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
            printf("%s\n", );("left turning condition hit\n");
            hard_turn("left");
        }

        if(on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
             printf("right turning condition hit\n");
             hard_turn("right");
        }

        // robot falling off line in these two checks
        if(on_white(left_sensor) && on_white(middle_sensor) && on_black(right_sensor)){
            turn("right", half_turn);

            // If robot needs to hard turn instead of readjust, these checks get hit
            if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
                turn("left", half_turn); // readjust early
                hard_turn("left");
            }
            if(on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
                turn("left", half_turn); // readjust early
                hard_turn("right");
            }
            centering("left"); // call the centering function if no hard turn happens.
        }

        if(on_black(left_sensor) && on_white(middle_sensor) && on_white(right_sensor)){
            turn("left", half_turn);
            if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
                turn("right", half_turn); // re-adjust early
                hard_turn("left");
            }
            if(on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
                turn("right", half_turn); // re-adjust early
                hard_turn("right");
            }
            centering("right");
        }
    }

    ao(); //all motors off before termination
    return;
}

/***
    goes forward until a front bumper is hit,
    then turns and repeats...
 **/
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
    centers the robot so that the front 3 sensors
    read white/black/white. also contains logic for special cases

    @var direction: a character array containing eithr left or right
 **/
void centering(char* direction){

  bool loop = 1;
  while(loop){
      if(on_white(left_sensor) && on_black(middle_sensor) && on_white(right_sensor)){
          turn(direction, half_turn);
          loop = false;
      }


      // this is to check for a hard turn while centering
      if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
          printf("turning left from centering loop\n");
          turn(direction, half_turn);
          hard_turn("left");
      }

      if (on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
          printf("turning right from centering loop\n");
          turn(direction, half_turn);
          hard_turn("right");
      }

  }

  return;
}

/***
    use this function for 90 degree turns in the follow line function.
    robot wil look for the edge of the line and then look for
    white/black/white before re-adjusting the steering to be straight in line
    with the track.

    @var direction: character array with either left or right
 **/
void hard_turn(char* direction){

    safe_move_back(750);
    turn(direction, full_turn);
    motor(wheels, 100); // turn on wheels at 100 percent.
    mav(wheels, 500); // move wheels at 500 ticks per second
    sleep(3); // ignore other checks

    while(true){
        if(strcmp(direction, "left") == 0) {
            if((on_black(left_sensor) && on_white(middle_sensor) && on_white(right_sensor)) ||
               (on_white(left_sensor) && on_white(middle_sensor) && on_black(right_sensor))){
                while(true){
                    if(on_white(left_sensor) && on_black(middle_sensor) && on_white(right_sensor)){
                        off(wheels);
                        turn("right", full_turn);
                        motor(wheels, 100); // turn on wheels at 100 percent.
                        mav(wheels, 450); // move wheels at 500 ticks per second
                        return;
                    }
                }
            }
        }
        if(strcmp(direction, "right") == 0) {
            if((on_black(left_sensor) && on_white(middle_sensor) && on_white(right_sensor)) ||
               (on_white(left_sensor) && on_white(middle_sensor) && on_black(right_sensor))){
                while(true){
                    if(on_white(left_sensor) && on_black(middle_sensor) && on_white(right_sensor)){
                        off(wheels);
                        turn("left", full_turn);
                        motor(wheels, 100); // turn on wheels at 100 percent.
                        mav(wheels, 450); // move wheels at 500 ticks per second
                        return;
                    }
                }
            }
        }
    }
    return;
}

/***
    @var port: sensor's port number?
    @return: returns 1 if on a black line
 **/
int on_black(int port){
  if(analog(port) > 50){ //sensor left is on line
      return 1;
  }
  return 0;
}

/***
    @var port: sensor's port number?
    @return: returns 1 if on a white line
 **/
int on_white(int port){
  if(analog(port) < 50){ //sensor left is on line
      return 1;
  }
  return 0;
}


/***
    loops and checks for bumper collision
    stuck in loop for designated time or
    until collision.

    @var milliseconds: how many milliseconds
    in multiples of 100

    @returns port of which bumper has been hit
    -1 means no collision
 **/
int fetch_bumpers_loop(int seconds){
    bool loop = true;
    time_t start,now;

    start = time(NULL);

    while(loop){

        if (digital(left_bmpr)){ // left bumper was hit
            move_relative_position(wheels,-800,5000);
            return left_bmpr; // return the left bumper port number
        }
        if (digital(right_bmpr)){ // right bumper has been hit
            move_relative_position(wheels,-800,5000);
            return right_bmpr; // return right bumper port number
        }
        now = time(NULL); // update now to current time

        int val = difftime(now, start); // store the difference in time from now and start
        if( val >= seconds){ // has enough time passed?
            // printf("comparison of time is val=%d", val);
            return -1; // no bumper collision, finished looping.
        }
    }
    return -1; // this is never hit.
}
