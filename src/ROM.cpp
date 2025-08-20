/*
 * ROM.cpp - Class for accessing ROM and performing operations
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "ROM.h"
#include "Model1.h"
#include "utils.h"

#define ROM_START 0x00
#define ROM_1K_LENGTH 1024
#define ROM_4K_LENGTH 4 * 1024

struct ROMSignature
{
  const char *name;
  uint16_t romA;
  uint16_t romB;
  uint16_t romC;
  uint16_t romD;
};

// List of system ROM names
const char DIAG_ROM[] PROGMEM = "ADB Diagnostic ROM";
const char SYS80_ROM5[] PROGMEM = "System-80-ROM-5 Black Label";
const char SYS80_ROM4[] PROGMEM = "System-80-ROM-4 Blue Label";
const char SYS80_ROM3[] PROGMEM = "System-80-ROM-3 Blue Label";
const char SYS80_ROM2[] PROGMEM = "System-80-ROM-2 Black Label";
const char SYS80_ROM1[] PROGMEM = "System-80-ROM-1 Black Label";
const char LNW80_ROM2[] PROGMEM = "LNW-80 Rom 2";
const char LNW80_ROM1[] PROGMEM = "LNW-80 Rom 1";
const char HT1080Z[] PROGMEM = "HT-1080z v2.2 HT-1080Z";
const char L2V1_3_TEC_KANA[] PROGMEM = "LII v1.3 TEC Kana";
const char L2V1_3_TEC[] PROGMEM = "LII v1.3 TEC";
const char L2V1_3_HD_PATCH[] PROGMEM = "LII v1.3 HD Patch";
const char L2V1_3_LC_PATCH[] PROGMEM = "LII v1.3 Lower-Case Patch";
const char L2V1_3[] PROGMEM = "LII v1.3";
const char L2V1_2_DELAY_PATCH[] PROGMEM = "LII v1.2 Delay Patch";
const char L2V1_2[] PROGMEM = "LII v1.2";
const char L2V1_1B[] PROGMEM = "LII v1.1b";
const char L2V1_1A[] PROGMEM = "LII v1.1a";
const char L2V1_0[] PROGMEM = "LII v1.0";
const char L1V1_2[] PROGMEM = "LI v1.2";
const char L1V1_1[] PROGMEM = "LI v1.1";
const char L1V1_0[] PROGMEM = "LI v1.0";

// Information sourced from https://www.trs-80.com/wordpress/roms/checksums-mod-1/
// Entries in reverse order
const ROMSignature signatures[] PROGMEM = {
    {SYS80_ROM5, 0xA74E, 0xDA67, 0x40BA, 0x0000},
    {SYS80_ROM4, 0xA74E, 0xDA67, 0x40BA, 0xB4AD},
    {SYS80_ROM3, 0xA94F, 0xDA67, 0x40BA, 0xB4AD},
    {SYS80_ROM2, 0xA94F, 0xDA67, 0x40BA, 0x0000},
    {SYS80_ROM1, 0xA94F, 0xDA67, 0x40BA, 0x0000},
    {LNW80_ROM2, 0xAB79, 0xDA56, 0x40BA, 0x0000},
    {LNW80_ROM1, 0xAB79, 0xDA45, 0x40BA, 0x0000},
    {HT1080Z, 0xC437, 0xDA30, 0x40BA, 0x0000},
    {L2V1_3_TEC_KANA, 0xA1CA, 0xDA45, 0x3DC0, 0x75AA},
    {L2V1_3_TEC, 0xA1CA, 0xDA45, 0x3DC0, 0x0000},
    {L2V1_3_HD_PATCH, 0xB77B, 0xDA45, 0x3DF9, 0x0000},
    {L2V1_3_LC_PATCH, 0xB058, 0xDA45, 0x4006, 0x0000},
    {L2V1_3, 0xB078, 0xDA45, 0x4006, 0x0000},
    {L2V1_2_DELAY_PATCH, 0xAD8C, 0xDA45, 0x40BA, 0x0000},
    {L2V1_2, 0xAE60, 0xDA45, 0x40BA, 0x0000},
    {L2V1_1B, 0xAE60, 0xDA45, 0x3E3E, 0x0000},
    {L2V1_1A, 0xAE60, 0xDA45, 0x40E0, 0x0000},
    {L2V1_0, 0xAE5D, 0xDA84, 0x4002, 0x0000},
    {L1V1_2, 0x5D0C, 0x99C2, 0x0000, 0x0000},
    {L1V1_1, 0x5A51, 0x9F9A, 0x0000, 0x0000},
    {L1V1_0, 0xF6CE, 0x0000, 0x0000, 0x0000},
    {DIAG_ROM, 0xAE31, 0x0000, 0x0000, 0x0000},
};

// Constructor - initialize ROM interface
ROM::ROM()
{
  _logger = nullptr;
}

// Set logger for debugging output
void ROM::setLogger(ILogger &logger)
{
  _logger = &logger;
}

// Get starting memory address for specified ROM number
uint16_t ROM::getROMStartAddress(uint8_t rom)
{
  if (!_checkROMNumber(rom))
    return 0;

  return ROM_START + (ROM_4K_LENGTH * rom);
}

// Get length of specified ROM
uint16_t ROM::getROMLength(uint8_t rom)
{
  if (!_checkROMNumber(rom))
    return 0;

  if (rom == 3)
  {
    return ROM_1K_LENGTH; // Japanese ROM is 1k
  }
  return ROM_4K_LENGTH;
}

// Calculate checksum for specified ROM
uint32_t ROM::getChecksum(uint8_t rom)
{
  if (!_checkROMNumber(rom))
    return 0;

  uint16_t addr = getROMStartAddress(rom);
  uint16_t size = getROMLength(rom);

  uint32_t checksum = 0;
  for (uint16_t i = 0; i < size; i++)
  {
    uint8_t value = Model1.readMemory(addr + i);
    checksum += value;
  }

  checksum &= 0xFFFF;

  return checksum;
}

// Identify ROM contents and return description string
const __FlashStringHelper *ROM::identifyROM()
{
  uint16_t a = getChecksum(0);
  uint16_t b = getChecksum(1);
  uint16_t c = getChecksum(2);
  uint16_t d = getChecksum(3);

  size_t signatureCount = sizeof(signatures) / sizeof(signatures[0]);
  for (size_t i = 0; i < signatureCount; ++i)
  {
    ROMSignature s;
    memcpy_P(&s, &signatures[i], sizeof(ROMSignature));

    // Compare A and B
    if (s.romA != a || s.romB != b)
      continue;

    // If ROM C is 0 in signature, don't care about C
    if ((s.romC == 0 || s.romC == c) && (s.romD == 0 || s.romD == d))
    {
      return (__FlashStringHelper *)s.name;
    }
  }

  return nullptr;
}

// Print ROM contents with specified formatting options
void ROM::printROMContents(uint8_t rom, PRINT_STYLE style, bool relative, uint16_t bytesPerLine)
{
  if (!_logger)
    return;
  if (!_checkROMNumber(rom))
    return;

  uint16_t addr = getROMStartAddress(rom);
  uint16_t size = getROMLength(rom);

  Model1.printMemoryContents(*_logger, addr, size, style, relative, bytesPerLine);
}

// Check if the specified ROM number is valid
bool ROM::_checkROMNumber(uint8_t rom) const
{
  if (rom > 3)
  {
    if (_logger)
      _logger->errF(F("Invalid ROM number: %d. Valid range is 0-3."), rom);
    return false;
  }
  return true;
}
