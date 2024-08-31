// ROM.h

#ifndef ROM_H
#define ROM_H

#include <Arduino.h>
#include "CRC32.h"
#include "Utils.h"

class Model1;             // forward decleration

// Define a structure to represent each ROM entry
typedef struct {
  char version[5]; // ROM version
  unsigned int checksumA; // Checksum for ROM A
  unsigned int checksumB; // Checksum for ROM B
  unsigned int checksumC; // Checksum for ROM C
  unsigned long crc32;    // CRC-32 value
  char valid[2];          // Validity ("Yes" or "No")
} M1_ROMs;

class ROM {
  public:
    ROM(Model1* model);

    ROM();
    void dump();
    uint32_t getROMChecksum(bool=true);
    bool isROMValid(uint32_t, bool=true);
    char romVersion[10] = "?\0";
    bool isValid;

  private:
      Model1* model1;

  protected:
};

#endif // ROM_H