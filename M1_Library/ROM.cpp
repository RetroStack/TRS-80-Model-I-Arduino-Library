// ROM related operations

#include "./ROM.h"
#include "./Model1.h"

const uint16_t ROM_START = 0x00;
const uint16_t ROM_LENGTH = 1024 * 12; // 12k

ROM::ROM(Model1 *model1, ILogger *logger = nullptr)
{
  _model1 = model1;
  _logger = logger;
}

/**
 * Returns the contents of the ROMs
 */
uint8_t *ROM::getContent()
{
  return _model1->readMemory(ROM_START, ROM_LENGTH);
}
