int ldr = A0;
int relay = 7;
int led = 13;
int threshold = 500;  // Adjust based on your room light

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);

  digitalWrite(relay, LOW);  // Initially OFF
  digitalWrite(led, LOW);

  Serial.begin(9600);
}

void loop() {
  int value = analogRead(ldr);
  Serial.println(value);

  if (value > threshold) {
    digitalWrite(relay, HIGH);  // Relay ON
    digitalWrite(led, HIGH);    // LED ON
  } else {
    digitalWrite(relay, LOW);   // Relay OFF
    digitalWrite(led, LOW);     // LED OFF
  }

  delay(200);
}
