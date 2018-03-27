#include <kipr/botball.h>

typedef int bool;
#define true 1
#define false 0

// motor ports
const int wheels = 0;
const int steering = 1;
const int light_left = 5;
const int light_right = 4;

int full_turn = 800;
int half_turn = 300;

int move(char* dir, int duration);
void turn(char* dir, int turn_ticks);
int light_sensor_normalizing(int port);


int main(int argc, char** argv) {

    turn("right", 1400);
    turn("left", 1400);
    while(true){

        if() //if turn?
            turn("right", 1000);
        //if
        //turn("left", 1000);
        return 0;
    }

  return 0;
}

int light_sensor_normalizing(int port){
    int data = analog(light_left);

    if(data < 30)
        return 0;
    if(data < 60)
        return 20;
    if(data < 90)
        return 50
    return 100;
}

/***
    turns the wheels left or right.

    @var dir: direction can be "left" or
    "right"

    @var turn_duration: describes how far should
    the wheels turn. IE. full_turn is which is defined
    as 1000
 **/
 void turn(char* dir, int turn_ticks){

     clear_motor_position_counter(steering);
     bool loop = true;
     int steering_position = 0;

     motor(steering, 100); //turn on motor at port steering with 100% power

     // start the steering
     if(strcmp(dir, "left") == 0){
         mav(steering, 1000);
         printf("turning left");
     } else if(strcmp(dir, "right") == 0){
         printf("turned right");
         mav(steering, -1000);
     }

     //loop the steering until finished
     while(loop){
         steering_position = get_motor_position_counter(steering);

         if(strcmp(dir, "left") == 0){
             if(steering_position >= turn_ticks)
                 loop = false;
         }
         if(strcmp(dir, "right") == 0){
             if(steering_position <= -turn_ticks)
                 loop = false;
         }
     }

     off(steering);
     return;
 }

 int move(char* dir, int duration){
     float val;
     motor(wheels, 100); // turn on wheels at 100 percent.
     if(strcmp(dir, "forward") == 0) {
         printf("moving forward for %d seconds\n", duration);
         mav(wheels, 1000);
     }
     else {
         printf("moving back for %d seconds\n", duration);
         mav(wheels, -1000);
     }
     msleep(duration);

     off(wheels); // wheels motor off
     return val; // returns bumper port or -1 if nothing hit
 }


