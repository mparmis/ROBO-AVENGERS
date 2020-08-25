#include "stdbool.h"

int prevError = 0;

// Check to see if we have rotated from a collision.
bool hasRotated = false;

// Scaling Constant to offset for the difference in motor speeds
double scalingC = 1;
// Proportionality Constant
double kp = 300;
// Derivative Constant
double kd = 95;
// Threshold
int tapeThreshold = 700;
int junctionThreshold = 225;

// Boolean indicating whether arm is carrying stone
bool carryingStone = false;
bool failedGrab = false;
bool expectingCollision = false;

//Define pwm variables
int clock = 100000; //100 kHz
int period = 500;

// Indicate whether junctionNumber corresponds to a post with a stone.
bool junctionHasStone[9] = {false, false, true, true, false, true, true, true, false};
int junctionCount = 0;

// Example role
int role;