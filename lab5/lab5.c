#include <kipr/botball.h>

typedef int bool;
#define true 1
#define false 0

//enumerated scanner positions
typedef enum scanner {
    left = 20,
    left_mid = 520,
    mid = 1020,
    mid_right = 1520,
    right = 2020,
    true_mid = 1080
} Scanner;

//enumerated wheel positions
typedef enum wheels {
    d_left = -1200,
    d_mid_left = -600,
    d_mid = 0,
    d_mid_right = 600,
    d_right = 1200,
} Direction;

// motor ports
const int wheels = 0;
const int steering = 1;
const int sensor_mount = 2; //need to put this in

//digital
const int left_bmpr = 0;
const int right_bmpr = 1;
const int light_left = 5;
const int light_right = 4;

// wheels position in ticks. 0 is centered.
int wheel_position =0;

int sense_wall(int port);
void turn(int turn_ticks);

// turn sensor mount this will 
int scan_area();
char* get_scanner_names(Scanner scanner);
char* get_direction_names(Direction direction);
Direction choose_best_direction(Scanner s);
Direction choose_worst_direction(Scanner s);
void turn_to(Direction direction);

int main(int argc, char** argv) {
    //start moving @ 100% power in the back wheels.
    //motor(wheels, 100);

    while (true) {

        // move forward a little
        move_relative_position(wheels, 1000, 1000); // move at velocity of 800 ticks per second
        block_motor_done(wheels);

        // find worst position.
        Scanner s = scan_area();
        printf("worst dir is %s\n", get_scanner_names(s));

        //decide on best direction
        Direction best_direction = choose_best_direction(s);
        Direction worst_direction = choose_worst_direction(s);
        printf("best direction is %s\n",get_direction_names(best_direction));
        printf("worst direction is %s\n",get_direction_names(worst_direction));

        // turn to the best direction.

        if(best_direction != d_mid){ // if not turning mid...

            // turn wheels in negative best direction
            turn_to(worst_direction);

            //move back
            move_relative_position(wheels, 1000, -500); // move back
            block_motor_done(wheels);

            // turn wheels in best direction
            turn_to(best_direction);

            // move forward
            move_relative_position(wheels, 1000, 300); // move forward
            block_motor_done(wheels);

            //straighten wheels
            turn_to(d_mid);


        } // end if

    }// end while

    //turn off wheels motor
    off(wheels);

    return 0;
}

Direction choose_worst_direction(Scanner s){
    switch (s) 
    {
        case left: return d_right;
        case left_mid: return d_mid_right;
        case mid: return d_mid;
        case mid_right: return d_mid_left;
        case right: return d_left;
        case true_mid: return d_mid;
    }

    //error. should handle this better later.
    return d_mid;
}

Direction choose_best_direction(Scanner s){
    switch (s) 
    {
        case left: return d_left;
        case left_mid: return d_mid_left;
        case mid: return d_mid;
        case mid_right: return d_mid_right;
        case right: return d_right;
        case true_mid: return d_mid;
    }

    //error. should handle this better later.
    return d_mid;
}

char* get_direction_names(Direction direction){
    switch (direction) 
    {
        case d_left: return "left";
        case d_mid_left: return "mid_left";
        case d_mid: return "mid";
        case d_mid_right: return "mid_right";
        case d_right: return "right";
    }
    return "error";
}

char* get_scanner_names(Scanner scanner) 
{
    switch (scanner) 
    {
        case left: return "left";
        case left_mid: return "left_mid";
        case mid: return "mid";
        case mid_right: return "mid_right";
        case right: return "right";
        case true_mid: return "true_mid";
    }
    return "error";
}

int scan_area(){

    enable_servos();
    Scanner position;
    int worst_direction = left, current_val=0, old_val = 0;
    for(position = left; position <= right; position += 500) {
        set_servo_position(1, position);

        if(position == left)
            msleep(350);
        else
            msleep(250);

        //update the current sensor value
        current_val = sense_wall(3); //pass it the port of the sensor

        printf("%s %d\n", get_scanner_names(position), current_val);
        if (current_val > old_val){
            old_val = current_val;
            worst_direction = position; //when a new high val is found, that is the new worst
            printf("new worst is set\n");
        }
        printf("the worst pos is %s\n", get_scanner_names(worst_direction));

    }
    set_servo_position(1, true_mid);
    msleep(300);

    disable_servos();
    return worst_direction;
}



/*
  sense wall  

  @var port:  the sensor port
*/
int sense_wall(int port){
    return analog(port);
}

void turn_to(Direction direction){
    if(wheel_position != d_mid){
        printf("reseting to wheels position 0\n");
        switch (wheel_position) 
        {
            case d_left:
                turn(d_right);
                break;
            case d_mid_left:
                turn(d_mid_right);
                break;
            case d_mid_right:
                turn(d_mid_left);
                break;
            case d_right:
                turn(d_left);
                break;
        }//end switch
    } // end if

    // wheels are now in mid. do the turn..
    if(direction != d_mid) 
	    turn(direction);   

}

/***
    turns the wheels left or right.
 **/
void turn(int turn_ticks){

    if( (turn_ticks + wheel_position) < d_left-1
       || (turn_ticks + wheel_position) > d_right+1)
    { //out of turning bounds
        printf("cant turn that far.\n");
        printf("turn ticks + wheel position would be %d which is over the bounds of + or - 1200.\n", turn_ticks + wheel_position);
        return;
    }
    wheel_position += turn_ticks;
    printf("wheel position is now %d\n", wheel_position);

    motor(steering, 100); //turn on motor at port steering with 100% power

    move_relative_position(steering, 1000, turn_ticks);
    block_motor_done(steering);

    off(steering);
    return;
}