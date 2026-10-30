#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

String readUID = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("CLEARSHEET");
  Serial.println("LABEL,Date,Time,Name,Student_ID");
}

void loop() {

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  readUID = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    readUID += String(mfrc522.uid.uidByte[i], HEX);
  }

  readUID.toUpperCase();

  String studentName = "Unknown";
  String studentID = "Unknown";

  // 🔥 Aapke 2 Students Add Kar Diye

  if (readUID == "723165") {
    studentName = "Priyanshi";
    studentID = "1008";
  }

  if (readUID == "4C1DDD0") {
    studentName = "Sarswati";
    studentID = "1009";
  }
   if (readUID == "FDE7145") {
    studentName = "Pragya";
    studentID = "1007";
  }
   if (readUID == "F2CDCE1B") {
    studentName = "Supriya";
    studentID = "1013";
  }
   if (readUID == "3211AC1B") {
    studentName = "Tapasya";
    studentID = "1006";
  }
   else if (readUID == "A5B1D50") {
    studentName = "Arohee";
    studentID = "1002";
  }

  Serial.print("DATA,DATE,TIME,");
  Serial.print(studentName);
  Serial.print(",");
  Serial.println(studentID);

  delay(2000);
}