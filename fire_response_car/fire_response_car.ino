
/*
  4WD Fire Response Robot
  Arduino UNO + Flame Sensor + L298N + Relay + Pump
*/

const int FLAME_PIN = 2;
const int RELAY_PIN = 7;

// L298N motor driver
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// Adjust after testing your modules
const int FLAME_ACTIVE = LOW;

// Common active-LOW relay module
const int RELAY_ON  = LOW;
const int RELAY_OFF = HIGH;

bool fireDetectedBefore = false;

void setup() {
  pinMode(FLAME_PIN, INPUT);

  // Set relay OFF before normal operation
  digitalWrite(RELAY_PIN, RELAY_OFF);
  pinMode(RELAY_PIN, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();

  Serial.begin(9600);
  Serial.println("Fire Response Robot Ready");
}

void loop() {
  int flameValue = digitalRead(FLAME_PIN);

  if (flameValue == FLAME_ACTIVE) {
    stopCar();
    digitalWrite(RELAY_PIN, RELAY_ON);

    fireDetectedBefore = true;
    Serial.println("Flame detected - Car STOP, Pump ON");
  } else {
    digitalWrite(RELAY_PIN, RELAY_OFF);

    if (fireDetectedBefore) {
      stopCar();
      Serial.println("Flame cleared - Car STOP, Pump OFF");
    } else {
      moveForward();
      Serial.println("No flame - Car moving forward");
    }
  }

  delay(100);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
