# BinaryLog

An Arduino library for logging arbitrary binary data through COBS encoded packets.

## Usage

### Setup
BinaryLog provides a wrapper for any `Stream` instance. A basic setup of BinaryLog is performed by initializing an instance and setting the stream source by pointer:

```c++
#include <BinaryLog.h>

BinaryLog log;

void setup() {
    Serial.begin(115200);
    log.setStream(&Serial);
}
```

### Writing data
The primary purpose of BinaryLog is to write out binary data for benchtop testing or simple device telemetry applications. An internal buffer is used to prepare data for transmission. Bytes are staged by calling `push()`:

```c++
void loop() {
    // Example data array
    uint8_t data[3] = {0, 1, 2};

    // The push method requires an array and length as arguments,
    // data must be converted to an array of byte length integers uint8_t
    log.push(data, 3);

    // To transmit, call the `write()` method, which encodes the data,
    // calculates a CRC8 checksum, and writes a complete packet to *Stream.
    log.write();
}
```

### Reading data
To read incoming packets, `read()` must be called frequently. All bytes available on `*Stream` are copied to an internal buffer and decoded. Once a packet is available, the `onMessage` callback is fired.

```c++

void setup() {
    Serial.begin(115200);
    log.setStream(&Serial);

    // Set the custom method to be called when a complete packet is received
    log.onMessage(customOnMessageCallback);
}

void loop() {
    log.read();
}

void customOnMessageCallback(uint8_t *buffer, size_t length) {
    // do something with incoming buffer
}
```

### Message IDs
BinaryLog supports message ID's to allow configurations with multiple callbacks. A callback by id can be registered by calling `onMessageById()`:

```c++

void setup() {
    Serial.begin(115200);
    log.setStream(&Serial);

    // Set the default custom callback (to be called when a message does include an ID)
    log.onMessage(customOnMessageCallback)

    // Set additional callbacks (up to 32)
    log.onMessageById(0, customOnMessageByIDCallback);
}

void loop() {
    log.read();
}

void customOnMessageCallback(uint8_t *buffer, size_t length) {
    // do something with incoming buffer
}

void customOnMessageByIDCallback(uint8_t *buffer, size_t length) {
    // do something with message by id
}

```

<!-- BinaryLog requires a stream instance to read and write data,  -->