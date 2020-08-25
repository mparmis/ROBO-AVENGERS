#ifndef calibration
#define calibration

/* Returns the robot's role for this round. Determined by the role switch on the chassis of the robot.
 * Returns: 1 for THANOS and -1 for METHANOS
*/
int getRole(void);

/* Prints the mv readings from the analog pins corresponding to each IR sensor.
*  Legend: LJS = Left Junction Sensor; FL = Far Left Sensor; L = Left Sensor
*  Legend: RJS = Right Junction Sensor; FR = Far Right Sensor; R = Right Sensor
*/
void calibrateIRSensors(void);

/* Initializes all input and output pins. */
void initializePins(void);

/* Initialzies the PWM output for the motor pins. */
void setupPWM(void);

#endif