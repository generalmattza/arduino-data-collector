#include "ToString.hpp"
#include <Arduino.h>

// Convert various types to an Arduino String
String toString(const char* value) {
    return String(value);
}

String toString(const String& value) {
    return value;
}

String toString(int value) {
    return String(value);
}

String toString(float value, int decimalPlaces = 2) {
    return String(value, decimalPlaces);  // Specify decimal places for float
}

String toString(double value, int decimalPlaces = 2) {
    return String(value, decimalPlaces);  // Specify decimal places for double
}

String toString(bool value) {
    return value ? "true" : "false";
}


// Function to convert different types to a C-style string (const char*)
const char* toCString(const char* value) {
    return value;
}

const char* toCString(const String& value) {
    static char buffer[64];
    value.toCharArray(buffer, sizeof(buffer));
    return buffer;
}

const char* toCString(int value) {
    static char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}

const char* toCString(float value, int decimalPlaces = 2) {
    static char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.*f", decimalPlaces, value);
    return buffer;
}

const char* toCString(double value, int decimalPlaces = 2) {
    static char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.*f", decimalPlaces, value);
    return buffer;
}

const char* toCString(bool value) {
    return value ? "true" : "false";
}

