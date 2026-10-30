#define trigPin 2
#define echoPin 3
#define relayPin 8

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH); // Relay OFF
  Serial.begin(9600);
}

void loop() {
  // Ultrasonic trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance <= 7) {   // Glass detected
    digitalWrite(relayPin, LOW);  // Pump ON
  } 
  else {
    digitalWrite(relayPin, HIGH); // Pump OFF
  }

  delay(200);
}
