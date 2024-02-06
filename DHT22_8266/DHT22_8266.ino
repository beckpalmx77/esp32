#include <ESP8266WiFi.h>
#define DHTPIN D4
#include <TridentTD_LineNotify.h>

#include <DHT.h>
#include <Adafruit_Sensor.h>
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

#define SSID        "work man-home_2.4G"   //ใส่ชื่อ Wifi
#define PASSWORD    "0900106833"   //ใส่รหัส Wifi
#define LINE_TOKEN  "XWPadPbBvbZUU6ZD2a4JCPCQaGdUg48tZEYxX0N3UTb"   //ใส่ TOKEN

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting ",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (t > 30) {     //หากอุณหภูมิมากกว่า 30°C จะแจ้งเตือนไปยังไลน์
    LINE.notify("อุณหภูมิสูง = " + String(t) + " C");
    delay(1000);
  }
  Serial.println(t);
  delay(100);
}
