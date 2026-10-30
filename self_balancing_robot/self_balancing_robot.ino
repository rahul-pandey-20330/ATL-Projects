
// #include <Wire.h>
// #include <math.h>

// #define MPU_ADDR 0x68

// // MPU6050
// #define SDA_PIN 21
// #define SCL_PIN 22

// // L298N motor driver
// #define IN1 18
// #define IN2 19
// #define IN3 16
// #define IN4 17

// // Tilt threshold in degrees
// const float TILT_LIMIT = 18.0;

// void stopMotors() {
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, LOW);
// }

// void forward() {
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
// }

// void backward() {
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, HIGH);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
// }

// void turnLeft() {
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, HIGH);
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
// }

// void turnRight() {
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
// }

// bool readAccelerometer(int16_t &ax, int16_t &ay, int16_t &az) {
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x3B);

//   if (Wire.endTransmission(false) != 0) {
//     return false;
//   }

//   if (Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)6) != 6) {
//     return false;
//   }

//   ax = (int16_t)((Wire.read() << 8) | Wire.read());
//   ay = (int16_t)((Wire.read() << 8) | Wire.read());
//   az = (int16_t)((Wire.read() << 8) | Wire.read());

//   return true;
// }

// void setup() {
//   Serial.begin(115200);

//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);
//   pinMode(IN3, OUTPUT);
//   pinMode(IN4, OUTPUT);

//   stopMotors();

//   Wire.begin(SDA_PIN, SCL_PIN);
//   Wire.setClock(100000);

//   // Check MPU6050
//   Wire.beginTransmission(MPU_ADDR);

//   if (Wire.endTransmission() != 0) {
//     Serial.println("MPU6050 not detected!");
//     while (true) {
//       stopMotors();
//       delay(100);
//     }
//   }

//   // Wake up sensor
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x6B);
//   Wire.write(0x00);

//   if (Wire.endTransmission() != 0) {
//     Serial.println("MPU6050 initialization failed!");
//     while (true) {
//       stopMotors();
//       delay(100);
//     }
//   }

//   Serial.println("Tilt Robot Ready!");
// }

// void loop() {
//   int16_t ax, ay, az;

//   if (!readAccelerometer(ax, ay, az)) {
//     stopMotors();
//     Serial.println("Sensor reading failed!");
//     delay(200);
//     return;
//   }

//   float roll = atan2((float)ay, (float)az)
//                * 180.0 / PI;

//   float pitch = atan2(
//     -(float)ax,
//     sqrt((float)ay * ay + (float)az * az)
//   ) * 180.0 / PI;

//   Serial.print("Roll: ");
//   Serial.print(roll, 1);
//   Serial.print(" | Pitch: ");
//   Serial.println(pitch, 1);

//   bool rollTilt = fabs(roll) >= TILT_LIMIT;
//   bool pitchTilt = fabs(pitch) >= TILT_LIMIT;

//   // Default state: stop
//   stopMotors();

//   if (!rollTilt && !pitchTilt) {
//     Serial.println("STOP");
//   }
//   else if (pitchTilt && (!rollTilt || fabs(pitch) >= fabs(roll))) {
//     if (pitch < 0) {
//       forward();
//       Serial.println("FORWARD");
//     } else {
//       backward();
//       Serial.println("BACKWARD");
//     }
//   }
//   else {
//     if (roll > 0) {
//       turnRight();
//       Serial.println("RIGHT");
//     } else {
//       turnLeft();
//       Serial.println("LEFT");
//     }
//   }

//   delay(100);
// }

// =====================================================
// Self-Balancing Robot: ESP32 (Arduino Core 3.x) + MPU6050 + L298N
// Live tuning via Serial Monitor (115200, "Newline" line ending)
//
// Commands:
//   p15.5   -> Kp = 15.5
//   i0.5    -> Ki = 0.5
//   d0.8    -> Kd = 0.8
//   o1.2    -> angleOffset = 1.2 (balance point, degrees)
//   m70     -> motor minimum PWM (deadzone)
//   z       -> robot ko balance point par pakdo, 'z' bhejo
//   v       -> telemetry print on/off
//   s       -> current settings print
// =====================================================
#include <Wire.h>
#include <math.h>

// ---------- Pins ----------
#define MPU_ADDR 0x68
#define SDA_PIN 21
#define SCL_PIN 22

#define IN1 18
#define IN2 19
#define IN3 16
#define IN4 17
#define ENA 25
#define ENB 26

// L298N ke liye 20kHz bahut zyada hai. 1-5 kHz use karo.
const int PWM_FREQ = 2000;
const int PWM_RESOLUTION = 8;  // 0-255

// ---------- Tuning (live change ho sakta hai) ----------
float Kp = 18.0;
float Ki = 0.0;
float Kd = 0.8;
float angleOffset = 0.0;  // degrees, balance point
int motorMinPWM = 60;     // motor hilne ke liye minimum PWM

// Agar wheels galat direction mein ghoomein to -1 karo
int MOTOR_DIRECTION = 1;
// Agar angle ka sign ulta ho (aage jhukne par angle negative) to -1 karo
const int SENSOR_DIRECTION = 1;

// ---------- Constants ----------
const float FALL_ANGLE = 35.0;      // isse zyada jhuka to motors band
const float RESTART_ANGLE = 3.0;    // is angle ke andar aate hi dobara start
const float ALPHA = 0.985;          // complementary filter
const unsigned long LOOP_US = 5000; // 200 Hz
const float GYRO_SCALE = 65.5;      // +/-500 dps => 65.5 LSB/(deg/s)
const float OUTPUT_DEADBAND = 1.5;  // PID output isse chhota ho to motor band

// ---------- State ----------
float gyroYBias = 0.0;
float angle = 0.0;
float integral = 0.0;
unsigned long prevMicros = 0;
unsigned long lastPrintMs = 0;
bool active = false;
bool telemetry = false;
String cmdBuf = "";

// ---------- Motors ----------
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
}

void driveOneMotor(int pwm, int pinA, int pinB, int enPin) {
  if (pwm > 0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  } else if (pwm < 0) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
  } else {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
  ledcWrite(enPin, abs(pwm));
}

void driveMotors(float out) {
  out *= MOTOR_DIRECTION;

  int pwm = 0;
  float a = fabs(out);

  if (a > OUTPUT_DEADBAND) {
    // Deadzone compensation: motorMinPWM se 255 tak scale
    float scaled = motorMinPWM + (a / 255.0) * (255 - motorMinPWM);
    pwm = (int)constrain(scaled, 0.0, 255.0);
    if (out < 0) pwm = -pwm;
  }

  driveOneMotor(pwm, IN1, IN2, ENA);
  driveOneMotor(pwm, IN3, IN4, ENB);
}

// ---------- MPU6050 ----------
void mpuWrite(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

bool readMPU(int16_t &ax, int16_t &ay, int16_t &az, int16_t &gy) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)14) != 14) return false;

  ax = (int16_t)((Wire.read() << 8) | Wire.read());
  ay = (int16_t)((Wire.read() << 8) | Wire.read());
  az = (int16_t)((Wire.read() << 8) | Wire.read());
  Wire.read(); Wire.read();                          // temperature
  Wire.read(); Wire.read();                          // gx (unused)
  gy = (int16_t)((Wire.read() << 8) | Wire.read());
  Wire.read(); Wire.read();                          // gz (unused)
  return true;
}

float accelPitch(int16_t ax, int16_t ay, int16_t az) {
  return atan2(-(float)ax, sqrt((float)ay * ay + (float)az * az)) * 180.0 / PI;
}

void calibrateGyro() {
  Serial.println("Gyro calibration: robot ko bilkul still rakho...");
  delay(1500);

  const int samples = 800;
  float sum = 0;
  int good = 0;
  for (int i = 0; i < samples; i++) {
    int16_t ax, ay, az, gy;
    if (readMPU(ax, ay, az, gy)) {
      sum += gy;
      good++;
    }
    delay(3);
  }
  if (good > 0) gyroYBias = sum / good;
  Serial.print("Gyro Y bias: ");
  Serial.println(gyroYBias);
}

// ---------- Serial commands ----------
void printSettings() {
  Serial.print("Kp="); Serial.print(Kp);
  Serial.print(" Ki="); Serial.print(Ki);
  Serial.print(" Kd="); Serial.print(Kd);
  Serial.print(" offset="); Serial.print(angleOffset);
  Serial.print(" minPWM="); Serial.println(motorMinPWM);
}

void handleCommand(String c) {
  c.trim();
  if (c.length() == 0) return;
  char cmd = c.charAt(0);
  float val = c.substring(1).toFloat();

  switch (cmd) {
    case 'p': Kp = val; break;
    case 'i': Ki = val; integral = 0; break;
    case 'd': Kd = val; break;
    case 'o': angleOffset = val; break;
    case 'm': motorMinPWM = constrain((int)val, 0, 200); break;
    case 'z': angleOffset += angle; integral = 0; break;
    case 'v': telemetry = !telemetry; break;
    case 's': break;
    default: Serial.println("Unknown command"); return;
  }
  printSettings();
}

void readSerialCommands() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n' || ch == '\r') {
      handleCommand(cmdBuf);
      cmdBuf = "";
    } else if (cmdBuf.length() < 20) {
      cmdBuf += ch;
    }
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  if (!ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION) ||
      !ledcAttach(ENB, PWM_FREQ, PWM_RESOLUTION)) {
    Serial.println("PWM setup failed!");
    while (true) delay(100);
  }
  stopMotors();

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);  // wake up
  if (Wire.endTransmission() != 0) {
    Serial.println("MPU6050 not detected!");
    while (true) delay(100);
  }

  mpuWrite(0x6B, 0x01);  // clock = gyro X PLL (zyada stable)
  mpuWrite(0x19, 0x04);  // sample rate = 200 Hz
  mpuWrite(0x1A, 0x03);  // DLPF ~44 Hz
  mpuWrite(0x1B, 0x08);  // gyro +/-500 dps
  mpuWrite(0x1C, 0x00);  // accel +/-2g
  delay(100);

  calibrateGyro();

  int16_t ax, ay, az, gy;
  if (!readMPU(ax, ay, az, gy)) {
    Serial.println("MPU6050 read failed!");
    while (true) delay(100);
  }
  angle = (accelPitch(ax, ay, az) - angleOffset) * SENSOR_DIRECTION;

  prevMicros = micros();
  printSettings();
  Serial.println("Ready. Robot ko haath se seedha karo, motors tab chalenge jab angle +-3 deg ke andar ho.");
}

// ---------- Main loop ----------
void loop() {
  readSerialCommands();

  unsigned long now = micros();
  if (now - prevMicros < LOOP_US) return;
  float dt = (now - prevMicros) * 1e-6;
  prevMicros = now;

  int16_t ax, ay, az, gy;
  if (!readMPU(ax, ay, az, gy)) {
    stopMotors();
    return;
  }
  if (dt > 0.05) dt = 0.005;

  // ----- Angle estimate -----
  float accAngle = (accelPitch(ax, ay, az) - angleOffset) * SENSOR_DIRECTION;
  float gyroRate = ((float)gy - gyroYBias) / GYRO_SCALE * SENSOR_DIRECTION;

  angle = ALPHA * (angle + gyroRate * dt) + (1.0 - ALPHA) * accAngle;

  // ----- Safety / auto restart -----
  if (fabs(angle) > FALL_ANGLE) {
    if (active) Serial.println("FALL - motors off");
    active = false;
  } else if (!active && fabs(angle) < RESTART_ANGLE) {
    active = true;
    integral = 0;
    Serial.println("ACTIVE");
  }

  if (!active) {
    stopMotors();
    return;
  }

  // ----- PID -----
  float error = angle;

  integral += error * dt;
  integral = constrain(integral, -30.0, 30.0);

  float output = Kp * error + Ki * integral + Kd * gyroRate;
  output = constrain(output, -255.0, 255.0);

  driveMotors(output);

  // ----- Telemetry -----
  if (telemetry && millis() - lastPrintMs > 100) {
    lastPrintMs = millis();
    Serial.print("A:");
    Serial.print(angle, 1);
    Serial.print(" G:");
    Serial.print(gyroRate, 0);
    Serial.print(" OUT:");
    Serial.println((int)output);
  }
}