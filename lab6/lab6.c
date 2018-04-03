/***

lab report - lastochkin Vasilii and Oliver Walker

for our lab, we added a ball grabbing feature.
Our robot seeks out a red ball and chases it down. once the ball is within our robots clutches
it will use it's high pressure air powered claw to then squeeze the ball.
once the ball has been succesfully grasped, the robot will spin.
when spinning, the robot will then release the air powered claws and the ball will be thrown in some direction.


***/


// for claw
//  open is 1300
// closed is 660

#include <kipr/botball.h>

typedef int bool;
#define true 1
#define false 0


// motor ports
const int left_wheels = 0;
const int right_wheels = 1;



// function delcarations
void turn_left(void);
void turn_right(void);
void stop(void);
void travel(void);


int main(int argc, char** argv){

    // open claw if it is not open
    enable_servos();
    printf("moving server 0 to 1300");
    set_servo_position(0, 1300);
    msleep(700);
    disable_servos();


    camera_open(LOW_RES);

    //short for channel number
    int c = 0;
    int x;

    int end, i = 0;
    end = (random() % 5) + 5; // 5-10
    printf("researching after %d steps\n", end);

    turn_right();
    while(true){
        if(i > end){
            printf("next search starting\n");
            stop();
            msleep(100);

            // reposition
            //   travel();
            msleep(100);
            stop();

            msleep(500);
            i=0;
            end = (random() % 5) + 5; // 5-10
            turn_right();
        }
        i++;

        // grab new camera image
        camera_update();

        // if there is a red object in the camera image objecy count is not 0
        if(get_object_count(c) != 0){
            printf("found a red ball\n");
            stop();

            // get center x  coord of object 0 on channel c
            x = get_object_center_x(c, 0);


            // if x < 80, ball is to the left
            if(x < 65){
                printf("turning right\n");
                stop();
                turn_right();
            }

            //if x > 80, ball is to the right.
            if(x > 82){
                printf("turning left\n");
                stop();
                turn_left();
            }

            // go forward if ball is betwen x values 75 -85
            if(x > 65 && x < 95){
                printf("target centered\n");
                bool keep_going = true;
                int check = 0;
                while(keep_going){
                    stop();
                    travel();
                    camera_update();
                    printf("obj y is %d\n", get_object_center_y(c, 0));
                    if(get_object_center_y(c, 0) >100){
                        msleep(600);

                        printf("ball captured\n");
                        keep_going = false;

                        //close claw
                        enable_servos();
                        printf("moving server 0 to 660");
                        set_servo_position(0, 660);
                        msleep(700);
                        disable_servos();
                        stop();


                        //spin really fast
                        motor(left_wheels, -100);  // back
                        motor(right_wheels, -100); // forward

                        //open claw
                        enable_servos();
                        printf("moving server 0 to 1300");
                        set_servo_position(0, 1300);
                        msleep(700);
                        disable_servos();

                        //stop moving
                        stop();
                        return 3;
                    }

                    // if there has been no red ball on camera for a long time, stop the program
                    if(get_object_count(c) == 0){
                        printf("ball not on cam, probably captured\n");
                        check++;
                        if(check > 10)
                            return 4;
                    }

                    // this fine tunes the centering on the ball during aproach.
                    // if x < 80, ball is to the left
                    if(x < 80){
                        printf("turning right\n");
                        stop();
                        turn_right();
                    }

                    //if x > 80, ball is to the right.
                    if(x > 80){
                        printf("turning left\n");
                        stop();
                        turn_left();
                    }
                }

            }

        }


        printf("end loop\n");
    } // end while
    camera_close();
    //turn off wheel tread motors
  //  off(left_wheels);
   // off(right_wheels);
 
    return 1;
}

/***
    turns the robot left
 **/
void turn_left(void){

    motor(left_wheels, -25);  // back
    motor(right_wheels, -25); // forward
    return;
}

/***
    turns the robot right.
 **/
void turn_right(void){

    motor(left_wheels, 25);  // forward
    motor(right_wheels, 25); // back
    return;
}

/***
    goes foward or back while called in a loop.

    @var vel: short for velocity. range
    is -1000 to 1000
 **/
void travel(void){

    motor(left_wheels, 100);  // forward
    motor(right_wheels, -100); // forward
    return;
}

void stop(void){
    off(left_wheels);
    off(right_wheels);
    return;
}
