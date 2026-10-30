#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int alcoholPin = A0;
int buzzer = 8;
int led = 9;

int threshold = 400;   // Adjust as needed

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  lcd.setCursor(0,0);
  lcd.print("Alcohol Sensor");
  delay(2000);
  lcd.clear();
}

void loop() {
  int alcoholValue = analogRead(alcoholPin);
  Serial.println(alcoholValue);

  lcd.clear();

  if (alcoholValue > threshold) {
    lcd.setCursor(0,0);
    lcd.print("ALERT!");
    lcd.setCursor(0,1);
    lcd.print("Alcohol Found");

    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
  } 
  else {
    lcd.setCursor(0,0);
    lcd.print("Status:");
    lcd.setCursor(0,1);
    lcd.print("SAFE");

    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }

  delay(1000);
}