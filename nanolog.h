#include <Arduino.h>

class Nanolog {

    typedef void (*onMessageCallback)(const uint8_t *buffer, uint8_t length);
    typedef void (*onMessageByIdCallback)(const uint8_t id, const uint8_t *buffer, uint8_t length);

public:

    Nanolog();

    void read();

    void available();

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

    uint8_t _writeBuffer[256];
    uint8_t _readBuffer[256];
    uint8_t _readBufferLen = 0;

    uint8_t stuff(uint8_t *inputBuffer, size_t length, uint8_t *outputBuffer);
    void unstuff(uint8_t *buffer, uint8_t length);
    uint8_t checksum(uint8_t *buffer, uint8_t length);

    // CRC8-ITU 256 byte lookup
    uint8_t crcLookup[256] = {0, 7, 14, 9, 28, 27, 18, 21, 56, 63, 54, 49, 36, 35, 42, 45, 112, 119, 126, 121, 108, 107, 98, 101, 72, 79, 70, 65, 84, 83, 90, 93, 224, 231, 238, 233, 252, 251, 242, 245, 216, 223, 214, 209, 196, 195, 202, 205, 144, 151, 158, 153, 140, 139, 130, 133, 168, 175, 166, 161, 180, 179, 186, 189, 199, 192, 201, 206, 219, 220, 213, 210, 255, 248, 241, 246, 227, 228, 237, 234, 183, 176, 185, 190, 171, 172, 165, 162, 143, 136, 129, 134, 147, 148, 157, 154, 39, 32, 41, 46, 59, 60, 53, 50, 31, 24, 17, 22, 3, 4, 13, 10, 87, 80, 89, 94, 75, 76, 69, 66, 111, 104, 97, 102, 115, 116, 125, 122, 137, 142, 135, 128, 149, 146, 155, 156, 177, 182, 191, 184, 173, 170, 163, 164, 249, 254, 247, 240, 229, 226, 235, 236, 193, 198, 207, 200, 221, 218, 211, 212, 105, 110, 103, 96, 117, 114, 123, 124, 81, 86, 95, 88, 77, 74, 67, 68, 25, 30, 23, 16, 5, 2, 11, 12, 33, 38, 47, 40, 61, 58, 51, 52, 78, 73, 64, 71, 82, 85, 92, 91, 118, 113, 120, 127, 106, 109, 100, 99, 62, 57, 48, 55, 34, 37, 44, 43, 6, 1, 8, 15, 26, 29, 20, 19, 174, 169, 160, 167, 178, 181, 188, 187, 150, 145, 152, 159, 138, 141, 132, 131, 222, 217, 208, 215, 194, 197, 204, 203, 230, 225, 232, 239, 250, 253, 244, 243}
}
