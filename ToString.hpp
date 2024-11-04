#ifndef TOSTRING_HPP
#define TOSTRING_HPP

#include <Arduino.h>

// Function declarations for converting to Arduino String
String toString(const char* value);
String toString(const String& value);
String toString(int value);
String toString(float value, int decimalPlaces = 2);
String toString(double value, int decimalPlaces = 2);
String toString(bool value);

// Function declarations for converting to C-style string (const char*)
const char* toCString(const char* value);
const char* toCString(const String& value);
const char* toCString(int value);
const char* toCString(float value, int decimalPlaces = 2);
const char* toCString(double value, int decimalPlaces = 2);
const char* toCString(bool value);

#endif // TOSTRING_HPP
