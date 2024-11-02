#include <Arduino.h>
#include <math.h>
#include "SerialManager.hpp"

const float frequency = 1.0;         // Frequency of the sine wave in Hz
const int amplitude = 1000;          // Amplitude of the sine wave
const float samplingInterval = 0.01; // Time interval between samples in seconds
const unsigned long transmitInterval = 20;  // Transmit every 500 milliseconds

SerialManager serial_manager;

void setup() {
  Serial.begin(115200);
}

void loop() {
  static unsigned long lastTransmitTime = 0;  // Track the last transmission time

  // Generate sine wave data
  float sineValue = generateSineWave();

  // Format data as needed, e.g., "X.velocity <sineValue>"
  char data[30];
  snprintf(data, sizeof(data), "X.velocity %d", (int)sineValue);
  // Add data to buffer
  serial_manager.addDataToBuffer(data);

  snprintf(data, sizeof(data), "Y.velocity %d", (int)sineValue+100);
  // Add data to buffer
  serial_manager.addDataToBuffer(data);

  snprintf(data, sizeof(data), "Z.velocity %d", (int)sineValue-100);
  // Add data to buffer
  serial_manager.addDataToBuffer(data);


  snprintf(data, sizeof(data), "logger velocity %d", (int)sineValue);
  // Add data to buffer
  serial_manager.addDataToBuffer(data);


  // Check if it's time to transmit or if the buffer is full
  unsigned long currentTime = millis();
  if (serial_manager.isBufferFull() || (currentTime - lastTransmitTime >= transmitInterval)) {
    serial_manager.transmitData();
    lastTransmitTime = currentTime;  // Update the last transmit time
  }

  delay(samplingInterval * 1000); // Convert seconds to milliseconds
}

float generateSineWave() {
  static float time = 0.0;

  // Calculate sine wave value
  float value = amplitude * (sin(2 * PI * frequency * time));  // Scale to 0 - amplitude
  time += samplingInterval;  // Increment time for the next sample

  return value;
}
