#include <Esp.h>

void setup() {
    // specifying the timeout isn't currently available
    ESP.wdtEnable();
}

void loop() {
    // do useful stuff

    ESP.wdtFeed(); // service the WDT here
}
