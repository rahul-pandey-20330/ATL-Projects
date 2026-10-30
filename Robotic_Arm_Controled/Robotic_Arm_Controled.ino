
#include <Servo.h>

// ========== JOYSTICK PINS ==========
const byte J1_X = A0;  // Base
const byte J1_Y = A1;  // Shoulder
const byte J2_X = A2;  // Elbow
const byte J2_Y = A3;  // Gripper

// ========== SERVO PINS ==========
const byte BASE_PIN = 3;
const byte SHOULDER_PIN = 5;
const byte ELBOW_PIN = 6;
const byte GRIPPER_PIN = 9;

// ========== SERVO OBJECTS ==========
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// ========== CURRENT POSITIONS ==========
int basePos = 90;
int shoulderPos = 90;
int elbowPos = 90;
int gripperPos = 90;

// ========== SAFE MOVEMENT LIMITS ==========
// Adjust these according to your arm's mechanics.

const int BASE_MIN = 10;
const int BASE_MAX = 170;

const int SHOULDER_MIN = 30;
const int SHOULDER_MAX = 150;

const int ELBOW_MIN = 10;
const int ELBOW_MAX = 170;

// These are starting limits, not guaranteed
// open/close angles for every gripper.
const int GRIPPER_MIN = 20;
const int GRIPPER_MAX = 160;

// ========== CONTROL SETTINGS ==========
const int CENTER = 512;
const int DEAD_ZONE = 100;

// Higher value = faster movement
// Lower value = slower, more precise movement
const int SPEED = 3;

unsigned long lastMoveTime = 0;
const unsigned long MOVE_INTERVAL = 20;

void setup() {
  Serial.begin(9600);

  baseServo.attach(BASE_PIN);
  shoulderServo.attach(SHOULDER_PIN);
  elbowServo.attach(ELBOW_PIN);
  gripperServo.attach(GRIPPER_PIN);

  // Starting positions
  baseServo.write(basePos);
  shoulderServo.write(shoulderPos);
  elbowServo.write(elbowPos);
  gripperServo.write(gripperPos);

  Serial.println(F("Robotic Arm Ready"));
  Serial.println(F("J1 X: Base, J1 Y: Shoulder"));
  Serial.println(F("J2 X: Elbow, J2 Y: Gripper"));
}

void loop() {
  if (millis() - lastMoveTime < MOVE_INTERVAL) {
    return;
  }

  lastMoveTime = millis();

  int j1x = analogRead(J1_X);
  int j1y = analogRead(J1_Y);
  int j2x = analogRead(J2_X);
  int j2y = analogRead(J2_Y);

  // Independent control of all four servos
  controlServo(j1x, basePos,
               BASE_MIN, BASE_MAX, baseServo);

  controlServo(j1y, shoulderPos,
               SHOULDER_MIN, SHOULDER_MAX,
               shoulderServo);

  controlServo(j2x, elbowPos,
               ELBOW_MIN, ELBOW_MAX, elbowServo);

  controlServo(j2y, gripperPos,
               GRIPPER_MIN, GRIPPER_MAX,
               gripperServo);
}

void controlServo(int joystickValue,
                  int &position,
                  int minAngle,
                  int maxAngle,
                  Servo &motor) {

  int difference = joystickValue - CENTER;

  // Joystick released: hold current position
  if (abs(difference) < DEAD_ZONE) {
    return;
  }

  // Proportional speed:
  // small joystick movement = slow
  // large joystick movement = faster
  int stepSize = map(abs(difference),
                     DEAD_ZONE, 512, 1, SPEED);

  stepSize = constrain(stepSize, 1, SPEED);

  if (difference > 0) {
    position += stepSize;
  } else {
    position -= stepSize;
  }

  position = constrain(position, minAngle, maxAngle);

  motor.write(position);
}
