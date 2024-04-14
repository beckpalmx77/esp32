#include <ESP8266WiFi.h>

// Define the restart delay in milliseconds (minutes)
int minutes = 1;
long RESTART_DELAY = minutes * 60 * 1000;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin("wkman_ais", "0900106833");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");


}

void loop() {

  for (int i = 1; i <= 100; i++) {
    digitalWrite(2, 0);
    delay(1000);
    digitalWrite(2, 1);
    delay(1000);
    Serial.print("i = ");
    Serial.println(i);
  }

  Serial.println("Restarting in minutes...");
  ESP.restart();



}
