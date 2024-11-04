#ifndef ENCODEDSERIALMANAGER_HPP
#define ENCODEDSERIALMANAGER_HPP

#include "SerialManager.hpp"
#include <Arduino.h>

// Key-value pair structure for dictionary encoding
struct VarID {
    const char* name;
    byte id;
};

class EncodedSerialManager {
  public:
    // Constructor that accepts a dictionary for encoding
    EncodedSerialManager(const VarID varDictionary[], size_t dict_size);

    // Adds encoded data with optional raw data (used for strings and integers)
    byte lookupVarID(const char* varName);
    void addDataToBuffer(const char* varName, byte dataType, void* value, const char* params[] = nullptr, byte paramsLength = 0);
    void transmitData(void);
    void resetBuffer(void);

  private:
    const VarID* varDictionary;  // Pointer to the array of KeyValuePairs
    size_t dict_size;                // Number of items in the dictionary
    int bufferIndex = 0;
    static const size_t BUFFER_SIZE = 1024;   // Adjust as needed
    char buffer[BUFFER_SIZE];
    const int FIXED_POINT_SCALING_FACTOR = 1000;  // Fixed-point scaling factor
};

#endif // ENCODEDSERIALMANAGER_HPP
