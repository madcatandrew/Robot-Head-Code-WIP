/* Author: Andrew Johnson (Last Update 12/21/2023) */
/* For the purposes of animating my robot's facial expressions, to be extended to all motion above the neck */

/* 
  Major TODO List:
  
  Add servos for ear motion up/down
  Add servos for eyebrow motion (Total of 19 DoF in the face is physically built)
  Test and confirm safe start positions on all servos
  Improve serial transmission of target positions to a higher-precision format (currently 255 mapped to 180 degrees with 4096 possible resolution as a maximum)
*/

#include <Servo.h>

// Define constants for the servo pins
const int JAW_RIGHT_PIN = 23;
const int JAW_LEFT_PIN = 15;
const int TONGUE_PIN = 25;
const int CHEEK_LEFT_LOWER_PIN = 21;
const int CHEEK_LEFT_UPPER_PIN = 29;
const int LIP_LOWER_PIN = 17;
const int LIP_UPPER_PIN = 27;
const int CHEEK_RIGHT_LOWER_PIN = 31;
const int CHEEK_RIGHT_UPPER_PIN = 19;

// Define Servo objects for each servo
Servo jaw_right;
Servo jaw_left;
Servo tongue;
Servo cheek_left_lower;
Servo cheek_left_upper;
Servo lip_lower;
Servo lip_upper;
Servo cheek_right_lower;
Servo cheek_right_upper;

// Define smoothing parameters
const int MIN_STEP = 5; // minimum number of steps to take
const int MAX_STEP = 20; // maximum number of steps to take
const int MAX_SPEED = 10; // maximum speed (in degrees per step)

// Define variables for current position tracking
int pos_jaw_right = 0;
int pos_jaw_left = 0;
int pos_tongue = 0;
int pos_cheek_left_lower = 0;
int pos_cheek_left_upper = 0;
int pos_lip_lower = 0;
int pos_lip_upper = 0;
int pos_cheek_right_lower = 0;
int pos_cheek_right_upper = 0;

//TODO: Add target position tracking

// Define smoothing function
void smoothMove(Servo& servo, int targetPos) {
  int currPos = servo.read();
  int diff = targetPos - currPos;
  if (diff == 0) {
    return;
  }
  int sign = diff / abs(diff);
  int steps = map(abs(diff), 0, 255, MAX_STEP, MIN_STEP);
  int speed = map(abs(diff), 0, 255, MAX_SPEED, 1);
  for (int i = 0; i < abs(diff); i++) {
    int newPos = currPos + sign * map(i, 0, abs(diff), 0, steps);
    servo.write(newPos);
    delay(speed);
  }
}

void setup() {
  // Attach servos to pins
  jaw_right.attach(JAW_RIGHT_PIN);
  jaw_left.attach(JAW_LEFT_PIN);
  tongue.attach(TONGUE_PIN);
  cheek_left_lower.attach(CHEEK_LEFT_LOWER_PIN);
  cheek_left_upper.attach(CHEEK_LEFT_UPPER_PIN);
  lip_lower.attach(LIP_LOWER_PIN);
  lip_upper.attach(LIP_UPPER_PIN);
  cheek_right_lower.attach(CHEEK_RIGHT_LOWER_PIN);
  cheek_right_upper.attach(CHEEK_RIGHT_UPPER_PIN);

  // Set start servo positions | TODO: Need to configure further for fine-tuning
  jaw_right.write(0);
  jaw_left.write(0);
  tongue.write(0);
  cheek_left_lower.write(0);
  cheek_left_upper.write(0);
  lip_lower.write(0);
  lip_upper.write(0);
  cheek_right_lower.write(0);
  cheek_right_upper.write(0);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read incoming serial bytes from Raspberry Pi animation library, as referenced by LLM
  if (Serial.available() >= 8) {
    pos_jaw_right =  map(Serial.read(), 0, 255, 0, 180);
    pos_jaw_left = map(Serial.read(), 0, 255, 180, 0); // map second jaw servo inverted
    pos_tongue = map(Serial.read(), 0, 255, 0, 180);
    pos_cheek_left_lower = map(Serial.read(), 0, 255, 0, 180);
    pos_cheek_left_upper = map(Serial.read(), 0, 255, 0, 180);
    pos_lip_lower = map(Serial.read(), 0, 255, 0, 180);
    pos_lip_upper = map(Serial.read(), 0, 255, 0, 180);
    pos_cheek_right_lower = map(Serial.read(), 0, 255, 0, 180);
    pos_cheek_right_upper = map(Serial.read(), 0, 255, 0, 180);
  }

  // Move servos smoothly to their target positions
  smoothMove(jaw_right, pos_jaw_right);
  smoothMove(jaw_left, pos_jaw_left);
  smoothMove(tongue, pos_tongue);
  smoothMove(cheek_left_lower, pos_cheek_left_lower);
  smoothMove(cheek_left_upper, pos_cheek_left_upper);
  smoothMove(lip_lower, pos_lip_lower);
  smoothMove(lip_upper, pos_lip_upper);
  smoothMove(cheek_right_lower, pos_cheek_right_lower);
  smoothMove(cheek_right_upper, pos_cheek_right_upper);
}
