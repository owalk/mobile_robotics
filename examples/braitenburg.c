#include <kipr/botball.h>

//light seeking
int main(){

    //move forward
    motor(0, 100);
    motor(2, 100);

    while(true){

        int l = analog(3);
        int r = analog(4);

        if(l > r) {
            mav(0,500);
            mav(2,1000);
        }

        if(l > r) {
            mav(0,1000);
            mav(2,500);
        }

    }

    return 0;
}

//back up on bump
int main(){

    //move forward
    motor(0, 100);
    motor(2, 100);

    while(true){

        int l = digital(9);
        int r = digital(10);

        if(l && r) {

            //back up
            mrp(0,-1000, 4000);
            mrp(2,-1000, 4000);
            block_motor_done(2);

            //turn
            mav(0,500);
            mav(2,1000);

            msleep(600);

            off(0);
            off(2);
            return;

        } else {
            //go straight
            mav(0,1000);
            mav(2,1000);
        }

    }

    return 0;
}
