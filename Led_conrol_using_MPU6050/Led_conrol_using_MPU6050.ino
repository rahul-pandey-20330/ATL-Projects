
#include <Wire.h>
#include <math.h>

#define MPU_ADDR 0x68

#define SDA_PIN 21
#define SCL_PIN 22

#define RED_PIN   25
#define GREEN_PIN 26
#define BLUE_PIN  27

// Common cathode = false
// Common anode = true
const bool COMMON_ANODE = false;

const float TILT_LIMIT = 15.0;

void setColor(bool red, bool green, bool blue) {
  if (COMMON_ANODE) {
    digitalWrite(RED_PIN, red ? LOW : HIGH);
    digitalWrite(GREEN_PIN, green ? LOW : HIGH);
    digitalWrite(BLUE_PIN, blue ? LOW : HIGH);
  } else {
    digitalWrite(RED_PIN, red ? HIGH : LOW);
    digitalWrite(GREEN_PIN, green ? HIGH : LOW);
    digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
  }
}

bool readAccelerometer(int16_t &ax, int16_t &ay, int16_t &az) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  if (Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)6) != 6) {
    return false;
  }

  ax = (int16_t)((Wire.read() << 8) | Wire.read());
  ay = (int16_t)((Wire.read() << 8) | Wire.read());
  az = (int16_t)((Wire.read() << 8) | Wire.read());

  return true;
}

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  setColor(false, false, false);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  // Verify I2C device at address 0x68
  Wire.beginTransmission(MPU_ADDR);

  if (Wire.endTransmission() != 0) {
    Serial.println("MPU6050 not responding at 0x68!");
    setColor(true, false, false);

    while (true) {
      delay(500);
    }
  }

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);

  byte error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("MPU6050 initialization error: ");
    Serial.println(error);
    setColor(true, false, false);

    while (true) {
      delay(500);
    }
  }

  Serial.println("MPU6050 initialized successfully!");
  Serial.println("Tilt the sensor to change LED colour.");
}

void loop() {
  int16_t ax, ay, az;

  if (!readAccelerometer(ax, ay, az)) {
    Serial.println("Sensor reading failed!");
    setColor(true, false, false);
    delay(300);
    return;
  }

  // Calculate tilt angles in degrees
  float roll = atan2((float)ay, (float)az) * 180.0 / PI;

  float pitch = atan2(
    -(float)ax,
    sqrt((float)ay * ay + (float)az * az)
  ) * 180.0 / PI;

  Serial.print("Roll: ");
  Serial.print(roll, 1);

  Serial.print(" | Pitch: ");
  Serial.println(pitch, 1);

  bool rollTilt = fabs(roll) >= TILT_LIMIT;
  bool pitchTilt = fabs(pitch) >= TILT_LIMIT;

  if (!rollTilt && !pitchTilt) {
    // Level position: Green
    setColor(false, true, false);
  }
  else if (rollTilt && !pitchTilt) {
    if (roll > 0) {
      // Side tilt: Red
      setColor(true, false, false);
    } else {
      // Opposite side: Yellow
      setColor(true, true, false);
    }
  }
  else if (!rollTilt && pitchTilt) {
    if (pitch > 0) {
      // Forward/backward tilt: Blue
      setColor(false, false, true);
    } else {
      // Opposite direction: Cyan
      setColor(false, true, true);
    }
  }
  else {
    // Both axes tilted: Purple
    setColor(true, false, true);
  }

  delay(150);
}
