#include <Servo.h>

Servo solarServo;

int ldrLeft = A0;
int ldrRight = A1;

int servoPos = 90;

void setup()
{
  solarServo.attach(9);
  solarServo.write(servoPos);
  Serial.begin(9600);
}

void loop()
{

  int leftValue = analogRead(ldrLeft);
  int rightValue = analogRead(ldrRight);

  Serial.print("Left: ");
  Serial.print(leftValue);
  Serial.print("  Right: ");
  Serial.println(rightValue);

  if(leftValue > rightValue + 50)
  {
    servoPos--;
    solarServo.write(servoPos);
    delay(100);
  }

  if(rightValue > leftValue + 50)
  {
    servoPos++;
    solarServo.write(servoPos);
    delay(100);
  }

}