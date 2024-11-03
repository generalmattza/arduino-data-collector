#include "EncodedSerialManager.hpp"
#include <Arduino.h>

EncodedSerialManager::EncodedSerialManager(const KeyValuePair dictionary[], size_t dict_size)
  : dictionary(dictionary), dict_size(dict_size) {
  resetBuffer();
}

void EncodedSerialManager::addDataToBuffer(const char *encoded_data, const String &raw_data) {
    printEncodedValue(encoded_data);  // Step 1: Encode and print the encoded_data or encoded ID
    printRawData(raw_data);           // Step 2: Print raw data with hex if needed
    Serial.print(SEPARATOR);          // Step 3: Add a separator (comma) at the end
}

void EncodedSerialManager::printEncodedValue(const char *encoded_data) {
    // Try to get the encoded integer value from the dictionary
    int encoded_value = getEncodedValue(encoded_data);

    if (encoded_value != -1) {
        // If the ID is found, print in hex or decimal based on hex_enable
        if (hex_enable) {
            Serial.print(encoded_value, HEX);
        } else {
            Serial.print(encoded_value);
        }
    } else {
        // If the ID is not found, print the encoded_data as a string
        Serial.print(encoded_data);
    }
}

void EncodedSerialManager::printRawData(const String &raw_data) {
    // If raw_data is non-empty, print a space separator and then the data
    if (raw_data.length() > 0) {
        Serial.print(" ");
        
        // Print raw_data as hex if hex_enable is true and it's numeric
        if (hex_enable && raw_data.toInt() != 0) {
            Serial.print(raw_data.toInt(), HEX);
        } else {
            Serial.print(raw_data);
        }
    }
}

void EncodedSerialManager::addDataToBufferWithFloat(const char *encoded_data, float value) {
    printEncodedValue(encoded_data);    // Step 1: Print encoded ID or fallback
    Serial.print(" ");               // Step 2: Space separator
    printFixedPointFloat(value);     // Step 3: Print the fixed-point float value
    Serial.print(SEPARATOR);         // Step 4: Add separator (comma) at the end
}


void EncodedSerialManager::printFixedPointFloat(float value) {
    // Round to two decimal places, scale to fixed-point integer
    int32_t scaled_value = static_cast<int32_t>(round(value * 100));

    // Print the fixed-point integer in hex or decimal format based on hex_enable
    if (hex_enable) {
        Serial.print(scaled_value, HEX);
    } else {
        Serial.print(scaled_value);
    }
}


// Helper function to find the encoding for a given key
int EncodedSerialManager::getEncodedValue(const char *key) const {
  // Loop through the dictionary to find a matching key
  for (size_t i = 0; i < dict_size; i++) {
    if (strcmp(key, dictionary[i].key) == 0) {
      return dictionary[i].value;  // Return the corresponding value
    }
  }
  return -1;  // Return -1 if the key is not found
}
