#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27

const char* ssid = "HeartMonitor";
WiFiServer server(80);

int threshold = 550;
int bpm = 0;
int beatCount = 0;

unsigned long lastBeatTime = 0;
unsigned long lastCalcTime = 0;
unsigned long wifiStartTime;

void setup() {
  Serial.begin(115200);
  delay(2000);

  // LCD Start
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  
  // WiFi Start
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  Serial.println("WiFi Started");
  Serial.println(WiFi.softAPIP());

  server.begin();
  wifiStartTime = millis();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi Ready");
}

void loop() {

  // Sensor read after 5 sec
  if (millis() - wifiStartTime > 5000) {

    static unsigned long lastRead = 0;

    if (millis() - lastRead > 20) {
      lastRead = millis();

      int signal = analogRead(A0);

      if (signal > threshold && millis() - lastBeatTime > 500) {
        beatCount++;
        lastBeatTime = millis();
      }

      if (millis() - lastCalcTime >= 10000) {
        bpm = beatCount * 6;
        beatCount = 0;
        lastCalcTime = millis();

        // LCD Update
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Heart Rate:");
        lcd.setCursor(0,1);
        lcd.print(bpm);
        lcd.print(" BPM");
      }
    }
  }

  WiFiClient client = server.available();

  if (client) {
    while (!client.available()) delay(1);
    client.readStringUntil('\r');

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html><head>");
    client.println("<meta http-equiv='refresh' content='2'>");
    client.println("</head><body style='text-align:center;'>");
    client.println("<h1>Heart Rate Monitor</h1>");
    client.println("<h2>");
    client.println(bpm);
    client.println(" BPM</h2>");
    client.println("</body></html>");

    client.stop();
  }
}