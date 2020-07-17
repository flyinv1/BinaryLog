#include "./src/BinaryLog.h"

BinaryLog logger;

void setup() {
    Serial.begin(115200);
    logger.setStream(&Serial);
    logger.onMessage(onMessage);
}

void loop() {
    // for (int i = 0; i < 32; i++) {
    //     union {
    //         float f = float(rand()) / float(rand());
    //         uint8_t b[4];
    //     } dat;
    //     logger.push(dat.b, 4);
    // }
    uint8_t dat[4] = { 0x41, 0x42, 0x43, 0x44 };
    logger.push(dat, 4);
    logger.writeById(0x02);
    delay(1000);

    Serial.println();

    uint8_t dat2[7] = { 0x41, 0x42, 0x43, 0x44, 0x55, 0x47 };
    logger.push(dat2, 6);
    logger.writeById(0x03);
    delay(1000);

    Serial.println();
}

void onMessage(const uint8_t *buffer, const size_t length) {

}