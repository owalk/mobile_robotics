#include <kipr/botball.h>
typedef int bool;
#define true 1
#define false 0

int wheels = 0;
int steering = 1;
int full_turn = 1000;
int half_turn = 600;

int main() {
bool loop = true;
while(loop){
    enable_servos();
    mav(wheels, 800);
    switch (digital(0) || digital(1)){
    case 1:
        loop = false;
        ao();
        move("back", 3);
        break;

    }
 }
// move_turn_90("left");
// move_turn_90("right");

}

void fetch_bumpers(int time){
    bool loop = true;
    while(loop){
        if (digital(0)){
            loop = false;
        }
        if (digital(1)){
            loop = false;
        }
    }
}

/***
    move forward or back for dur seconds.

    @var dir: direction. either forward
    or back

    @var dur: duration. seconds to move for.
 **/
 void move(char* dir, int dur){
     enable_servos();
     if("forward")
         mav(wheels, 800);
     else //"back"
         mav(wheels, -800);
     sleep(dur);
     ao();
     return;
 }

/***
    robot moves while turning 90 degrees.
    Wheels straighten after.

    @var dir: direction should be "left"
    or "right"
 **/
void move_turn_90(char* dir){
    if(dir == "left")
        turn("left", full_turn);
    else
        turn("right", full_turn);

    mav(wheels, 500);
    msleep(3900);
    ao(); //shuts off all motors
    if(dir == "left")
        turn("right", full_turn);
    else
        turn("left", full_turn);

    return;
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
        mav(steering, -800)
    msleep(turn_duration);
    ao(); //turn off motor
    return;
}
