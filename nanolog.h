#include <Arduino.h>

class Nanolog {

    typedef void (*onMessageCallback)(const uint8_t *buffer, uint8_t length);
    typedef void (*onMessageByIdCallback)(const uint8_t id, const uint8_t *buffer, uint8_t length);

public:

    Nanolog();

    void read();

    void write(uint8_t *buffer);

    void writeById(uint8_t id, uint8_t *buffer);

    void onMessage(uint8_t id, onMessageCallback callback);

    void onMessageById(uint8_t id, onMessageByIdCallback callback);

    void setStreamSource(Stream *stream);

private:

    // Pointer to stream (Serial or SoftSerial instance)
    Serial *_serialP;

    // Array of ids attatched to message instances
    uint8_t _ids[32];
    onMessageCallback _callbacks[32];
    
    uint8_t _readBuffer[256];
    uint8_t _readBufferLen = 0;

    uint8_t stuff(uint8_t *buffer, uint8_t length);
    void unstuff(uint8_t *buffer, uint8_t length);
    uint8_t checksum(uint8_t *buffer, uint8_t length);
}
