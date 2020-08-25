#include "Arduino.h"
#include "Wire.h"
#include "stdbool.h"
#include "navigation.h"
#include "calibration.h"
#include "motor_control.h"
#include "global_values.h"

//------------
void setup() {
  Serial.begin(9600); // Setup BP-laptop serial comms

  initializePins(); // Initialize defined pins as inputs and outputs

  setupPWM(); // Initalize PWM pins with 0 values

  attachInterrupt(digitalPinToInterrupt(bumperPin), collisionResponse, RISING); // Setup collision interrupt
  //attachInterrupt(digitalPintToInterrupt(armActionSuccess)), stoneDropped, FALLING); // Detect if stone dropped

  role = getRole(); //Determine whether we are Methanos or Thanos
  delay(15000);
}

// The goods
void loop() {
  // Drive Function
  prevError = tapeFollow(prevError);
}