#include <SoftwareSerial.h>

SoftwareSerial gsm(7,8); // RX, TX

int pir = 2;
int buzzer = 9;

String number = "+917901978908";

void setup()
{
Serial.begin(9600);
gsm.begin(9600);

pinMode(pir,INPUT);
pinMode(buzzer,OUTPUT);

delay(20000); // GSM network connect

Serial.println("System Ready");
}

void loop()
{

if(digitalRead(pir)==HIGH)
{

digitalWrite(buzzer,HIGH);

Serial.println("Motion Detected");

sendSMS();

delay(8000);   // SMS send hone ka time

makeCall();

delay(20000);

digitalWrite(buzzer,LOW);

}

}

void sendSMS()
{

gsm.println("AT+CMGF=1");
delay(1000);

gsm.print("AT+CMGS=\"");
gsm.print(number);
gsm.println("\"");

delay(1000);

gsm.print("Alert! Thief Detected in your home");

delay(500);

gsm.write(26);

delay(5000);

}

void makeCall()
{

gsm.print("ATD");
gsm.print(number);
gsm.println(";");

Serial.println("Calling...");

delay(20000);

gsm.println("ATH");

}