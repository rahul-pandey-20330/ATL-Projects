int pirPin = 2;      // PIR sensor pin
int buzzerPin = 8;  // Buzzer pin
int pirState = 0;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  pirState = digitalRead(pirPin);

  if (pirState == HIGH) {
    Serial.println("Motion Detected!");
    
    delay(5000);            // Delay Lock (5 seconds)
    
    digitalWrite(buzzerPin, HIGH);  // Alarm ON
    delay(3000);            // Buzzer ON for 3 sec
    digitalWrite(buzzerPin, LOW);   // Alarm OFF
  } 
  else {
    digitalWrite(buzzerPin, LOW);
  }
}
 
