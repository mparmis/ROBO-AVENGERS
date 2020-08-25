#include "Arduino.h"
#include "Wire.h"
#include "stdbool.h"
#include "navigation.h"
#include "calibration.h"
#include "motor_control.h"
#include "global_values.h"

// PID controller for operation
int tapeFollow(int prevError){

  // Get error
  int error = getError(prevError);

  // Calculate corresponding gain
  double propG = kp * error;
  double derivG = kd * (error - prevError);

  // Use gain to calculate corresponding speed
  int rightSpeed = (BASE_SPEED + propG + derivG) * scalingC;
  int leftSpeed = BASE_SPEED - propG + derivG;

  // Make sure values aren't negative
  if (rightSpeed < 0) {rightSpeed = 0;}
  if (leftSpeed < 0) {leftSpeed = 0;}

  // Send power to motors
  pwm_start(rightForward, clock, period, rightSpeed, 0);
  pwm_start(leftForward, clock, period, leftSpeed, 0);

  return error;
}

// Returns PID error for use by tapeFollow(). If |returnVal| = 100, then robot is at a junction.
int getError(int prevError) {

  int error;

  // Value of false means sensor is on tape. All must be false initially
  bool leftJunction = false;
  bool farLeftOnTape = false;
  bool leftOnTape = false;
  bool rightOnTape = false;
  bool farRightOnTape = false;
  bool rightJunction = false;

  // ADC IR sensor values to determine relative location of tape.
  if (analogRead(leftJunctionSensor) > tapeThreshold) { leftJunction = true; }
  if (analogRead(farLeftTape) > tapeThreshold) { farLeftOnTape = true; }
  if (analogRead(leftTape) > tapeThreshold) { leftOnTape = true; }
  if (analogRead(rightTape) > tapeThreshold) { rightOnTape = true; }
  if (analogRead(farRightTape) > tapeThreshold) { farRightOnTape = true; }
  if (analogRead(rightJunctionSensor) > tapeThreshold) { rightJunction = true; }

  /* Ugly state machine to determine error */
  // Standard tape following
  if (farLeftOnTape == false && leftOnTape == false && rightOnTape == false && farRightOnTape == false) { error = 5 * prevError; } // No idea where tape is

  else if (farLeftOnTape == true && leftOnTape == false && rightOnTape == false && farRightOnTape == false) { error = 5; } // Far L off centre

  else if (farLeftOnTape == true && leftOnTape == true && rightOnTape == false && farRightOnTape == false) { error = 3; } // L Off centre

  else if (farLeftOnTape == false && leftOnTape == true && rightOnTape == false && farRightOnTape == false) { error = 1; } // Slightly L off centre

  else if (farLeftOnTape == false && leftOnTape == true && rightOnTape == true && farRightOnTape == false) { error = 0; } // On centre
 
  else if (farLeftOnTape == false && leftOnTape == false && rightOnTape == true && farRightOnTape == false) { error = -1; } // Slightly R off centre

  else if (farLeftOnTape == false && leftOnTape == false && rightOnTape == true && farRightOnTape == true) { error = -3; } // R Off centre

  else if (farLeftOnTape == false && leftOnTape == false && rightOnTape == false && farRightOnTape == true) { error = -5; } // Far R off centre

  else if (farLeftOnTape == true && leftOnTape == true && rightOnTape == true && farRightOnTape == false) { error = 4; }
  
  else if (farLeftOnTape == false && leftOnTape == true && rightOnTape == true && farRightOnTape == true) { error = -4; }

  else { error = 0; }

  return error;
}

// Called when a junction is detected. Responds according to current robot state.
void respondToJunction(void) {
  int direction;

  // On way there, add then evaluate. On way back, evaluate then subtract.
  if (!hasRotated) {
    junctionCount++;
    direction = getDirection(junctionCount) * role; // Inverse for other role.
  }
  else {
    direction = getDirection(junctionCount) * role; // Inverse for other role.
    junctionCount--;
  }

  turn(direction);
}

// Rotate on contact with other robot. Parameter: number of milliseconds you want to reverse before you begin rotating.
void collisionResponse() {

  if (expectingCollision) {
    killDriverMotors(); // Stop moving

    digitalWrite(armActionOutput, HIGH); // Indicate to the arm that we're ready
    while (digitalRead(armActionComplete) == LOW); // Wait until the robot indicates it has finished its action.

    // After the arm has attempted a grab, set booleans depending on whether it was successful
    if (digitalRead(armActionSuccess == HIGH)) {
        if (junctionCount != -1){ carryingStone = true; }
      }
    else { failedGrab = true; }

    digitalWrite(armActionOutput, LOW); // Action successful

    expectingCollision = false; // Toggle boolean, as expected collision has occurred
  }
  else { hasRotated = !hasRotated; } // Toggle boolean to indicate change in direction. Only if collision did not result from post/gauntlet.

  // Reverse robot slightly
  driveStraight(-1 * BASE_SPEED);
  delay(100);

  // rotate an arbitrary direction until it is once again lined up with the tape
  turn(LEFT);
}

// Returns direction of turning based on junction number. Currently assumes we're Methanos.
int getDirection(int junctionCount){
  // Convert number of passed junctions into the number of the junction we are considering
  int junctionNumber = junctionCount % 8;

  // Means you are on your way to the gauntlet
  if (junctionNumber < 1) { return STRAIGHT; }

  // Arm failed. Keep looking
  if (failedGrab) {

    failedGrab = false;
    if (hasRotated) { return RIGHT; }
    else { return LEFT; }
  }

  // If this junction has a stone and we don't, then turn down the junction.
  if (!carryingStone && junctionHasStone[junctionNumber] == true) {

    expectingCollision = true; // Robot expecting to hit the post.

    junctionHasStone[junctionNumber] == false; // We only get one shot.
    
    if (!hasRotated) { return LEFT; }
    else { return RIGHT; }
  }

  if (carryingStone) {

    if (junctionNumber == 2) {
      
      if (hasRotated) { return STRAIGHT; }
      else { return LEFT; }
    }

    if (junctionNumber == 1) {
      expectingCollision = true;
      hasRotated = false; // Reset hasRotated boolean
      junctionNumber = -3; // To account for the 4 false positives at the gauntlet.
      return RIGHT;
    }
  }

  return STRAIGHT; // If none of the above conditions apply
}

void stoneDropped(void) { carryingStone == false; }