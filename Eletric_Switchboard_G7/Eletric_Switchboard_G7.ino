int button1 = 2;
int button2 = 3;

int led1 = 8;
int led2 = 9;

bool led1State = false;
bool led2State = false;

int lastButton1 = HIGH;
int lastButton2 = HIGH;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  int b1 = digitalRead(button1);
  int b2 = digitalRead(button2);

  // Button 1 controls LED 1
  if (b1 == LOW && lastButton1 == HIGH) {
    led1State = !led1State;
    digitalWrite(led1, led1State);
    delay(200);
  }

  // Button 2 controls LED 2
  if (b2 == LOW && lastButton2 == HIGH) {
    led2State = !led2State;
    digitalWrite(led2, led2State);
    delay(200);
  }

  lastButton1 = b1;
  lastButton2 = b2;
}
