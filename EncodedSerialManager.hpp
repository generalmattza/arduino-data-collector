#ifndef ENCODEDSERIALMANAGER_HPP
#define ENCODEDSERIALMANAGER_HPP

#include "SerialManager.hpp"
#include <Arduino.h>

// Key-value pair structure for dictionary encoding
struct KeyValuePair {
    const char* key;
    int value;
};

class EncodedSerialManager : public SerialManager {
  public:
    // Constructor that accepts a dictionary for encoding
    EncodedSerialManager(const KeyValuePair dictionary[], size_t dict_size);

    // Adds encoded data with optional raw data (used for strings and integers)
    void addDataToBuffer(const char *encoded_data, const String& raw_data = "");

    // Adds encoded data with a float value, rounded and scaled for fixed-point transmission
    void addDataToBufferWithFloat(const char *encoded_data, float value);

    // Enable or disable hexadecimal output
    void setHexEnable(bool enable) { hex_enable = enable; }

  private:
    // Helper function to find the encoded integer value for a given key
    int getEncodedValue(const char *key) const;

    // Helper function to print encoded value in hex or decimal
    void printEncodedValue(const char *encoded_data);

    void printRawData(const String &raw_data);

    // Helper function to print a float value as a fixed-point integer, in hex or decimal
    void printFixedPointFloat(float value);

    const KeyValuePair* dictionary;  // Pointer to the array of KeyValuePairs
    size_t dict_size;                // Number of items in the dictionary
    bool hex_enable = false;         // Flag for enabling hex encoding
};

#endif // ENCODEDSERIALMANAGER_HPP
