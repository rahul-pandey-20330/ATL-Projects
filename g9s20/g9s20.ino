#include <ESP8266WiFi.h>

const char* ssid = "Rahul";
const char* password = "#3112003";

WiFiServer server(80);
int gasPin = A0;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  int gasValue = analogRead(gasPin);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  
  client.println("<html><body>");
  client.println("<h1>Air Quality Monitoring</h1>");
  client.println("<p>Gas Value: " + String(gasValue) + "</p>");

  if (gasValue < 200)
    client.println("<p>Status: GOOD</p>");
  else if (gasValue < 400)
    client.println("<p>Status: MODERATE</p>");
  else
    client.println("<p>Status: POOR</p>");

  client.println("</body></html>");
  delay(1000);
}
