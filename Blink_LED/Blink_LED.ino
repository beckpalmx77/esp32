#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager

#define LED_BUILTIN1 2
#define LED_BUILTIN2 16

#define ESP_AP_NAME "ESP8266 Config AP"
#define ConfigWiFi_Pin D1

void setup() {

  Serial.begin(115200);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN1, OUTPUT);
  pinMode(LED_BUILTIN2, OUTPUT);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  if (digitalRead(ConfigWiFi_Pin) == LOW) // Press button
  {
    //reset saved settings
    wifiManager.resetSettings(); // go to ip 192.168.4.1 to config
  }

  wifiManager.autoConnect(ESP_AP_NAME);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// the loop function runs over and over again forever
void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN1, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN2, LOW);   // turn the LED on (HIGH is the voltage level)
    Serial.print("WiFi status : ");
    Serial.print(WiFi.status());
    Serial.print(" IP address : ");
    Serial.println(WiFi.localIP());
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN1, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
  } else {
    digitalWrite(LED_BUILTIN1, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN2, LOW);   // turn the LED on (HIGH is the voltage level)
  }

}
