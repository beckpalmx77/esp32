#include <Ticker.h>

#define relay1 D0
#define relay2 D1
#define relay3 D2

Ticker tm500ms;

int loop_cnt = 0;

void  myToggle() {

  int state = digitalRead(LED_BUILTIN);  // get the current state of GPIO1 pin
  int state1 = digitalRead(relay1);  // get the current state of GPIO1 pin
  int state2 = digitalRead(relay2);  // get the current state of GPIO1 pin
  int state3 = digitalRead(relay2);  // get the current state of GPIO1 pin
  digitalWrite(LED_BUILTIN, !state);     // set pin to the opposite state
  digitalWrite(relay1, !state1);     // set pin to the opposite state
  digitalWrite(relay2, !state2);     // set pin to the state
  digitalWrite(relay3, !state3);     // set pin to the opposite state
  loop_cnt++;
  Serial.print("loop = ");
  Serial.println(loop_cnt);

  if (loop_cnt == 15) {
    ESP.restart();
  }

}

void setup() {

  Serial.begin(115200);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  tm500ms.attach(5, myToggle);

}

void loop() {
}
