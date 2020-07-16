#include <Arduino.h>
#include <Nanolog.h>


// Constructor
Nanolog::Nanolog() {
    _serialP = nullptr;
}

~Nanolog::Nanolog() {

}

void Nanolog::read() {

}

void Nanolog::write(uint8_t *buffer, uint8_t length) {

}

void Nanolog::writeById(uint8_t id, uint8_t *buffer, uint8_t length) {

}

void Nanolog::onMessage(onMessageCallback callback) {

}

void Nanolog::onMessageById(uint8_t id, onMessageByIdCallback callback) {

}

void Nanolog::setStream(Stream *stream) {
    _serialP = stream;
}

void Nanolog::stuff(uint8_t *buffer, uint8_t length) {

}

void Nanolog::unstuff(uint8_t *buffer, uint8_t length) {

}

uint8_t Nanolog::checksum(uint8_t *buffer, uint8_t length) {

}







