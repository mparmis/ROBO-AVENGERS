#ifndef motor_control 
#define motor_control

/* Rotate a given direction until once again lined up on the tape. 
 * Parameter - direction with the following mapping: left = 1; right = -1. Will do nothing for all other inputs.
*/
void turn(int);

/* Rotates clockwise for a given amount of time in a given direction.
 * Parameter - time in ms
 * Parameter - direction with the following mapping: left = 1; right = -1.
*/
void rotateTime(int time, int direction);

/* Drive in a straight line.
 * Parameter - value, integer applied to pwm pulse on left motor. value * scalingC is applied to right motor
*/
void driveStraight(int value);

/* Sets all motor pin pwm values to 0. */
void killDriverMotors(void);

#endif