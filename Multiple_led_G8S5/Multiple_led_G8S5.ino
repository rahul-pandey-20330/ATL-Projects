void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  // Turn all ON
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(1000);

  // Turn all OFF
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(1000);

  // One-by-one blink
  digitalWrite(2, HIGH); delay(300); digitalWrite(2, LOW);
  digitalWrite(3, HIGH); delay(300); digitalWrite(3, LOW);
  digitalWrite(4, HIGH); delay(300); digitalWrite(4, LOW);
  digitalWrite(5, HIGH); delay(300); digitalWrite(5, LOW);
}
