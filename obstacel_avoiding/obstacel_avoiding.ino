#include <Servo.h>

Servo myservo;

// Ultrasonic Pins
#define trigPin A1
#define echoPin A2

// L298N Motor Pins
#define IN1 7
#define IN2 6
#define IN3 5

#define IN4 4

// Servo pin
#define servoPin 10

long duration;
int distance;

// ----- Motor Functions -----
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void leftTurn() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rightTurn() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ----- Ultrasonic -----
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;
  return dist;
}

// ----- SETUP -----
void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  myservo.attach(servoPin);
  myservo.write(90);   // center position
  delay(500);
}

// ----- LOOP -----
void loop() {
  myservo.write(90);  // front
  delay(300);

  int front = getDistance();
  Serial.println(front);

  if (front > 25) {
    forward();
  } else {
    stopCar();
    delay(200);

    backward();
    delay(400);

    stopCar();
    delay(200);

    // Scan Left
    myservo.write(150);
    delay(400);
    int left = getDistance();

    // Scan Right
    myservo.write(30);
    delay(400);
    int right = getDistance();

    // Back to center
    myservo.write(90);
    delay(200);

    if (left > right) {
      leftTurn();
      delay(500);
    } else {
      rightTurn();
      delay(500);
    }
  }
}
