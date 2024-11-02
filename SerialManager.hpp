#ifndef SERIALMANAGER_HPP
#define SERIALMANAGER_HPP

#include <cstring>           // For string manipulation functions
#include <cstdint>           // For size_t and other fixed-width types if needed
#include "HardwareSerial.h"  // For serial communication

#define BUFFER_SIZE 512  // Adjust this based on your requirements

class SerialManager {
public:
  // Constructor that takes a pointer to a HardwareSerial object
  SerialManager(HardwareSerial *serial);

  // Pure virtual function to gather data from the device(s)
  virtual void gatherData() = 0;

  // Adds custom data to the buffer
  void addDataToBuffer(const char *data, size_t length);

  // Transmits the buffered data over the serial port
  void transmitData();

  // Resets the buffer index without transmitting data
  void resetBuffer();

  // Checks if the buffer is full
  bool isBufferFull() const;

protected:
  char buffer[BUFFER_SIZE];  // Buffer to store data
  size_t bufferIndex;        // Current index in the buffer

private:
  HardwareSerial *serial;  // Pointer to the serial interface
};

#endif  // SERIALMANAGER_HPP
