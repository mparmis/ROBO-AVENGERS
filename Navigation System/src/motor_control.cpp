#include "Arduino.h"
#include "Wire.h"
#include "stdbool.h"
#include "navigation.h"
#include "calibration.h"
#include "motor_control.h"
#include "global_values.h"

void turn(int direction) {
  // Rotate in the desired direction until one of the sensors is no longer lined up with the tape
  if (direction == LEFT){
    pwm_start(rightForward, clock, period, BASE_SPEED * scalingC, 0);
    pwm_start(leftBackward, clock, period, BASE_SPEED, 0);
  }
  // If not left, must turn right.
  else if (direction == RIGHT){
    pwm_start(leftForward, clock, period, BASE_SPEED, 0);
    pwm_start(rightBackward, clock, period, BASE_SPEED * scalingC, 0);
  }

  // Make sure that the one IR sensor is definitely not on the tape.
  delay(200);

  // Rotate until robot once again lines up with tape.
  if (direction == LEFT) {
    while (analogRead(leftTape) < tapeThreshold && analogRead(rightTape) < tapeThreshold) {
      pwm_start(rightForward, clock, period, BASE_SPEED * scalingC, 0);
      pwm_start(leftBackward, clock, period, BASE_SPEED, 0);
    }
  }
  else if (direction == RIGHT) {
    while (analogRead(leftTape) < tapeThreshold && analogRead(rightTape) < tapeThreshold) {
      pwm_start(leftForward, clock, period, BASE_SPEED, 0);
      pwm_start(rightBackward, clock, period, BASE_SPEED * scalingC, 0);
    }
  }
  killDriverMotors();
}

// Rotates in a given direction for a given amount of time.
void rotateTime(int runTime, int direction){
  int startTime = millis();

  if (direction == RIGHT){
    while (millis() - startTime < runTime){
      pwm_start(leftForward, clock, period, BASE_SPEED, 0);
      pwm_start(rightBackward, clock, period, BASE_SPEED * scalingC, 0);
    }
  }
  else if (direction == LEFT){
    while (millis() - startTime < runTime){
      pwm_start(leftBackward, clock, period, BASE_SPEED, 0);
      pwm_start(rightForward, clock, period, BASE_SPEED * scalingC, 0);
    }
  }

  // Kill driver motors
  killDriverMotors();
}
// Drives straight. If direction > 0, drives forward. Else, drives backwards
void driveStraight(int direction) {
  if (direction > 0 ){
    pwm_start(rightForward, clock, period, direction * scalingC, 0);
    pwm_start(leftForward, clock, period, direction, 0);
  }
  else {
    pwm_start(rightBackward, clock, period, direction * scalingC, 0);
    pwm_start(leftBackward, clock, period, direction, 0);
  }
}

// Kills motors responsible for driving motors
void killDriverMotors(void){
  pwm_start(rightForward, clock, period, 0, 0);
  pwm_start(leftForward, clock, period, 0, 0);
  pwm_start(rightBackward, clock, period, 0, 0);
  pwm_start(leftBackward, clock, period, 0, 0);
}