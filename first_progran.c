#include <kipr/botball.h>
#include <time.h>
typedef int bool;
#define true 1
#define false 0

//motor ports
const int wheels = 0;
const int steering = 1;

//bumper ports
const int left_bmpr = 0;
const int right_bmpr = 1;

//turning wheels for 1000 milliseconds results in the hardest turn possible
int full_turn = 1000;
//turning wheels for 600 milliseconds results in about half a full turn.
int half_turn = 600;

int main(int argc, char** argv) {

    switch(move("forward", 5)){
    case 0: //left_bmpr
        move("back", 3); //does not work for some reason???
        move_turn_90("right");
        move_turn_90("left");
        break;
    case 1: //right_bmpr
        move("back", 3);
        move_turn_90("left");
        move_turn_90("right");
        break;
    case -1:
        //bumper not hit :)
        break;
    }

 return 0;
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
            return left_bmpr;
        }
        if (digital(right_bmpr)){
            return right_bmpr;
        }
        now = time(NULL);

        if(difftime(now, start) >= seconds) //probably running into issue  here on second move
            return -1;
    }
    return -1;
}

/***
    move forward or back for dur seconds.

    @var dir: direction. either forward
    or back

    @var dur: duration. seconds to move for.
 **/
 int move(char* dir, int dur){
     int val;
     enable_servos(); // turn on motors
     if("forward")
         move_relative_position(wheels, 1000, 1000); // move at velocity 800 out of -1000 to 1000
     else //"back"
         mav(wheels, -800);
     val = fetch_bumpers_loop(dur); // loop and check bumper
     ao(); // all motors off
     return val; // returns bumper port or -1 if nothing hit
 }

/***
    robot moves while turning 90 degrees.
    Wheels straighten after.

    @var dir: direction should be "left"
    or "right"
 **/
int move_turn_90(char* dir){
    if(dir == "left")
        turn("left", full_turn);
    else
        turn("right", full_turn);

    mav(wheels, 500);
    //msleep(3900); // changed this to fetch_bumper_loop
    fetch_bumpers_loop(4);
    ao(); //shuts off all motors
    if(dir == "left")
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
    enable_servos(); //turn on motor
    if(dir == "left")
        mav(steering, 800);
    else//dir == right
        mav(steering, -800);
    msleep(turn_duration);
    ao(); //turn off motor
    return;
}
