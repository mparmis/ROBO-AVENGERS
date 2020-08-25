#ifndef global_values
#define global_values

// Roles
#define METHANOS 1
#define THANOS -1
#define rolePin PA15

// Define integer directions to pass to rotate function
#define LEFT 1
#define STRAIGHT 0
#define RIGHT -1

// Motor Declarations
#define rightForward PA_6
#define rightBackward PA_7
#define leftBackward PB_1
#define leftForward PB_0

#define BASE_SPEED 350

//IR Sensor Pins
#define rightJunctionSensor PA_5
#define farRightTape PA_1
#define rightTape PA_2
#define leftTape PA_4
#define farLeftTape PA_3
#define leftJunctionSensor PA_0

// Bumper
#define bumperPin PB7

#define armActionOutput PB5
#define armActionComplete PB6
#define armActionSuccess PB7

extern int prevError;

extern bool hasRotated;

// Scaling Constant to offset for the difference in motor speeds
extern double scalingC;
// Proportionality Constant
extern double kp;
// Derivative Constant
extern double kd;
// Threshold
extern int tapeThreshold;

// Boolean indicating whether arm is carrying stone
extern bool carryingStone;
extern bool failedGrab;
extern bool expectingCollision;

//Define pwm variables
extern int clock;
extern int period;

// Indicate whether junctionNumber corresponds to a post with a stone.
extern bool junctionHasStone[9];
extern int junctionCount;

// Example role
extern int role;

#endif