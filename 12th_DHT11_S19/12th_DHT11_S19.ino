int sensorPin = A0;

int yellowLED = 4;
int greenLED  = 5;
int redLED    = 6;
int buzzer    = 7;
int relayPin  = 8;

int value;
bool fullDone = false;

void setup() {
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH); // Pump ON
  Serial.begin(9600);
}

void loop() {
  value = analogRead(sensorPin);
  Serial.println(value);

  // 🟡 LOW water (EMPTY)
  if (value > 700) {
    fullDone = false;

    digitalWrite(yellowLED, HIGH);   // ✅ FIX
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);

    digitalWrite(relayPin, HIGH);
  }

  // 🟢 MEDIUM water
  else if (value <= 700 && value > 100) {
    fullDone = false;

    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);

    digitalWrite(relayPin, HIGH);
  }

  // 🔴 FULL water
  else {
    if (!fullDone) {
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
      digitalWrite(buzzer, HIGH);

      delay(6000); // 6 sec alert

      digitalWrite(redLED, LOW);
      digitalWrite(buzzer, LOW);
      digitalWrite(relayPin, LOW); // Pump OFF

      fullDone = true;
    }
  }

  delay(300);
}
