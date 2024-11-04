#include "EncodedSerialManager.hpp"
#include <Arduino.h>


EncodedSerialManager::EncodedSerialManager(const VarID varDictionary[], size_t dict_size)
  : varDictionary(varDictionary), dict_size(dict_size) {
  resetBuffer();
}



// Function to look up the byte ID for a given variable name
byte EncodedSerialManager::lookupVarID(const char* varName) {
    for (int i = 0; i < dict_size; i++) {
        if (strcmp(varDictionary[i].name, varName) == 0) {
            return varDictionary[i].id;
        }
    }
    return 0xFF;  // Return an invalid ID (0xFF) if the variable name is not found
}

void EncodedSerialManager::addDataToBuffer(const char* varName, byte dataType, void* value, const char* params[], byte paramsLength) {
    // Look up the byte ID for the main variable name
    byte varID = lookupVarID(varName);
    if (varID == 0xFF) {
        Serial.println("Error: Main variable name not found in dictionary.");
        return;
    }

    // Calculate the required size for the data packet
    int requiredSize = 6 + (dataType == 0x01 ? 4 : strlen((char*)value) + 1) + paramsLength;
    if (bufferIndex + requiredSize > BUFFER_SIZE) {
        transmitData();
    }

    buffer[bufferIndex++] = 0xAA;      // Start byte
    buffer[bufferIndex++] = varID; // Encoded variable ID from dictionary
    buffer[bufferIndex++] = dataType;  // Data type byte
    
    // Add value to the buffer based on data type
    if (dataType == 0x01) {  // Float
        // implement fixed point scaling
        int32_t fixedPointValue = *(float*)value * FIXED_POINT_SCALING_FACTOR;
        memcpy(&buffer[bufferIndex], &fixedPointValue, sizeof(fixedPointValue));
        bufferIndex += sizeof(fixedPointValue);
    } else if (dataType == 0x02) {  // Char array
        char* charArray = (char*)value;
        while (*charArray) {
            buffer[bufferIndex++] = *charArray++;
        }
        buffer[bufferIndex++] = 0x00;  // Null terminator
    }

    buffer[bufferIndex++] = paramsLength;  // Params length byte

    // Process params if provided
    if (params != nullptr && paramsLength > 0) {
        for (byte i = 0; i < paramsLength; i++) {
            byte paramID = lookupVarID(params[i]);
            if (paramID != 0xFF) {  // Only add valid param IDs
                buffer[bufferIndex++] = paramID;
            } else {
                Serial.print("Warning: Param ");
                Serial.print(params[i]);
                Serial.println(" not found in dictionary.");
            }
        }
    }

    buffer[bufferIndex++] = 0xFF;  // End byte
}

void EncodedSerialManager::transmitData() {
    for (int i = 0; i < bufferIndex; i++) {
        Serial.write(buffer[i]);
    }
    resetBuffer();  // Reset buffer after sending
}

// void setup() {
//     Serial.begin(9600);
// }

// void loop() {
//     // Example with float data and no params
//     float floatValue = 100.00;
//     addDataToBuffer("variable1", 0x01, &floatValue); // Encodes "variable1" to 0x01

//     // Example with char array data and no params
//     char text[] = "Hello";
//     addDataToBuffer("variable2", 0x02, text); // Encodes "variable2" to 0x02

//     // Example with params
//     const char* params[] = {"variable3", "variable4"}; // Param names
//     addDataToBuffer("variable1", 0x01, &floatValue, params, 2); // variable1 with float and encoded params

//     // Send the accumulated buffer data
//     sendBuffer();

//     delay(1000);  // Send data every second
// }

void EncodedSerialManager::resetBuffer(void) {
  bufferIndex = 0;
}

// void EncodedSerialManager::addDataToBuffer(const char *encoded_data, const String &raw_data) {
//     printEncodedValue(encoded_data);  // Step 1: Encode and print the encoded_data or encoded ID
//     printRawData(raw_data);           // Step 2: Print raw data with hex if needed
//     Serial.print(SEPARATOR);          // Step 3: Add a separator (comma) at the end
// }

// void EncodedSerialManager::printEncodedValue(const char *encoded_data) {
//     // Try to get the encoded integer value from the dictionary
//     int encoded_value = getEncodedValue(encoded_data);

//     if (encoded_value != -1) {
//         // If the ID is found, print in hex or decimal based on hex_enable
//         if (hex_enable) {
//             Serial.print(encoded_value, HEX);
//         } else {
//             Serial.print(encoded_value);
//         }
//     } else {
//         // If the ID is not found, print the encoded_data as a string
//         Serial.print(encoded_data);
//     }
// }

// void EncodedSerialManager::printRawData(const String &raw_data) {
//     // If raw_data is non-empty, print a space separator and then the data
//     if (raw_data.length() > 0) {
//         Serial.print(" ");
        
//         // Print raw_data as hex if hex_enable is true and it's numeric
//         if (hex_enable && raw_data.toInt() != 0) {
//             Serial.print(raw_data.toInt(), HEX);
//         } else {
//             Serial.print(raw_data);
//         }
//     }
// }

// void EncodedSerialManager::addDataToBufferWithFloat(const char *encoded_data, float value) {
//     printEncodedValue(encoded_data);    // Step 1: Print encoded ID or fallback
//     Serial.print(" ");               // Step 2: Space separator
//     printFixedPointFloat(value);     // Step 3: Print the fixed-point float value
//     Serial.print(SEPARATOR);         // Step 4: Add separator (comma) at the end
// }


// void EncodedSerialManager::printFixedPointFloat(float value) {
//     // Round to two decimal places, scale to fixed-point integer
//     int32_t scaled_value = static_cast<int32_t>(round(value * 100));

//     // Print the fixed-point integer in hex or decimal format based on hex_enable
//     if (hex_enable) {
//         Serial.print(scaled_value, HEX);
//     } else {
//         Serial.print(scaled_value);
//     }
// }


// // Helper function to find the encoding for a given key
// int EncodedSerialManager::getEncodedValue(const char *key) const {
//   // Loop through the dictionary to find a matching key
//   for (size_t i = 0; i < dict_size; i++) {
//     if (strcmp(key, dictionary[i].key) == 0) {
//       return dictionary[i].value;  // Return the corresponding value
//     }
//   }
//   return -1;  // Return -1 if the key is not found
// }
