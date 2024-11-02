#include "SerialManager.hpp"
#include <cstring>  // Include this for strncpy

// Constructor implementation
SerialManager::SerialManager(HardwareSerial *serial)
    : serial(serial), bufferIndex(0)
{
    buffer[0] = '\0'; // Initialize buffer with an empty string
}

void SerialManager::addDataToBuffer(const char *data, size_t length)
{
    if (bufferIndex + length < BUFFER_SIZE)
    {
        strncpy(buffer + bufferIndex, data, length);
        bufferIndex += length;
        buffer[bufferIndex] = '\0'; // Null-terminate the buffer
    }
}

void SerialManager::transmitData()
{
    if (bufferIndex > 0 && serial)
    {
        serial->print(buffer); // Transmit the entire buffer using the passed HardwareSerial instance
        resetBuffer();         // Reset the buffer index after transmitting
    }
}

void SerialManager::resetBuffer()
{
    bufferIndex = 0;
    buffer[0] = '\0';
}

bool SerialManager::isBufferFull() const
{
    return bufferIndex >= BUFFER_SIZE;
}
