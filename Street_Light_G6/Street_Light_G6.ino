int ldrPin = A0;
int ledPin = 13;
int ldrValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);

  if (ldrValue < 500) {  // dark
    digitalWrite(ledPin, LOW);
  } 
  else {                // light
    digitalWrite(ledPin, HIGH);
  }

  delay(200);
}
