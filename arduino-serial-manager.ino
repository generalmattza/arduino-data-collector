#include <Arduino.h>
#include <math.h>
#include "SerialManager.hpp"
#include "EncodedSerialManager.hpp"
// #include "ToString.hpp"

const float frequency = 1.0;         // Frequency of the sine wave in Hz
const int amplitude = 1000;          // Amplitude of the sine wave
const float samplingInterval = 0.01; // Time interval between samples in seconds
const unsigned long transmitInterval = 20;  // Transmit every 500 milliseconds

// Define the dictionary as an array of KeyValuePair
VarID varDictionary[] = {
    {"position", 0},
    {"X.velocity", 1},
    {"X.torque", 2},
    {"Y.velocity", 3},
    {"Y.torque", 4},
    {"Z.velocity", 5},
    {"Z.torque", 6},
    {"logger", 7}
};

// Number of items in the dictionary
const size_t dict_size = sizeof(varDictionary) / sizeof(varDictionary[0]);

// Create an instance of EncodedSerialManager with the custom map
EncodedSerialManager serial_manager(varDictionary, dict_size);

void setup() {
  Serial.begin(115200);
  // serial_manager.setHexEnable(true);
}

void loop() {
  static unsigned long lastTransmitTime = 0;  // Track the last transmission time

  // Generate sine wave data
  float sineValue = generateSineWave(amplitude, frequency, samplingInterval);
  float sineValue2 = generateSineWave(amplitude/2, frequency*2, samplingInterval);

  // Format data as needed, e.g., "X.velocity <sineValue>"
  // Encode "X.velocity" and add " 100.5" as raw data
  const char* params[] = {"logger"};
  serial_manager.addDataToBuffer("X.velocity", 0x01, &sineValue, params, 1);
  // serial_manager.addDataToBuffer("logger", "X.velocity " + String((int)sineValue));
  serial_manager.addDataToBuffer("X.torque", 0x01, &sineValue2);
  // serial_manager.addDataToBuffer("logger", "X.torque " + String((int)sineValue2));
  serial_manager.addDataToBuffer("Y.velocity", 0x01, &sineValue);
  // serial_manager.addDataToBuffer("logger", "Y.velocity " + String((int)sineValue+100));
  serial_manager.addDataToBuffer("Y.torque", 0x01, &sineValue2);
  // serial_manager.addDataToBuffer("logger", "Y.torque " + String((int)sineValue2));
  serial_manager.addDataToBuffer("Z.velocity", 0x01, &sineValue);
  // serial_manager.addDataToBuffer("logger", "Z.velocity " + String((int)sineValue-100));
  serial_manager.addDataToBuffer("Z.torque", 0x01, &sineValue2);
  // serial_manager.addDataToBuffer("logger", "Z.torque " + String((int)sineValue2));
  // serial_manager.addDataToBuffer("position", String((int)sineValue) + " " + String((int)sineValue2) + " " + String((int)sineValue2));


  // Check if it's time to transmit or if the buffer is full
  unsigned long currentTime = millis();
  if (currentTime - lastTransmitTime >= transmitInterval) {
    serial_manager.transmitData();
    lastTransmitTime = currentTime;  // Update the last transmit time
  }

  delay(samplingInterval * 1000); // Convert seconds to milliseconds
}

float generateSineWave(float amplitude, float frequency, float samplingInterval) {
  static float phase = 0.0;  // Track the phase of the sine wave
  float value = amplitude * sin(2 * PI * frequency * phase);
  phase += samplingInterval;  // Increment the phase
  return value;
}
