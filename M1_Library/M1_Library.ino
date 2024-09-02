// RetroStack Model 1 Test Harness Library Examples

#include <Arduino.h>
#include "Model1.h"
#include "Utils.h"

Model1 m1;

char splashMsg[] = "RETROSTACK M1 TEST HARNESS LIBRARY - V";
float version = 0.91; // Version

void setup()
{

  // Initialize serial communication at 500000 baud rate
  Serial.begin(500000);
  Serial.println("\n");
  Serial.print(splashMsg);
  Serial.println(version, 2); // Print the version with 3 decimal places
  printLine(F("Program setup complete."));

  // run examples
  examples();
}

void loop()
{
  // Your main code
}

// Example of the major functions, set silent=false if you want verbosity,
// enable the if blocks as needed.
void examples()
{

  /* Setup the Model1 first and go immediately into TEST mode*/
  m1.init();

  /*---------- Shield PIN statuses ---------*/
  if (0)
  {
    m1.displayCtrlPinStatus(false);
  }

  /*---------- VIDEO ----------*/
  m1.video.cls();
  m1.video.printToScreen("RETROSHACK TEST HARNESS LIBRARY", 16, 0);
  m1.video.printToScreen("\n\nENABLE THE 'IF()' BLOCKS IN THE ARDUINO CODE TO RUN EXAMPLES");

  if (0)
  {
    // TIP: comment out all the features first and then uncomment the feature you wish to review

    // Fill with a pattern
    m1.video.fillVRAMwithPattern(false, "TRS-80", 0x3D00, 0x3D3F);

    // lowercase mod check
    printLine("Lowercase mod detected: ", (m1.video.lowercaseModExists(true) ? "Yes" : "No"));

    // TODO: this is need as as the display ends up in the 32 character mode
    // not sure if it is due to contention with the video refresh circuitry.
    // And doesn't resolve issue 100% time, might be the board issue.
    m1.video.cls();

    // display font set
    m1.video.displayCharacterSet();

    // read VRAM and sent it to serial port
    m1.video.readVRAM(true, true, true);  // show values in hex
    m1.video.readVRAM(true, false, true); // show values in ASCII

    // fill screen (full or partial)
    m1.video.fillVRAM(true, '-', 0x3DC0, 0x3DFF);

    // write a byte
    uint8_t writeByte = '*';
    m1.setAddressLinesToOutput();              // do this once
    m1.setDataLinesToOutput();                 // do this once
    m1.video.writeByteVRAM(0x3CFF, writeByte); // do this per mem address

    // read a byte
    m1.setAddressLinesToOutput();                     // do this once
    m1.setDataLinesToInput();                         // do this once
    uint8_t readByte = m1.video.readByteVRAM(0x3CFF); // do this per mem address
    printLine("Wrote and read back: ", writeByte, "h, ", (char)readByte);
  }

  /*---------- KEYBOARD ----------*/
  if (0)
  {
    while (1)
    {
      uint8_t key = m1.keyboard.scanKeyboard();
      if (key != 0)
      {
        sprintf(stringBuffer, "Key: %2X, %c", key, (char)key);
        printLine(stringBuffer);
        // Serial.println((char)key);
      }
    }
  }

  /*---------- ROM functions ----------*/
  if (0)
  {
    // get ROM checksum
    uint32_t checksum = m1.rom.getROMChecksum(false);
    sprintf(stringBuffer, "ROM Checksum: %08lX", (unsigned long)checksum);
    printLine(stringBuffer);

    // Is the ROM checksum a known valid value
    m1.rom.isROMValid(checksum, true);
    printLine("ROM version ", m1.rom.romVersion, ", is it valid? ", (m1.rom.isValid ? "Y" : "N"));

    // dump ROM to serial port
    m1.rom.dump();
  }

  /*---------- DRAM ----------*/
  // TODO

  // At this point reset you can release the bus by setting address, data and control lines
  // to INPUTs and drive TEST* high
  // m1.setAddressLinesToInput();
  // m1.setDataLinesToInput();
  // m1.setAllPinsPortsToInput();
  // m1.setTESTPin(1);
}
