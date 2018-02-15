#include "IR_sensors.h"

// ir sensor ports
const int left_sensor = 0;
const int middle_sensor =1;
const int right_sensor = 2;
const int left_outer_sensor = 3;
const int right_outer_Sensor = 4;

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
	motor(wheels, 100); // turn on wheels at 100 percent.
  mav(wheels, 450); // move wheels at 500 ticks per second

  while(true){

      // robot hits a hard turn in these checks
      if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
          printf("left turning condition hit\n");
          printf("left_out: %d, lef: %d, middle %d, right: %d, right_out: %d\n",
                 on_black(left_outer_sensor),
                 on_black(left_sensor),
                 on_black(middle_sensor),
                 on_black(right_sensor),
                 on_black(right_outer_Sensor));
          hard_turn("left");
      }

      if(on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
             printf("right turning condition hit\n");
             printf("left_out: %d, lef: %d, middle %d, right: %d, right_out: %d\n",
                    on_black(left_outer_sensor),
                    on_black(left_sensor),
                    on_black(middle_sensor),
                    on_black(right_sensor),
                    on_black(right_outer_Sensor));
             hard_turn("right");
      }

      // robot falling off line in these two checks
      if(on_white(left_sensor) && on_white(middle_sensor) && on_black(right_sensor)){
          turn("right", half_turn);
          if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
              turn("left", half_turn);
              hard_turn("left");
          }
          if(on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
              turn("left", half_turn);
              hard_turn("right");
          }
          centering("left");
      }

      if(on_black(left_sensor) && on_white(middle_sensor) && on_white(right_sensor)){
          turn("left", half_turn);
          if(on_black(left_outer_sensor) && on_white(right_outer_Sensor)){
              turn("right", half_turn);
              hard_turn("left");
          }
          if(on_white(left_outer_sensor) && on_black(right_outer_Sensor)){
              turn("right", half_turn);
              hard_turn("right");
          }
          centering("right");
      }
  }

  ao(); //all off
  move_relative_position(wheels, 1000, 2);
  return;
}

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