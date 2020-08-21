#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#define LimitPin_stone PB3 //contact sensor for stone detection
#define LimitPin_claw PB4 //contact sensor for arm claw
#define LimitPin_Chassis PB12 //contact sensor on the chassis (retracted clow)
#define LimitPin_vertical_max PB15 //highest the arm can go 
#define LimitPin_vertical_min PB14 //lowest the arm can go 
#define LimitPin_horizontal_min PA8 //most distance the arm can go forward
#define LimitPin_horizontal_max PA15 //most distance the arm can retract
#define BP1_in PB6 //recieve signal from driving blue pill
#define BP2_out_stone PB11 //send signal about stone possession to driving blue pill
#define BP2_out PB7 //send signal to driving blue pill
#define PWM_OUT_A_V PB_9 //motor upward direction
#define PWM_OUT_B_V PB_8 //motor downward direction
#define PWM_OUT_A_H PA_6 //motor backward direction 
#define PWM_OUT_B_H PA_7 //motor forward direction

#define MOVEDELAY_open 30 //seconds
#define MOVEDELAY_close 5 //seconds
#define close 2
#define open 1
#define forward 1 
#define reverse -1
#define up 1 
#define down -1
#define speed_vertical 1250 //vertical motor speed
#define speed_horizontal 1250 //horizontal motor speed
#define nudge_ms 5 //slight move forward in seconds
//set rotation angles for different holes on the gauntlet
#define rotation_for_hole_1 90+15 
#define rotation_for_hole_2 90-15
#define rotation_for_hole_3 90+30
#define rotation_for_hole_4 90-30


Servo myServo_claw; //servo motor to open/close the clow
Servo myServo_rotate; //servo motor to rotate the clow
//function declaration
void control_Claw(int);
void control_Rotation(int);
int Limit_switch(int);
void time_control_horizontal_arm(int,int);
void time_control_vertical_arm(int,int);
void process_Gauntlet();
void process_Post();
void Arm_Horizontal_drop_position();

static int volatile gauntletVisitNumber = 0; //which gauntlet we're at
static int volatile count=1;
static int volatile processDoneFlag=LOW;


//navigates the gautlet and storage of stones
//signals out to the motors and servos

void process_Gauntlet(){

  control_Claw(close); //make sure the clow is closed
  
  //make sure the arm is at the starting position 
  pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
  while (digitalRead(LimitPin_Chassis)==LOW);
  pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
  delay(500);
  
  //move upward for the duration of delay
  pwm_start(PWM_OUT_B_V, 10000, 1500, speed_vertical, 0);
  delay(250);
  pwm_start(PWM_OUT_B_V, 10000, 1500, 0, 0);

  delay(500);
  control_Rotation(90);
  delay(1000);
  
  //set the angle of the arm based on which hole is it's going to drop the stone in
  Arm_Horizontal_drop_position();

  //retract arm to the max 
  pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
  while (digitalRead(LimitPin_horizontal_max)==HIGH);
  pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
  delay(1000);
  //control_Rotation(rotation_for_hole_1);

  //drop the stone
  control_Claw(open);
  delay(500);

  //close the claw and go back to the starting position 
  control_Rotation(90);
  control_Claw(close);
  delay(500);
  
  pwm_start(PWM_OUT_A_H, 10000, 1500, speed_horizontal, 0);
  while (digitalRead(LimitPin_horizontal_min)==HIGH);
  pwm_start(PWM_OUT_A_H, 10000, 1500, 0, 0); 
  delay(500);

  pwm_start(PWM_OUT_B_V, 10000, 1500, speed_horizontal, 0);
  while (digitalRead(LimitPin_Chassis)==HIGH);
  pwm_start(PWM_OUT_B_V, 10000, 1500, 0, 0);
  delay(500);

}

//set the angle of the arm based on which hole is it's going to drop the stone in
void Arm_Horizontal_drop_position(){

  if(gauntletVisitNumber==1){
    pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
    while (digitalRead(LimitPin_horizontal_max==HIGH));
    pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
    delay(500);
    control_Rotation(rotation_for_hole_1);
  }
  else if(gauntletVisitNumber==2){
    pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
    while (digitalRead(LimitPin_horizontal_max==HIGH));
    pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
    control_Rotation(rotation_for_hole_2);
  }
  else if(gauntletVisitNumber==3){
    pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
    while (digitalRead(LimitPin_horizontal_max==HIGH));
    pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
    control_Rotation(rotation_for_hole_3);
  }
  else{
    pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
    while (digitalRead(LimitPin_horizontal_max==HIGH));
    pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
    control_Rotation(rotation_for_hole_4);
  }

}

//navigates the post and collects the stones
//signals out to the motors and servos

void process_Post(){

  control_Rotation(90);
  control_Claw(open); //open the claw
  
  pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
  while (digitalRead(LimitPin_Chassis)==LOW); //go to the starting position
  pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);

  pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 0);
  while(Limit_switch(LimitPin_claw)==HIGH || Limit_switch(LimitPin_horizontal_max==HIGH)); //retract the arm until it hits the post or it's retracted to it's max lenght
  pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);

  pwm_start(PWM_OUT_A_V, 10000, 1500, 1250, 0);
  while(Limit_switch(LimitPin_claw)==LOW); //move up until claw gets to the top of the post
  pwm_start(PWM_OUT_A_V, 10000, 1500, 0, 0);

  //collect the stone from the hole and bring it up
  time_control_vertical_arm(up,600);
  control_Claw(close);
  delay(300);
  time_control_vertical_arm(up,600);
  
  //go bck to the strting position
  pwm_start(PWM_OUT_A_H, 10000, 1500, 1250, 0);
  while(digitalRead(LimitPin_horizontal_min)==HIGH);
  pwm_start(PWM_OUT_A_H, 10000, 1500, 0, 0);

  pwm_start(PWM_OUT_B_V, 10000, 1500, 1250, 0);
  while(Limit_switch(LimitPin_vertical_min)==HIGH);
  pwm_start(PWM_OUT_B_V, 10000, 1500, 0, 0);
  
}

//reads the state of the limit switch and reports it if it gets the same signal 49 out of 50 time 
// accounts for noisy signal 
int Limit_switch(int LimitPin){
    int h=0;
    int l=0;
    
    for(int i=0; i<50; i++){
        int reading = digitalRead(LimitPin);
        if(reading == HIGH){
           h++;}
        if(reading == LOW){
            l++;}
        if(h>49){
        return HIGH;}
        if(l>49){
        return LOW;}
        delay(3);
  }
}

//opens/closes the claw based on 'state'
void control_Claw(int state){

  int do_this=state;
  
  if(do_this==open){
    myServo_claw.write(90);
  }
  else{
    myServo_claw.write(180);

  }
}

//rotates the ar based on "angle"
void control_Rotation(int angle){
  myServo_rotate.write(angle);
}

//moves the horizontal motor based on "direction" and "time"
void time_control_horizontal_arm(int direction,int time){
  
  if (direction==forward) {
    pwm_start(PWM_OUT_A_H, 10000, 1500, speed_horizontal, 1);
    delay(time);
    pwm_start(PWM_OUT_A_H, 10000, 1500, 0, 0);
  }
  else {
    pwm_start(PWM_OUT_B_H, 10000, 1500, speed_horizontal, 1);
    delay(time);
    pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 0);
  }
}

//moves the vertical motor based on "direction" and "time"
void time_control_vertical_arm(int direction, int time){

    if (direction==down) {
    pwm_start(PWM_OUT_B_V, 10000, 1500, speed_vertical, 1);
    delay(time);
    pwm_start(PWM_OUT_B_V, 10000, 1500, 0, 0); 
  }
  else {
    pwm_start(PWM_OUT_A_V, 10000, 1500, speed_vertical, 1);
    delay(time);
    pwm_start(PWM_OUT_A_V, 10000, 1500, 0, 0);
  }

}


void setup() {

  Serial.begin(9600);
  
  //define pin modes
  pinMode(LimitPin_claw, INPUT);
  pinMode(LimitPin_stone, INPUT);
  pinMode(LimitPin_horizontal_max, INPUT);
  pinMode(LimitPin_horizontal_min, INPUT);
  pinMode(LimitPin_vertical_max, INPUT);
  pinMode(LimitPin_vertical_min, INPUT);
  pinMode(BP1_in, INPUT_PULLDOWN);
  pinMode(BP2_out, OUTPUT);
  pinMode(BP2_out_stone, OUTPUT);
  pinMode(LimitPin_Chassis,INPUT);

  //initialize the servos
  myServo_claw.attach(PIN_A1);
  myServo_rotate.attach(PIN_A0);

  //initialize the motors
  pwm_start(PWM_OUT_A_H, 10000, 1500, 0, 1);  
  pwm_start(PWM_OUT_B_H, 10000, 1500, 0, 1);
  pwm_start(PWM_OUT_A_V, 10000, 1500, 0, 1);  
  pwm_start(PWM_OUT_B_V, 10000, 1500, 0, 1);

  delay(18000); //45 seconds

}

//communicate with the driving blue pill, signal it to stop when the robot arrives at a gauntlet or a post
void loop() {

  control_Claw(close);
  digitalWrite(BP2_out_stone,digitalRead(LimitPin_stone));

  Serial.print(digitalRead(BP1_in));
  Serial.print("\n");

  if(digitalRead(BP1_in)==HIGH && processDoneFlag == LOW){
    
    if(digitalRead(LimitPin_stone)==LOW){
      gauntletVisitNumber++;
      process_Post();
      delay(8000);
      process_Gauntlet();
      delay(200000);
      processDoneFlag = HIGH;
      digitalWrite(BP2_out, processDoneFlag);
    }
    else
    {
      process_Post();
      processDoneFlag = HIGH;
      digitalWrite(BP2_out, processDoneFlag);
    }

  if (digitalRead(BP1_in) == LOW && processDoneFlag == HIGH){
    processDoneFlag = LOW;
    digitalWrite(BP2_out, processDoneFlag);
  }
  
}