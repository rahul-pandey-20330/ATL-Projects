#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int fsrPin = A0;
int fsrValue;

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop()
{
  fsrValue = analogRead(fsrPin);

  lcd.setCursor(0,0);
  lcd.print("Force Value:");

  lcd.setCursor(0,1);
  lcd.print(fsrValue);
  lcd.print("   ");

  Serial.println(fsrValue);

  delay(500);
}
  
