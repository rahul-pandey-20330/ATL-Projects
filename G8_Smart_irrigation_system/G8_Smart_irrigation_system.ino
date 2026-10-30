int sensorPin = A0;     // Soil sensor analog pin
int relayPin  = 8;      // Relay control pin
int moisture;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay OFF
}

void loop() {
  moisture = analogRead(sensorPin);
  Serial.print("Moisture Value: ");
  Serial.println(moisture);

  if (moisture > 400) {     // Soil DRY -> High value
    digitalWrite(relayPin, LOW);   // Pump ON
    Serial.println("Pump ON");
  }
  else {                    // Soil WET -> Low value
    digitalWrite(relayPin, HIGH);  // Pump OFF
    Serial.println("Pump OFF");
  }

  delay(500);
}
