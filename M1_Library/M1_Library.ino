// RetroStack Model 1 Library Examples

#include <Arduino.h>
#include "./Model1.h"
#include "./SerialLogger.h"

Model1 *m1;
SerialLogger *logger;

const float VERSION = 0.91;

void setup()
{
  Serial.begin(500000); // baud rate
  Serial.println("\n");

  logger = new SerialLogger();
  logger->info("RETROSTACK M1 LIBRARY - V%.2f", VERSION);

  m1 = new Model1(logger);

  logger->info("Library setup complete.");

  // Demonstration of basic function
  examples();
}

void loop()
{
  // Your main code
}

// Examples of the major functions. Enable the if blocks as needed.
void examples()
{
  // Get control of the system
  m1->setTESTPin(LOW);

  /*---------- Shield Pin statuses ---------*/
  if (0)
  {
    m1->displayCtrlPinStatus();
  }

  Keyboard *keyboard = m1->getKeyboard();
  ROM *rom = m1->getROM();
  Video *video = m1->getVideo();

  /*---------- VIDEO ----------*/
  video->cls();
  video->printToScreen("RETROSHACK M1 LIBRARY", 16, 0);
  video->printToScreen("\n\nENABLE THE 'IF()' BLOCKS IN THE ARDUINO CODE TO RUN EXAMPLES");

  if (0)
  {
    // TIP: comment out all the features first and then uncomment the feature you wish to review

    // Fill with a pattern
    video->fillVRAMwithPattern("TRS-80", 0x3D00, 0x3D3F);

    // TODO: this is need as as the display ends up in the 32 character mode
    // not sure if it is due to contention with the video refresh circuitry.
    // And doesn't resolve issue 100% time, might be the board issue.
    video->cls();

    // fill screen (full or partial)
    video->fillVRAM('-', 0x3DC0, 0x3DFF);

    // write a byte
    uint8_t writeByte = '*';
    m1->setAddressLinesToOutput();           // do this once
    m1->setDataLinesToOutput();              // do this once
    video->writeByteVRAM(0x3CFF, writeByte); // do this per mem address

    // read a byte
    m1->setAddressLinesToOutput();                  // do this once
    m1->setDataLinesToInput();                      // do this once
    uint8_t readByte = video->readByteVRAM(0x3CFF); // do this per mem address
    logger->info("Wrote and read back: %dh, %d", writeByte, (char)readByte);
  }

  /*---------- KEYBOARD ----------*/
  if (0)
  {
    while (1)
    {
      uint8_t key = keyboard->scan();
      if (key != 0)
        logger->info("Key: %2X, %c", key, (char)key);
    }
  }

  /*---------- ROM functions ----------*/
  if (0)
  {
    // get ROM checksum
    uint32_t checksum = rom->getChecksum();
    logger->info("ROM Checksum: %08lX", checksum);
  }

  /*---------- DRAM ----------*/
  // TODO

  // At this point reset you can release the bus by setting address, data and control lines
  // to INPUTs and drive TEST* high
  // m1->setAddressLinesToInput();
  // m1->setDataLinesToInput();
  // m1->setAllPinsPortsToInput();
  // m1->setTESTPin(1);
}
