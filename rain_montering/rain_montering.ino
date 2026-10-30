
#include <Servo.h>

const int rainPin = A0;
const int servoPin = 9;
const int buzzerPin = 8;
const int ledPin = 7;

Servo rainServo;

// Adjust after checking Serial Monitor
const int rainThreshold = 500;

// Change angles according to your cover mechanism
const int openAngle = 0;
const int closeAngle = 90;

bool isRaining = false;

void setup() {
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  rainServo.attach(servoPin);
  rainServo.write(openAngle);

  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  Serial.println("Rain Monitoring System Ready");
}

void loop() {
  int rainValue = analogRead(rainPin);

  Serial.print("Rain Sensor Value: ");
  Serial.println(rainValue);

  // Typical rain modules give lower readings when wet.
  // Verify this behavior with your own sensor.
  bool rainDetected = (rainValue < rainThreshold);

  if (rainDetected) {
    rainServo.write(closeAngle);
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);

    Serial.println("RAIN DETECTED - COVER CLOSING");
  } else {
    rainServo.write(openAngle);
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    Serial.println("NO RAIN - COVER OPEN");
  }

  delay(300);
}
