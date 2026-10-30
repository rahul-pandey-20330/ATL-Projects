#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);  // RX, TX

int LED = 8;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("Bluetooth Ready");
}

void loop() {

  if (BT.available()) {

    char data = BT.read();

    Serial.print("Received: ");
    Serial.println(data);

    if (data == '1') {
      digitalWrite(LED, HIGH);
      BT.println("LED ON");
    }

    else if (data == '0') {
      digitalWrite(LED, LOW);
      BT.println("LED OFF");
    }
  }
}