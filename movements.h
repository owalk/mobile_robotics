#include "globals.h"

// movement function declarations
void safe_move_back(int duration);
int move(char* dir, int dur);
int move_turn_90(char* dir);
void turn(char* dir, int turn_duration);
int fetch_bumpers_loop(int seconds);
