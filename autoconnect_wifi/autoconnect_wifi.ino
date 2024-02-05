#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

ESP8266WebServer Server;
AutoConnect      Portal(Server);

void rootPage() {
  char content[] = "Hello, world ESP8266";
  Server.send(200, "text/plain", content);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  pinMode(2, OUTPUT);
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
  Portal.handleClient();
  digitalWrite(2, LOW);
  delay(1000);
  digitalWrite(2, HIGH);
  delay(2000);
}
