#include <Servo.h>

Servo gate;

int sensor1 = 2;
int sensor2 = 3;

int buzzer = 6;
int redLED = 7;
int greenLED = 8;

void setup() {

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  gate.attach(9);

  // Initially gate open
  gate.write(90);

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, LOW);
}

void loop() {

  int trainApproaching = digitalRead(sensor1);
  int trainPassed = digitalRead(sensor2);

  // Train approaching
  if (trainApproaching == LOW) {

    gate.write(0);              // Close gate

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

    digitalWrite(buzzer, HIGH);
  }

  // Train has crossed
  if (trainPassed == LOW) {

    gate.write(90);             // Open gate

    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);

    digitalWrite(buzzer, LOW);
  }
}