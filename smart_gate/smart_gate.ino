#include <Servo.h>

Servo gate;

#define LDR A0
#define TRIG 9
#define ECHO 10

#define LED1 7
#define LED2 4
#define LED3 5

long duration;
int distance;
int ldrValue;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  gate.attach(6);
  gate.write(0);  // Gate Closed

  Serial.begin(9600);
}

void loop() {

  ldrValue = analogRead(LDR);

  // Ultrasonic Trigger
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  // Object detect condition
  if (distance > 0 && distance < 20) {

      // 🌙 Night condition
      if (ldrValue > 500) {
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
      }

      gate.write(90);   // Open
      delay(10000);     // 10 seconds

      gate.write(0);    // Close

      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
  }
}
