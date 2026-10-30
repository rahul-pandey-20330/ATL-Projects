#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

void handleRoot() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  String page = "<html><body>";
  page += "<h2>ESP32 Temperature & Humidity</h2>";
  page += "<p>Temperature: " + String(t) + " °C</p>";
  page += "<p>Humidity: " + String(h) + " %</p>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFiManager wm;

  // ESP32 AP name
  bool res = wm.autoConnect("ESP32_TempSetup");

  if (!res) {
    Serial.println("WiFi Failed");
    ESP.restart();
  }

  Serial.println("WiFi Connected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
