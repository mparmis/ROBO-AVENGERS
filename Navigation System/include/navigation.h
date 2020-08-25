#ifndef navigation
#define navigation

/* Operates the tape follower. Namely, fetching the error, running it through a PID function, and sending the values to the motors.
 * Parameter - prevError: the error corresponding to our robot's last analysed state.
 * Returns - the working error
*/
int tapeFollow(int);

/* Reads IR sensors and computes the error corresponding to the robots current state. Calls respondToJunction a junction sensor is HIGH.
 * Parameter - prevError: the error corresponding to our robot's last analysed state.
 * Returns - the working error
 */
int getError(int);

/* Responds to the junction the robot is currently on. Fetches direction and drives accordingly. Robot must be on a junction. */
void respondToJunction(void);

/* Responds to a HIGH reading from our bumper sensor. Kills driver motors if expectingCollision = true. Reverses slightly and rotates 180 otherwise */
void collisionResponse(void);

/* Rotate a given direction until once again lined up on the tape. 
 * Parameter - direction with the following mapping: left = 1; right = -1. Will do nothing for all other inputs.
 */
int getDirection(int);

/* Interrupt Service Routine that indicates the stone has been dropped. Sets carryingStone boolean to false. */
void stoneDropped(void);

#endif