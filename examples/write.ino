#include <BinaryLog.h>

BinaryLog logger;

void setup() {
    Serial.begin(115200);
    logger.setStream(&Serial);
}

void loop() {
    uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    logger.push(data, 10);
    logger.write();
}