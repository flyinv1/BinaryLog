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

bool Nanolog::available() {
    return _serialP->available();
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

size_t Nanolog::stuff(uint8_t *inputBuffer, size_t length, uint8_t *outputBuffer) {

    // This is an implementation of COBS by jacquesf:
    // https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing

    size_t read_index = 0;
    size_t write_index = 1;
    size_t code_index = 0;

    uint8_t code = 1;

    while (read_index < length) {
        if (inputBuffer[read_index] == 0) {
            outputBuffer[code_index] = code;
            code = 1;
            code_index = write_index++;
            read_index++;
        } else {
            outputBuffer[write_index++] = inputBuffer[read_index++];
            code++;
            if (code == 0xFF) {
                outputBuffer[code_index] = code;
                code = 1;
                code_index = write_index;
            }
        }
    }

    outputBuffer[code_index] = code;

    return write_index;

}

size_t Nanolog::unstuff(uint8_t *inputBuffer, size_t length, uint8_t *outputBuffer) {
    size_t read_index = 0;
    size_t write_index = 0;
    uint8_t code;
    uint8_t i;

    while(read_index < length) {
        code = inputBuffer[read_index];

        if (read_index + code > length && code != 1) {
            return 0;
        }

        read_index++;

        for (i = 1; i < code; i++) {
            outputBuffer[write_index++] = inputBuffer[read_index++];
        }
        if(code != 0xff && read_index != length) {
            ouputBuffer[write_index] = '\0';
        }
    }
}

uint8_t Nanolog::crc8(uint8_t *buffer, uint8_t length) {
    // 8 bit CRC (CRC8-ITU w/ 0x07)
    uint8_t crc = 0;
    uint8_t i;

    for (i = 0; i < length; i++) {
        crc = crcLookup[buffer[i] ^ crc];
    }

    return crc;

}







