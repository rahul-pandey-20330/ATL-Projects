#include <SoftwareSerial.h>

SoftwareSerial bt(2, 3); // RX, TX

int led1 = 8;
int led2 = 9;
int led3 = 10;

char data;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  bt.begin(9600);
}

void loop() {
  if (bt.available()) {
    data = bt.read();

    // ALL LEDs ON
    if (data == 'A') {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
    }

    // ALL LEDs OFF
    if (data == 'B') {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
  }
}
