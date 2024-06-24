// ROM.cpp
// ROM related operations
#include "ROM.h"

// Thanks to Ira @ trs-80.com: https://www.trs-80.com/wordpress/roms/checksums-mod-1/
M1_ROMs romTable[] = {
    {"1.0\0", 0xAE5D, 0xDA84, 0x4002, 0xD8E9DFA7, "Y\0"},
    {"1.1\0", 0xAE60, 0xDA45, 0x40E0, 0x2DE3AFEC, "Y\0"},
    {"1.1\0", 0xAE60, 0xDA45, 0x3E3E, 0x4BE1227E, "Y\0"},
    {"1.2\0", 0xAE60, 0xDA45, 0x40BA, 0x0D8A132E, "Y\0"},
    {"1.3\0", 0xB078, 0xDA45, 0x4006, 0xA8E60D9A, "Y\0"},

    {"1.2\0", 0xAD8C, 0xDA45, 0x40BA, 0xB1ABA28D, "N\0"},
    {"1.2\0", 0xAD8C, 0xDA45, 0x40BA, 0xFDC1F12C, "N\0"},
    {"1.2\0", 0xAD8C, 0xDA45, 0x40BA, 0xD6FD9041, "N\0"},

    {"1.3\0", 0xAED7, 0xDA45, 0x4006, 0x39F02E2F, "N\0"}
};

int numberOfROMEntries = sizeof(romTable) / sizeof(romTable[0]);

// Class constructor
ROM::ROM() {
}

void ROM::dump() {
  Serial.println(F("----- BEGIN ROM DUMP -----"));
  getROMChecksum(true);
  Serial.println(F("\n----- END ROM DUMP -----"));
}

// Get the ROM's checksum, also supports dumping ROM to ATMega's serial port
uint32_t ROM::getROMChecksum(bool sentToSerialPort) {
  CRC32 crc;
  const uint32_t totalBytes = 0x2FFF;
  uint8_t data;

  // Init control pins
  Shield::setDataLinesToInput();

  pinMode(RAS_L, OUTPUT);
  digitalWrite(RAS_L, LOW);

  pinMode(WR_L, OUTPUT);
  digitalWrite(WR_L, HIGH);
  pinMode(WR_L, INPUT);

  pinMode(RD_L, OUTPUT);
  digitalWrite(RD_L, LOW);
  
  // Note: NOT toggling the RAS and RD signals per each memory location as we do
  // for VRAM/SRAM and DRAM - this may change, for now it seems to be working
  for (uint32_t i = 0; i <= totalBytes; ++i) {
    Shield::setAddressLinesToOutput(i);       // Update address for each byte
    asmWait(5);

    // Read a byte and process
    data = PINF;

    crc.update(data);

    // send to serial port if dumping ROM
    if (sentToSerialPort) {
      Serial.print(data, HEX);
    }
  }

  // turn off RAS*, RD*, WR* lines
  Shield::turnOffReadWriteRASLines();

  return crc.finalize();
}

// See if ROM's CRC value exists in table and if it a known good or bad CRC.
bool ROM::isROMValid(uint32_t crcValue, bool silent) {
  isValid = false;

  SILENT_PRINT(silent, F("Known ROM versions: "));
  SILENT_PRINTLN(silent, numberOfROMEntries);

  SILENT_PRINT(silent, F("Checking if database has checksum/CRC32 value of: "));
  SILENT_PRINTLN(silent, crcValue, HEX);

  strcpy(romVersion, "Unknown\0");
  for (int i = 0; i < numberOfROMEntries; i++) {
    if (romTable[i].crc32 == crcValue) {
      SILENT_PRINT(silent, "ROM match found: ");
      SILENT_PRINT(silent, romTable[i].version);
      SILENT_PRINT(silent, ", valid: ");
      SILENT_PRINTLN(silent, romTable[i].valid);
      strcpy(romVersion, romTable[i].version);
      isValid = true;
      break;
    }
  }

  if (!isValid) {
    SILENT_PRINTLN(silent, F("Unknown ROM - contact RetroStack"));
  }

  return isValid;
}