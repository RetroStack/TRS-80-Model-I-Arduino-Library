// ROM related operations

#include "ROM.h"
#include "Model1.h"

const uint16_t ROM_START = 0x00;
const uint16_t ROM_LENGTH = 1024 * 12; // 12k
const uint16_t ROM_LENGTH_SINGLE = 4 * 1024;

ROM::ROM(Model1 *model1, ILogger *logger)
{
  _model1 = model1;
  _logger = logger;
}

/**
 * Returns the contents of the ROMs
 */
uint8_t *ROM::getContent()
{
  return _model1->readMemory(0, 2 * 1024);
}

uint32_t ROM::getChecksum(uint8_t rom)
{
  uint32_t checksum = 0;

  for (uint16_t addr = 0; addr < ROM_LENGTH_SINGLE; addr++)
  {
    uint8_t value = _model1->readMemory(ROM_START + (ROM_LENGTH_SINGLE * rom) + addr);
    checksum += value;
    // Optionally show progress every 1K
    if ((addr & 0x3FF) == 0)
    {
      // Serial.print(".");
    }
  }

  checksum &= 0xFFFF;

  return checksum;
}
