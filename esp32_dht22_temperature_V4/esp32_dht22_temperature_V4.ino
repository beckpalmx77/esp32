// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <time.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Ticker.h>
#include <TridentTD_LineNotify.h>

#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

WebServer Server;
AutoConnect      Portal(Server);

WiFiClient client;
const int httpPort = 8999;

/* thingspeak esp32 channel */
unsigned long myChannelNumber = 2413736;            //เลข ID
const char * myWriteAPIKey = "GU81VWWSDJIO0STE";    //API KEY

/*
  #define SSID        "SAC_HotSpot"
  #define PASSWORD    "0935757771"
*/


#define SSID        "work man-home_2.4G"                                     //ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "0900106833"                                   //ใส่ รหัส Wifi


unsigned long previousMillis = 0;
unsigned long interval = 30000;

const char* host = "171.100.56.194";  //ใส่ IP หรือ Host ของเครื่อง Database ก็ได้
String  variable1   = "-";  //ตัวแปรที่ต้องการจะส่ง
String  variable2   = "-";  //ตัวแปรที่ต้องการจะส่ง
String  variable3   = "-";  //ตัวแปรที่ต้องการจะส่ง
String  variable4   = "-";  //ตัวแปรที่ต้องการจะส่ง
String  variable5   = "-";  //ตัวแปรที่ต้องการจะส่ง
String  variable6   = "PS33_24G";  //ตัวแปรที่ต้องการจะส่ง

#define LINE_TOKEN  "XWPadPbBvbZUU6ZD2a4JCPCQaGdUg48tZEYxX0N3UTb" //ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

const char* ntpServer = "pool.ntp.org";

#define LED_BUILTIN 2

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

int loop_chk = 0;

int check_temperature_false = 0;

int value = 0;

const long  gmtOffset_sec = 7 * 3600;
const int   daylightOffset_sec = 7 * 3600;

void rootPage() {
  char content[] = "Hello, world ESP32 Temperature Send Data";
  Server.send(200, "text/plain", content);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi .. ");
  Serial.println(SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void check_wifi() {

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    //ESP.restart();
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }

}

void setup() {


  Serial.begin(115200);

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  initWiFi();

  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

  ThingSpeak.begin(client);

  Serial.print("WiFi Status = ");
  Serial.println(WiFi.status());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Serial.print("Start ...");
  Serial.println(LINE.getVersion());

  pinMode(LED_BUILTIN, OUTPUT);


  LINE.notify("Start DHT22 Temperature Application with LINE " + LINE.getVersion() + " IP Address " + WiFi.localIP().toString());
  Serial.println(F("Start DHT22 Temperature Application"));
  dht.begin();

}

void loop() {

  Portal.handleClient();
  check_wifi();

  delay(6000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  printLocalTime();

  delay(2000);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    check_temperature_false++;
    Serial.print(F("Failed to read from DHT sensor! "));
    Serial.println(check_temperature_false);
    if (check_temperature_false == 5) {
      delay(5000);
      ESP.restart();
    } else {
      return;
    }
  }


  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity (ความชื้น) : "));
  Serial.print(h);
  Serial.print(F("%  Temperature (อุณหภูมิ) : "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index (ดัชนีความร้อน) : "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  //delay(1000);

  LINE.notify(" อุณหภูมิ (Temperature) : " + String(t) + " °C  " + "  ความชื้น (Humidity) : " + String(h) + " %" + "  ดัชนีความร้อน (Heat index) : " + String(hic) + "°C");

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, f);
  ThingSpeak.setField(3, h);
  ThingSpeak.setField(4, hic);
  thingspeak_multi_write();

  variable1 = String(t);
  variable2 = String(f);
  variable3 = String(h);
  variable4 = String(hic);
  variable5 = String(WiFi.localIP().toString());

  send_data();

  digitalWrite(LED_BUILTIN, 1);

  loop_chk++;
  Serial.print("loop_chk = ");
  Serial.println(loop_chk);

  if (loop_chk == 5) {
    ESP.restart();
  }


  for (int cnt = 1; cnt <= 160; ++cnt) {

    if ((cnt % 10) == 0) {
      Serial.print(cnt);
    } else {
      Serial.print(".");
    }
    digitalWrite(LED_BUILTIN, 0);
    delay(500);
    digitalWrite(LED_BUILTIN, 1);
    delay(500);
  }
  Serial.println("");

  //delay(320000);

}


void printLocalTime() {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

}



void send_data() {

  delay(5000);
  ++value;
  digitalWrite(2, HIGH);
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/sac_empl/model/get_temperature.php?temp_c=" + variable1 + "&temp_f=" + variable2 + "&humidity=" + variable3 + "&heat_index=" + variable4 + "&ip_address=" + variable5 + "&ssid=" + variable6;
  /* + "&ip_address=" + String(WiFi.localIP()) + "&ssid=" + String(SSID); */

  //ชุด Directory ที่เก็บไฟล์ และตัวแปรที่ต้องการจะฝาก

  /* url += variable;                          //ส่งค่าตัวแปร */

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }

  digitalWrite(2, LOW);

  Serial.println("Send Data To DB Server ... ");
  Serial.println("closing connection");

}

void thingspeak_single_write(byte ch, int value) {
  int x = ThingSpeak.writeField(myChannelNumber, ch, value, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channe " + String(ch) + " update value " + String(value) + " successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void thingspeak_multi_write() {
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Multichannel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
