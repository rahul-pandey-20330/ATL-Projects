#include <Servo.h>

Servo solarServo;

int LDR_Top = A0;     // Top LDR
int LDR_Bottom = A1;  // Bottom LDR

int pos = 90;  // Start from middle

void setup() {
  solarServo.attach(9);
  Serial.begin(9600);
}

void loop() {
  int topValue = analogRead(LDR_Top);
  int bottomValue = analogRead(LDR_Bottom);

  int diff = topValue - bottomValue;

  Serial.print("Top: ");
  Serial.print(topValue);
  Serial.print("  Bottom: ");
  Serial.println(bottomValue);

  // Top is brighter → move UP
  if (diff > 50 && pos < 180) {
    pos++;
  }

  // Bottom is brighter → move DOWN
  if (diff < -50 && pos > 0) {
    pos--;
  }

  solarServo.write(pos);
  delay(15);
}
