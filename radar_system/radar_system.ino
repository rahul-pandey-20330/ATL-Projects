
#include <Servo.h>

Servo radarServo;

const int trigPin = 10;
const int echoPin = 11;
const int servoPin = 9;
const int ledPin = 6;
const int buzzerPin = 7;

const int detectionDistance = 30;

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return 400;
  }

  return duration * 0.0343 / 2;
}

void scanAtAngle(int angle) {
  radarServo.write(angle);
  delay(35);

  int distance = measureDistance();

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" degrees | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= detectionDistance) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  radarServo.attach(servoPin);
  Serial.begin(9600);

  radarServo.write(90);
  delay(500);

  Serial.println("Arduino Radar System Started");
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 2) {
    scanAtAngle(angle);
  }

  for (int angle = 180; angle >= 0; angle -= 2) {
    scanAtAngle(angle);
  }
}
