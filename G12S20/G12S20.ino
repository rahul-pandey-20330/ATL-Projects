#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

#define DHTPIN 4       // GPIO4
#define DHTTYPE DHT11  // DHT11 या DHT22

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// 🔑 Apna Mobile Hotspot WiFi
const char* ssid = "Rahul";
const char* password = "#3112003";

// Web page handler
void handleRoot() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<style>";
  html += "body{font-family:Arial;background:#0f172a;color:white;text-align:center;}";
  html += ".card{background:#1e293b;padding:20px;border-radius:15px;margin:20px;}";
  html += "h1{color:#38bdf8;}";
  html += ".value{font-size:40px;font-weight:bold;}";
  html += "</style></head><body>";

  html += "<h1>Weather Station</h1>";
  html += "<div class='card'>";
  html += "<p>Temperature</p>";
  html += "<div class='value'>" + String(t) + " °C</div>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<p>Humidity</p>";
  html += "<div class='value'>" + String(h) + " %</div>";
  html += "</div>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Open this IP in browser: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
