#include "movements.h"

// motor ports
const int wheels = 0;
const int steering = 1;


/***
    move back without checking bumpers.
    this lets us avoid a bug where you cant move
    if bumper is already down.
    **/
void safe_move_back(int duration){
    off(wheels);
    motor(wheels, 100);
    mav(0, -800);
    msleep(duration);
    off(wheels);
    return;
}


int fetch_bumpers_loop(int seconds); // declaring bumper function for move function.

// this should be moved to bumpers.c and renamed as bumper specific move
/***
    move forward or back for dur seconds.

    @var dir: direction. either forward
    or back

    @var dur: duration. seconds to move for.
 **/
 int move(char* dir, int dur){
     float val;
     motor(wheels, 100); // turn on wheels at 100 percent.
     if(strcmp(dir, "forward") == 0) {
         printf("moving forward for %d seconds\n", dur);
         mav(wheels, -800);
     }
     else {
         printf("moving back for %d seconds\n", dur);
         mav(wheels, 800);
     }
     val = fetch_bumpers_loop(dur); // loop and check bumper
     
     off(wheels); // wheels motor off
     return val; // returns bumper port or -1 if nothing hit
 }

/***
    robot moves while turning 90 degrees.
    Wheels straighten after.

    @var dir: direction should be "left"
    or "right"
 **/
int move_turn_90(char* dir){
    if(strcmp(dir, "left") == 0)
        turn("left", full_turn);
    else
        turn("right", full_turn);

    //mav(wheels, 500);
    msleep(3900); // changed this to fetch_bumper_loop
    //fetch_bumpers_loop(4);
    //ao(); //shuts off all motors
    if(strcmp(dir, "left") == 0)
        turn("right", full_turn);
    else
        turn("left", full_turn);

    return -1; //change to return bumper hit
}

/***
    turns the wheels left or right.

    @var dir: direction can be "left" or
    "right"

    @var turn_duration: describes how far should
    the wheels turn. IE. full_turn is which is defined
    as 1000
 **/
 void turn(char* dir, int turn_duration){
/*     float dur = (float)turn_duration/1000;
     bool loop = true;
     time_t start,current;
*/
     motor(steering, 100); //turn on motor at port steering with 100% power
     if(strcmp(dir, "left") == 0){
         mav(steering, 1000); //turn left
         printf("turning left");
     } else if(strcmp(dir, "right") == 0){
         printf("turned right");
         mav(steering, -1000); // turn right
     }
     msleep(turn_duration);
     off(steering);
     return;
// movement
/* // this attempts to check for turn codition while sleeping. not wokring..
    start = time(NULL);

    while(loop){
        current = time(NULL);
        int val = difftime(current, start);
        printf("val is: %.6f", val);

        // check turn condition
        if( val >= dur){
            off(steering); //turn off motor after # seconds passed that we want
        }

        }*/
    return;
 }
