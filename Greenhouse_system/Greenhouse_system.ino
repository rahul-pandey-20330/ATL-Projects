
#include <DHT.h>

// ESP32 pin configuration
const int SOIL_PIN = 34;
const int RELAY_PIN = 26;
const int DHT_PIN = 4;

#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Most relay modules are active LOW.
// Change to false if your relay is active HIGH.
const bool RELAY_ACTIVE_LOW = true;

// Replace these after sensor calibration
int DRY_VALUE = 3000;
int WET_VALUE = 1500;

// Example moisture thresholds
const int PUMP_ON_BELOW = 35;
const int PUMP_OFF_ABOVE = 55;

bool pumpOn = false;
unsigned long lastRead = 0;
const unsigned long INTERVAL = 2000;

void setPump(bool on) {
  pumpOn = on;

  digitalWrite(
    RELAY_PIN,
    RELAY_ACTIVE_LOW
      ? (on ? LOW : HIGH)
      : (on ? HIGH : LOW)
  );
}

int readSoil() {
  long total = 0;

  for (int i = 0; i < 10; i++) {
    total += analogRead(SOIL_PIN);
    delay(5);
  }

  return total / 10;
}

int getMoisturePercent(int raw) {
  if (DRY_VALUE == WET_VALUE) {
    return -1;
  }

  int percent = map(
    raw,
    DRY_VALUE,
    WET_VALUE,
    0,
    100
  );

  return constrain(percent, 0, 100);
}

void setup() {
  Serial.begin(115200);

  // Keep the pump OFF during startup
  pinMode(RELAY_PIN, OUTPUT);
  setPump(false);

  analogReadResolution(12);
  dht.begin();

  Serial.println("Smart Irrigation System");
  Serial.println("Soil | Temperature | Humidity | Pump");
}

void loop() {
  if (millis() - lastRead < INTERVAL) {
    return;
  }

  lastRead = millis();

  int raw = readSoil();
  int moisture = getMoisturePercent(raw);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.println("-----------------------------");
  Serial.print("Raw soil reading: ");
  Serial.println(raw);

  if (moisture < 0) {
    setPump(false);
    Serial.println("Calibration error: pump OFF");
    return;
  }

  Serial.print("Estimated soil moisture: ");
  Serial.print(moisture);
  Serial.println("%");

  // Hysteresis: separate ON and OFF thresholds
  if (!pumpOn && moisture < PUMP_ON_BELOW) {
    setPump(true);
  } else if (pumpOn && moisture >= PUMP_OFF_ABOVE) {
    setPump(false);
  }

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 reading failed");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Air humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }

  Serial.print("Pump status: ");
  Serial.println(pumpOn ? "ON" : "OFF");
}
