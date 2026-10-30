
/*
  4-Sensor Line Following Robot
  Arduino UNO + L298N + 2 BO Motors

  IR sensor order: S1, S2, S3, S4
  Left to right
*/

const int S1 = 2;  // Far left
const int S2 = 3;  // Left center
const int S3 = 4;  // Right center
const int S4 = 5;  // Far right

// L298N motor driver
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

const int IN3 = 6;
const int IN4 = 10;
const int ENB = 11;

// Change to false if sensor gives HIGH on black line
const bool BLACK_IS_LOW = true;

int baseSpeed = 140;
int slowSpeed = 100;
int turnSpeed = 145;

int lastDirection = 0; // -1 = left, +1 = right

bool onBlack(int pin) {
  int value = digitalRead(pin);

  if (BLACK_IS_LOW) {
    return value == LOW;
  }

  return value == HIGH;
}

// Left motor: positive = forward
// Right motor: positive = forward
void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  digitalWrite(IN1, leftSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN2, leftSpeed >= 0 ? LOW : HIGH);

  digitalWrite(IN3, rightSpeed >= 0 ? HIGH : LOW);
  digitalWrite(IN4, rightSpeed >= 0 ? LOW : HIGH);

  analogWrite(ENA, abs(leftSpeed));
  analogWrite(ENB, abs(rightSpeed));
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  Serial.begin(9600);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();
  delay(1000);
}

void loop() {
  bool a = onBlack(S1);
  bool b = onBlack(S2);
  bool c = onBlack(S3);
  bool d = onBlack(S4);

  // Print sensor status: 1 means black detected
  Serial.print(a);
  Serial.print(" ");
  Serial.print(b);
  Serial.print(" ");
  Serial.print(c);
  Serial.print(" ");
  Serial.println(d);

  // Both center sensors detect the line
  if (b && c) {
    setMotors(baseSpeed, baseSpeed);
  }
  // Line is toward the left
  else if (a || b) {
    lastDirection = -1;

    if (a && !b) {
      setMotors(-turnSpeed, turnSpeed);
    } else {
      setMotors(slowSpeed, baseSpeed);
    }
  }
  // Line is toward the right
  else if (c || d) {
    lastDirection = 1;

    if (d && !c) {
      setMotors(turnSpeed, -turnSpeed);
    } else {
      setMotors(baseSpeed, slowSpeed);
    }
  }
  // No sensor sees the line
  else {
    if (lastDirection == -1) {
      setMotors(-slowSpeed, slowSpeed);
    } else if (lastDirection == 1) {
      setMotors(slowSpeed, -slowSpeed);
    } else {
      stopCar();
    }
  }

  delay(5);
}
