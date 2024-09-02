// ROM related operations

#include "CRC32.h"
#include "./ROM.h"
#include "./Model1.h"

// Class constructor
ROM::ROM(ILogger *logger, Model1 *model)
{
  _model1 = model;
  _logger = logger;
}

uint32_t ROM::getChecksum()
{
  CRC32 crc;
  const uint32_t totalBytes = 0x2FFF;

  // Init control pins
  _model1->setDataLinesToInput();

  pinMode(RAS_L, OUTPUT);
  digitalWrite(RAS_L, LOW);

  pinMode(WR_L, OUTPUT);
  digitalWrite(WR_L, HIGH);
  pinMode(WR_L, INPUT);

  pinMode(RD_L, OUTPUT);
  digitalWrite(RD_L, LOW);

  // Note: NOT toggling the RAS and RD signals per each memory location as we do
  // for VRAM/SRAM and DRAM - this may change, for now it seems to be working
  for (uint32_t i = 0; i <= totalBytes; ++i)
  {
    _model1->setAddressLinesToOutput(i); // Update address for each byte
    asmWait(5);

    // Read a byte and process
    uint8_t data = PINF;

    crc.update(data);
  }

  // turn off RAS*, RD*, WR* lines
  _model1->turnOffReadWriteRASLines();

  return crc.finalize();
}
