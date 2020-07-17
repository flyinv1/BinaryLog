#include <Arduino.h>
#include "BinaryLog.h"


// Constructor
BinaryLog::BinaryLog() {
    _stream = nullptr;
}

BinaryLog::~BinaryLog() {}

int BinaryLog::available() {
    return _stream->available();
}

void BinaryLog::read() {
    while (_stream->available() > 0) {
        uint8_t data = _stream->read();
        if (data == 0) {
            // packet terminator, decode the read buffer;
            uint8_t _decodeBuffer[_readBufferIndex];
            size_t _lenDecoded = unstuff(_readBuffer, _readBufferIndex, _decodeBuffer);

            // checksum
            uint8_t _messageCrc = _decodeBuffer[CRC_Index];
            uint8_t _crc = crc8(_decodeBuffer + 1, _lenDecoded - 1);

            if (_messageCrc != _crc) {
                // checksum fails, discard
                return;
            }
            
            // callback
            uint8_t _callbackId = _decodeBuffer[ID_Index];
            onMessageCallback _callbackP = _callbacks[_callbackId];
            if (_callbackP != nullptr) {
                _callbackP(_decodeBuffer + 2, _lenDecoded - 2);
            }

        } else {
            // packet is open, add to read buffer if not overflowing
            if (_readBufferIndex + 1 < 256) {
                _readBuffer[_readBufferIndex++] = data; 
            } else {
                // Buffer overflow
            }
        }
    }
}

void BinaryLog::write() {

    _writeBuffer[CRC_Index] = crc8(_writeBuffer + 1, _writeBufferIndex - 1);
    uint8_t _encodeBuffer[_writeBufferIndex];
    size_t _lenEncoded = stuff(_writeBuffer, _writeBufferIndex, _encodeBuffer);

    for (uint8_t i = 0; i < _lenEncoded; i++) {
        _stream->write(_encodeBuffer[i]);
    }

    flushWriteBuffer();
}

void BinaryLog::writeById(uint8_t id) {
    _writeBuffer[ID_Index] = id;
    write();
}

void BinaryLog::flushWriteBuffer() {
    _writeBufferIndex = 2;
    _writeBuffer[CRC_Index] = 0;
    _writeBuffer[ID_Index] = 0;
}

bool BinaryLog::push(uint8_t *buffer, size_t length) {
    if (_writeBufferIndex + length > 255) {
        return false;
    }
    for(uint8_t i = 0; i < length; i++) {
        _writeBuffer[_writeBufferIndex++] = buffer[i];
    }
    return true;
}

void BinaryLog::onMessage(onMessageCallback callback) {
    _callbacks[0] = callback;
}

void BinaryLog::onMessageById(uint8_t id, onMessageCallback callback) {
    _callbacks[id] = callback;
}

void BinaryLog::setStream(Stream *stream) {
    _stream = stream;
}

size_t BinaryLog::stuff(uint8_t *inputBuffer, size_t length, uint8_t *outputBuffer) {

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

size_t BinaryLog::unstuff(uint8_t *inputBuffer, size_t length, uint8_t *outputBuffer) {
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
            outputBuffer[write_index] = '\0';
        }
    }
}

uint8_t BinaryLog::crc8(uint8_t *buffer, size_t length) {
    // 8 bit CRC (CRC8-ITU w/ 0x07)
    uint8_t crc = 0;
    uint8_t i;

    for (i = 0; i < length; i++) {
        crc = _crcLookup[buffer[i] ^ crc];
    }

    _stream->print("crc: ");
    _stream->println(crc);

    return crc;

}







