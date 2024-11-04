#include "SerialManager.hpp"
#include <Arduino.h>

SerialManager::SerialManager() {
    resetBuffer();
}

void SerialManager::addDataToBuffer(const char *data, size_t length) {
    if (!length) {
        length = strlen(data);
    }

    // Add each character of the data to the buffer
    for (size_t i = 0; i < length; i++) {
        buffer[head] = data[i];
        head = (head + 1) % BUFFER_SIZE;

        // If head catches up with tail, advance tail to prevent overflow
        if (head == tail) {
            tail = (tail + 1) % BUFFER_SIZE;
        }
    }

    // Add the separator after the message if there is space left in the buffer
    if ((head + 1) % BUFFER_SIZE != tail) {  // Ensure we have space for the separator
        buffer[head] = SEPARATOR;
        head = (head + 1) % BUFFER_SIZE;

        // If head catches up with tail again, adjust tail
        if (head == tail) {
            tail = (tail + 1) % BUFFER_SIZE;
        }
    }

    // Null-terminate to ensure safe transmission
    buffer[head] = '\0';
}

void SerialManager::transmitData() {
    if (head == tail) {
        // Buffer is empty
        return;
    }

    // Transmit the buffer as a single chunk, up to the last character
    if (tail < head) {
        // Continuous block from tail to head
        Serial.write(buffer + tail, head - tail);
    } else {
        // Wrapped-around buffer: send two parts
        Serial.write(buffer + tail, BUFFER_SIZE - tail);  // From tail to end
        Serial.write(buffer, head);                       // From start to head
    }

    // Reset the buffer after transmission
    resetBuffer();
}

void SerialManager::resetBuffer() {
    head = 0;
    tail = 0;
}

bool SerialManager::isBufferFull() const {
    return (head + 1) % BUFFER_SIZE == tail;
}
