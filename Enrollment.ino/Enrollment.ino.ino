
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial fingerSerial(2, 3); // Arduino RX, TX
Adafruit_Fingerprint finger(&fingerSerial);

uint8_t enrollID;

uint8_t readFingerprintImage(uint8_t slot) {
  int p = -1;

  Serial.print("Place finger for ID ");
  Serial.println(enrollID);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) delay(100);
    else if (p != FINGERPRINT_OK) {
      Serial.println("Image error. Try again.");
      delay(500);
    }
  }

  p = finger.image2Tz(slot);
  if (p != FINGERPRINT_OK) {
    Serial.println("Could not process fingerprint.");
    return p;
  }
  return FINGERPRINT_OK;
}

bool enrollFingerprint(uint8_t id) {
  if (readFingerprintImage(1) != FINGERPRINT_OK) return false;

  Serial.println("Remove finger...");
  delay(1800);
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(100);
  }

  Serial.println("Place the SAME finger again.");

  if (readFingerprintImage(2) != FINGERPRINT_OK) return false;

  int p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Fingerprints did not match. Start again.");
    return false;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.print("SUCCESS: Fingerprint saved as ID ");
    Serial.println(id);
    return true;
  }

  Serial.println("Could not save fingerprint.");
  return false;
}

void setup() {
  Serial.begin(9600);
  finger.begin(57600);

  Serial.println("AS608 Enrollment");

  if (!finger.verifyPassword()) {
    Serial.println("Sensor not found. Check wiring.");
    while (true) delay(10);
  }

  Serial.println("Sensor ready.");
  Serial.println("Enter a unique ID from 1 to 127 in Serial Monitor.");
}

void loop() {
  if (!Serial.available()) return;

  int id = Serial.parseInt();
  while (Serial.available()) Serial.read();

  if (id < 1 || id > 127) {
    Serial.println("Invalid ID. Use 1 to 127.");
    return;
  }

  enrollID = (uint8_t)id;

  if (enrollFingerprint(enrollID)) {
    Serial.println("Ready for next student ID.");
  } else {
    Serial.println("Enrollment failed. Try this ID again.");
  }
}
