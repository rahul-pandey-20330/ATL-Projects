
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define BUZZER_PIN 5

#define RED_PIN 3
#define GREEN_PIN 4
#define BLUE_PIN 7

MFRC522 rfid(SS_PIN, RST_PIN);
Servo doorServo;

const int LOCK_ANGLE = 0;
const int UNLOCK_ANGLE = 90;

// YAHAN APNE CARD KI UID, USER ID AUR NAAM ADD KARO
struct User {
  const char* uid;
  const char* userID;
  const char* name;
};

User users[] = {
  {"46 40 17 07", "STU001", "White Card"},
  {"B7 31 12 64", "STU002", "keychain"},
  {"52 61 73 84", "TCH001", "Teacher"}
};

const int totalUsers = sizeof(users) / sizeof(users[0]);

void setColor(bool red, bool green, bool blue) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
}

String readUID() {
  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uid += "0";
    }

    uid += String(rfid.uid.uidByte[i], HEX);

    if (i < rfid.uid.size - 1) {
      uid += " ";
    }
  }

  uid.toUpperCase();
  return uid;
}

int findUser(String scannedUID) {
  for (int i = 0; i < totalUsers; i++) {
    if (scannedUID == users[i].uid) {
      return i;
    }
  }

  return -1;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  doorServo.attach(SERVO_PIN);
  doorServo.write(LOCK_ANGLE);

  setColor(false, false, true);

  Serial.println("RFID SMART DOOR LOCK");
  Serial.println("System Ready - Scan Card");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String scannedUID = readUID();

  Serial.println("\n--- CARD SCANNED ---");
  Serial.print("UID: ");
  Serial.println(scannedUID);

  int index = findUser(scannedUID);

  if (index != -1) {
    Serial.print("User ID: ");
    Serial.println(users[index].userID);

    Serial.print("Name: ");
    Serial.println(users[index].name);

    Serial.println("Access: AUTHORIZED");
    Serial.println("Door: UNLOCKED");

    setColor(false, true, false);
    tone(BUZZER_PIN, 1500, 150);

    doorServo.write(UNLOCK_ANGLE);
    delay(3000);

    doorServo.write(LOCK_ANGLE);
    Serial.println("Door locked again");

    setColor(false, false, true);

  } else {
    Serial.println("Name: Unknown User");
    Serial.println("User ID: Not Registered");
    Serial.println("Access: UNAUTHORIZED");
    Serial.println("Door: LOCKED");

    doorServo.write(LOCK_ANGLE);
    setColor(true, false, false);

    tone(BUZZER_PIN, 1000, 250);
    delay(350);
    tone(BUZZER_PIN, 1000, 250);
    delay(350);

    setColor(false, false, true);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(500);
}
