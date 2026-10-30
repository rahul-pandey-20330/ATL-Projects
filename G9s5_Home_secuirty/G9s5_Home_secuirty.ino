int irPin = 8;      
int buzzerPin = 9;

int baselineState;   // Pehle se rakhe object ki state store hogi

void setup() {
  pinMode(irPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  delay(2000);   // IR sensor ko stable hone ka time

  baselineState = digitalRead(irPin);  // Store initial object reading
  Serial.print("Baseline: ");
  Serial.println(baselineState);
}

void loop() {
  int currentState = digitalRead(irPin);
  Serial.println(currentState);

  // Agar extra object aata hai → IR ka reading change hoga
  if (currentState != baselineState) {
    digitalWrite(buzzerPin, HIGH);   // Alarm ON
  } else {
    digitalWrite(buzzerPin, LOW);    // Alarm OFF
  }

  delay(50);
}
