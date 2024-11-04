#ifndef SERIALMANAGER_HPP
#define SERIALMANAGER_HPP

#include <Arduino.h>

class SerialManager {
  public:
    SerialManager();

    // Adds data to the buffer, wrapping around if needed
    void addDataToBuffer(const char *data, size_t length = 0);
    
    // Transmits the contents of the buffer in a single chunk
    void transmitData();
    
    // Resets the buffer pointers
    void resetBuffer();
    
    // Checks if the buffer is full
    bool isBufferFull() const;

  protected:  // Change to protected to allow derived classes access
    static const size_t BUFFER_SIZE = 1024;   // Adjust as needed
    static const char SEPARATOR = ',';       // Separator between messages
    char buffer[BUFFER_SIZE];
    size_t head = 0;  // Points to the next position to write
    size_t tail = 0;  // Points to the next position to read
};

#endif // SERIALMANAGER_HPP
