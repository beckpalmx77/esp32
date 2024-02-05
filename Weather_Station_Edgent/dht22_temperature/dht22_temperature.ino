// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <time.h>
#include <DHT.h>
#include <Ticker.h>
#include <TridentTD_LineNotify.h>

WebServer Server;
AutoConnect      Portal(Server);

/* #define SSID        "SAC_HotSpot"                                     //ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "0935757771"                                   //ใส่ รหัส Wifi
*/

#define LINE_TOKEN  "zgbi6mXoK6rkJWSeFZm5wPjQfiOniYnV2MOxXeTMlA1" //ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

const char* ntpServer = "pool.ntp.org";

#define LED_BUILTIN 2

#define DHTPIN 4     // Digital pin connected to the DHT sensor

// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

DHT dht(DHTPIN, DHTTYPE);

const long  gmtOffset_sec = 7 * 3600;
const int   daylightOffset_sec = 7 * 3600;

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {

  Serial.begin(115200);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Serial.println();
  Serial.println(LINE.getVersion());

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());


  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
  LINE.notify("myarduino.net");

  Serial.println(F("DHTxx Temperature Application"));
  dht.begin();

}

void loop() {

  Portal.handleClient();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  printLocalTime();

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

  digitalWrite(LED_BUILTIN, HIGH);

  // Wait a few seconds between measurements.
  delay(600000);
  
}


void printLocalTime() {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  /*
    Serial.print("Day of week: ");
    Serial.println(&timeinfo, "%A");
    Serial.print("Month: ");
    Serial.println(&timeinfo, "%B");
    Serial.print("Day of Month: ");
    Serial.println(&timeinfo, "%d");
    Serial.print("Year: ");
    Serial.println(&timeinfo, "%Y");
    Serial.print("Hour: ");
    Serial.println(&timeinfo, "%H");
    Serial.print("Hour (12 hour format): ");
    Serial.println(&timeinfo, "%I");
    Serial.print("Minute: ");
    Serial.println(&timeinfo, "%M");
    Serial.print("Second: ");
    Serial.println(&timeinfo, "%S");

    Serial.println("Time variables");
    char timeHour[3];
    strftime(timeHour,3, "%H", &timeinfo);
    Serial.println(timeHour);
    char timeWeekDay[10];
    strftime(timeWeekDay,10, "%A", &timeinfo);
    Serial.println(timeWeekDay);
  */

  //Serial.println();

}
