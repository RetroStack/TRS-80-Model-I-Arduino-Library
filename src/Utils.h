#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <avr/io.h>
// #include "Shield.h"

#define MAX_BUFFER_SIZE       100
#define MAX_INPUT_PARAMETERS  5
#define CPU_BUS_WAIT      7         // assume 16MHz arudino, asmWait is about 187.5ns per cycle, assume 1.77MHz Z80
#define SILENT_PRINT(silentPrint, ...) do { if (!silentPrint) Serial.print(__VA_ARGS__); } while (0)
#define SILENT_PRINTLN(silentPrint, ...) do { if (!silentPrint) Serial.println(__VA_ARGS__); } while (0)

extern bool inGlobalTestMode;        // State variable for test mode status
extern bool isBUSAKWired;
extern char romVersion[10];
extern uint32_t romChecksum;
extern char stringBuffer[MAX_BUFFER_SIZE];
extern char inputBuffer[MAX_BUFFER_SIZE];
extern char binaryString[9];
extern char *parameters[MAX_INPUT_PARAMETERS];
extern int  parameterCount;
// extern Set memoryChipSet[2];
extern const char* sramChips[8];
extern const char* dramChips[8];

inline void asmWait() __attribute__((always_inline));

void asmWait(uint16_t wait);
void asmWait(uint16_t outerLoopCount, uint16_t innerLoopCount);

void convertHexStringToByteArray(char* hexString, unsigned char* byteArray);
void sprintfBinary(uint8_t value);

// Called last from the variadic template function
void printLine(); // Declaration of the non-template function
void readSerialInput2();

// Template function definition
template <typename T, typename... Types>

void printLine(T first, Types... other) {
  Serial.print(first);
  printLine(other...);
}

uint8_t inputPrompt(const char* str);
void serialFlush();

#endif