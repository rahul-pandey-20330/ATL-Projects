int touchPin = 2;
int led = 13;

void setup()
{
pinMode(touchPin, INPUT);
pinMode(led, OUTPUT);
}

void loop()
{

if(digitalRead(touchPin) == HIGH)
{

digitalWrite(led, HIGH);
delay(300);
digitalWrite(led, LOW);
delay(300);

}

}