//web server in SAP (soft access point) Mode
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>

#define ssid "myEsp"
#define password "12345"

#define ledPin 2

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

bool ledStatus = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());
  delay(1000);

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledOn);
  server.on("/ledoff", handle_ledOff);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP Server Started.");

}

void loop() {
  server.handleClient();
  if(ledStatus) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void handle_OnConnect() {
  ledStatus = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", getHTML());
}

void handle_ledOn(){
  ledStatus = HIGH;
  Serial.println("LED Status: ON");
  server.send(200, "text/html", getHTML());
}

void handle_ledOff(){
  ledStatus = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", getHTML());
}

void handle_NotFound(){
  server.send(404, "text/plain", "NOT FOUND");
}

String getHTML() {
  String htmlcode = "<!DOCTYPE html> <html>\n";
  htmlcode += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  htmlcode += "<title>LED Control</title>\n";

  htmlcode += "<style>html { font-family: Open Sans; display: inline-block; margin: 0px auto; text-align: center;}\n";
  htmlcode += "body { margin-top: 100px; } h1 { color: #154c79; margin: 30px auto 30px;} h3 { color: #1e81b0; margin-bottom: 50px}\n";
  htmlcode += ".button { display: inline-block; width: 150px; background-color: #3498db; border: 1px solid black; color: white; padding: 13px 30px; text-decoration: none;}\n";
  htmlcode += ".button-on { background-color: #1e81b0;}\n";
  htmlcode += ".button-on:active { background-color: #15767b;}\n";
  htmlcode += ".button-off { background-color: #5e0f1a;}\n";
  htmlcode += ".button-off:active { background-color: #4b0c15;}\n";
  htmlcode += "p { font-sixe: 18px; color: #888; margin-bottom: 10px;}\n";
  htmlcode += "</style>\n";
  htmlcode += "</head>\n";
  htmlcode += "<body>\n";
  htmlcode += "<h1>ESP32 Web Server</h1>\n";
  htmlcode += "<h3>A simple demo using Soft Access Point (SAP) Mode</h3>\n";

  if(ledStatus) {
    htmlcode += "<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">Turn it OFF</a>\n";
  } else {
    htmlcode += "<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">Turn it ON</a>\n";
  }
  htmlcode += "</body>\n";
  htmlcode += "</html>\n";
  
  return htmlcode;
}
