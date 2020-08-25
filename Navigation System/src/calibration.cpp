#include "Arduino.h"
#include "Wire.h"
#include "stdbool.h"
#include "navigation.h"
#include "calibration.h"
#include "motor_control.h"
#include "global_values.h"

// Reads switch to determine whether we are Thanos or Methanos
int getRole(void){
   int switchPos = digitalRead(rolePin); 

   if (switchPos == HIGH) { return METHANOS; }
   else { return THANOS; }
}

// Prints readings from all IR sensors to Serial
void calibrateIRSensors(void){
  delay(100);
  Serial.print("[LJS, FL, L, R, FR, RJS]");
  Serial.print(analogRead(leftJunctionSensor));
  Serial.print(", ");

  Serial.print(analogRead(farLeftTape));
  Serial.print(", ");

  Serial.print(analogRead(leftTape));
  Serial.print(", ");

  Serial.print(analogRead(rightTape));
  Serial.print(", ");

  Serial.print(analogRead(farRightTape));
  Serial.print(", ");

  Serial.println(analogRead(rightJunctionSensor));
}

// Initializes all relevant Bluepill pins. Call on startup only.
void initializePins(void) {
  // Initialize drive system output pins
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(armActionOutput, OUTPUT);

  // Initialize drive system input pins
  pinMode(rightTape, INPUT);
  pinMode(leftTape, INPUT);
  pinMode(farLeftTape, INPUT);
  pinMode(farRightTape, INPUT);
  pinMode(bumperPin, INPUT_PULLDOWN);
  pinMode(armActionComplete, INPUT_PULLDOWN);
  pinMode(armActionSuccess, INPUT_PULLDOWN);
}

// Initializes PWM pins for driver motor operation. Call on startup only.
void setupPWM(void){
  pwm_start(rightForward, clock, period, 0, 1);
  pwm_start(rightBackward, clock, period, 0, 1);
  pwm_start(leftForward, clock, period, 0, 1);
  pwm_start(leftBackward, clock, period, 0, 1);
}