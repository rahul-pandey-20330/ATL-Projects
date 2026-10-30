#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define EYE_SENSOR 2
#define BUZZER 8

void setup() {
  pinMode(EYE_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Eye Blink");
  lcd.setCursor(0, 1);
  lcd.print("Detection");
  delay(2000);

  lcd.clear();
}

void loop() {

  int eyeState = digitalRead(EYE_SENSOR);

  if (eyeState == LOW) {
    // Eye blink/closed detected
    lcd.setCursor(0, 0);
    lcd.print("Eye: CLOSED   ");

    lcd.setCursor(0, 1);
    lcd.print("Blink Detected ");

    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

  } else {
    // Eye open
    lcd.setCursor(0, 0);
    lcd.print("Eye: OPEN      ");

    lcd.setCursor(0, 1);
    lcd.print("Monitoring...  ");

    digitalWrite(BUZZER, LOW);
  }

  delay(100);
}