
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

const int waterPin = A0;
const int trigPin = 9;
const int echoPin = 10;
const int greenLED = 6;
const int redLED = 7;

// Adjust these for your tank/model
const float warningDistance = 15.0;
const float dangerDistance = 7.0;

// Calibrate this value using your water sensor
const int waterThreshold = 500;

float distanceCM = 0;
int waterValue = 0;

float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

void showDisplay(const char* status,
                 const char* message) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("AI FLOOD MONITOR");
  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  display.setCursor(0, 18);
  display.print("Water: ");
  display.println(waterValue);

  display.setCursor(0, 30);
  display.print("Distance: ");

  if (distanceCM < 0) {
    display.println("No echo");
  } else {
    display.print(distanceCM, 1);
    display.println(" cm");
  }

  display.setCursor(0, 43);
  display.print("Status: ");
  display.println(status);

  display.setCursor(0, 54);
  display.println(message);

  display.display();
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found. Check wiring/address.");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println("FLOOD");
  display.println("MONITOR");
  display.display();
  delay(2000);
}

void loop() {
  waterValue = analogRead(waterPin);
  distanceCM = readDistance();

  // Smaller distance means higher water level.
  bool danger =
    (distanceCM >= 0 && distanceCM <= dangerDistance);

  bool warning =
    (distanceCM >= 0 && distanceCM <= warningDistance)
    || (waterValue >= waterThreshold);

  if (danger) {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);

    showDisplay("DANGER!", "FLOOD ALERT!");
    Serial.println("DANGER: FLOOD ALERT");
  }
  else if (warning) {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);

    showDisplay("WARNING", "Water rising!");
    Serial.println("WARNING: HIGH WATER");
  }
  else if (distanceCM < 0) {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);

    showDisplay("CHECK SENSOR", "No echo detected");
    Serial.println("Ultrasonic sensor check required");
  }
  else {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);

    showDisplay("SAFE", "Normal water level");
    Serial.println("SAFE");
  }

  Serial.print("Water sensor: ");
  Serial.print(waterValue);
  Serial.print(" | Distance: ");
  Serial.print(distanceCM);
  Serial.println(" cm");

  delay(500);
}
