// Video.cpp
// Supports video operations, lowercase. SRAM/VRAM operations are used for ROM as well (for now)

#include "Video.h"
#include "./Model1.h"

Video::Video(ILogger *logger, Model1 *model)
{
  model1 = model;
  _logger = logger;

  _logger->info("Video constructor done.");
}

// uint8_t videoData[VIDEO_MEM_SIZE];
// uint8_t memPattern[10];
// unsigned int cursorPosition = VIDEO_MEM_START;
// CRC32 crc;

uint32_t vramChecksumTable[6] = {
    0xEFB5AF2E, // 0x00
    0x24D7C38D, // 0x20
    0xAB58F48B, // 0x6F
    0x235141FA, // 0x7F
    0xF58F20F3, // 0xBF
    0xB83AFFF4  // 0xFF
};

void Video::init()
{
  // Initialize static members if needed
}

// clear screen
void Video::cls()
{
  cursorPosition = VIDEO_MEM_START;
  fillVRAM(); // defaults to filling video memory with blanks
}

// Fill VRAM with a pattern
void Video::fillVRAMwithPattern(const char *pattern, uint16_t start, uint16_t end)
{
  int patternSize = strlen(pattern);

  _logger->info("Pattern size = %d", patternSize);
  _logger->info("pattern = %s", pattern);

  // make sure other control lines are set correct
  pinMode(RD_L, INPUT);
  model1->setAddressLinesToOutput(0x3c00);
  model1->setDataLinesToOutput();

  // Fill VRAM
  for (uint16_t i = 0; i <= (end - start); i++)
  {
    uint16_t memAddress = i + start;
    // Split address
    uint8_t lowerByte = (uint8_t)(memAddress & 0xFF); // Masking to get the lower byte
    uint8_t upperByte = (uint8_t)(memAddress >> 8);   // Shifting to get the upper byte

    PORTA = lowerByte; // Write the lower byte of the address to PORTA
    PORTC = upperByte; // Write the upper byte of the address to PORTC

    // RAS*
    pinMode(RAS_L, OUTPUT);
    digitalWrite(RAS_L, LOW);
    asmWait(1);

    // fill VRAM with fillValue
    PORTF = pattern[i % patternSize];

    _logger->info("Mem: %04X, Index: %i, Char: %c", i + VIDEO_MEM_START, i % patternSize, (char)pattern[i % patternSize]);

    // WR*
    pinMode(WR_L, OUTPUT);
    digitalWrite(WR_L, LOW);
    asmWait(3);

    // Exit (keep this order)
    model1->turnOffReadWriteRASLines();
  }

  // prep pins for exit
  model1->turnOffReadWriteRASLines();
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
  asmWait(3);
}

/*
  Write Cycle Waveform
    Address
    Chip Enable
    Data
    Read/Write
*/
void Video::fillVRAM(uint8_t fillValue, uint16_t start, uint16_t end)
{
  _logger->info("Fill: start=%4X, end=%4X, value=%02X", start, end, fillValue);

  pinMode(RD_L, INPUT); // JIC
  model1->setAddressLinesToOutput(VIDEO_MEM_START);
  model1->setDataLinesToOutput();

  // Fill VRAM
  for (uint16_t i = start; i <= end; i++)
  {
    // Split address
    uint8_t lowerByte = (uint8_t)(i & 0xFF); // Masking to get the lower byte
    uint8_t upperByte = (uint8_t)(i >> 8);   // Shifting to get the upper byte

    PORTA = lowerByte; // Write the lower byte of the address to PORTA
    PORTC = upperByte; // Write the upper byte of the address to PORTC

    // RAS*
    pinMode(RAS_L, OUTPUT);
    digitalWrite(RAS_L, LOW);
    asmWait(1);

    // fill VRAM with fillValue
    PORTF = fillValue;

    // WR*
    pinMode(WR_L, OUTPUT);
    digitalWrite(WR_L, LOW);
    asmWait(3);

    // Exit
    pinMode(WR_L, INPUT);
    pinMode(RAS_L, INPUT);
    asmWait(3);
  }

  // Exit
  model1->turnOffReadWriteRASLines();
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
  asmWait(3);
}

// Show character to display
void Video::displayCharacterSet()
{
  cls();
  printToScreen("ROM CHARACTER FONT DUMP", VIDEO_MEM_START + 20);

  // set control lines
  pinMode(RD_L, INPUT); // prob OK not to force it HIGH
  model1->setAddressLinesToOutput(0x3c00);
  model1->setDataLinesToOutput();

  // Fill VRAM
  for (int i = 0; i < 255; i++)
  {
    uint16_t address = i + 0x3C80;

    // Split address
    uint8_t lowerByte = (uint8_t)(address & 0xFF); // Masking to get the lower byte
    uint8_t upperByte = (uint8_t)(address >> 8);   // Shifting to get the upper byte

    PORTA = lowerByte; // Write the lower byte of the address to PORTA
    PORTC = upperByte; // Write the upper byte of the address to PORTC

    // RAS*
    pinMode(RAS_L, OUTPUT);
    digitalWrite(RAS_L, LOW);
    asmWait(1);

    // fill VRAM with fillValue
    PORTF = (uint8_t)i;

    // WR*
    pinMode(WR_L, OUTPUT);
    digitalWrite(WR_L, LOW);
    asmWait(3);

    // Exit write
    pinMode(WR_L, INPUT);
    pinMode(RAS_L, INPUT);
    asmWait(3);
  }

  // prep pins for exit
  model1->turnOffReadWriteRASLines();
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
  asmWait(3);
}

// Prints to video memory using address
void Video::printToScreen(const char *str, uint16_t startAddress)
{
  if (startAddress < VIDEO_MEM_START || startAddress > (VIDEO_MEM_START + VIDEO_MEM_SIZE))
  {
    _logger->info("Invalid range. Must be between %04X and %04X.", VIDEO_MEM_START, (VIDEO_MEM_START + VIDEO_MEM_SIZE));
    return;
  }

  // setup address and data lines for write
  model1->setAddressLinesToOutput(0x3C00);
  model1->setDataLinesToOutput();

  // go thru string and write to display memory
  uint16_t bufferIndex = startAddress;
  for (const char *p = str; *p != '\0'; p++)
  {
    if (bufferIndex >= (VIDEO_MEM_START + VIDEO_MEM_SIZE))
    {
      _logger->info("Buffer overflow.");
      break;
    }

    if (*p == '\r')
    { // Carriage return handling
      bufferIndex += VIDEO_COLS - (bufferIndex % VIDEO_COLS);
      if (bufferIndex >= (VIDEO_MEM_START + VIDEO_MEM_SIZE))
      {
        _logger->info("Buffer overflow.");
        break;
      }
      continue;
    }

    // Split address
    uint16_t memAddress = bufferIndex;
    uint8_t lowerByte = (uint8_t)(memAddress & 0xFF); // Masking to get the lower byte
    uint8_t upperByte = (uint8_t)(memAddress >> 8);   // Shifting to get the upper byte

    PORTA = lowerByte; // Write the lower byte of the address to PORTA
    PORTC = upperByte; // Write the upper byte of the address to PORTC

    // RAS*
    pinMode(RAS_L, OUTPUT);
    digitalWrite(RAS_L, LOW);
    asmWait(1);

    // write the byte
    PORTF = *p;
    // Serial.print(memAddress, (HEX));
    // Serial.print("-");
    // Serial.println(*p, (HEX));

    // WR*
    pinMode(WR_L, OUTPUT);
    digitalWrite(WR_L, LOW);
    asmWait(3);

    // Exit write
    pinMode(WR_L, INPUT);
    pinMode(RAS_L, INPUT);
    asmWait(3);

    bufferIndex++;
  }

  // prep pins for exit
  pinMode(RD_L, INPUT);
  pinMode(WR_L, INPUT);
  pinMode(RAS_L, INPUT);

  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
  asmWait(3);
}

/*
  Read Cycle Waveform
    Address
    Chip Enable
    Data
    Read
*/
uint32_t Video::readVRAM(bool showInHex, bool dumpVRAM)
{
  uint32_t checksum = 0;

  _logger->info("Reading VRAM...");

  // Setup to write VRAM
  model1->setAddressLinesToOutput(VIDEO_MEM_START);
  model1->setDataLinesToInput();

  // this seems to be needed otherwise a random character gets written to the VRAM
  // at the start of the address in the loop below.
  pinMode(WR_L, OUTPUT);
  digitalWrite(WR_L, HIGH);
  pinMode(WR_L, INPUT);

  // loop thru and read
  for (int i = 0; i < VIDEO_MEM_SIZE; ++i)
  {
    // Split address
    uint16_t memAddress = VIDEO_MEM_START + i;
    uint8_t lowerByte = (uint8_t)(memAddress & 0xFF); // Masking to get the lower byte
    uint8_t upperByte = (uint8_t)(memAddress >> 8);   // Shifting to get the upper byte

    PORTA = lowerByte; // Write the lower byte of the address to PORTA
    PORTC = upperByte; // Write the upper byte of the address to PORTC

    // RAS*
    pinMode(RAS_L, OUTPUT);
    digitalWrite(RAS_L, LOW);
    asmWait(2);

    // RD*
    pinMode(RD_L, OUTPUT);
    digitalWrite(RD_L, LOW);
    asmWait(3);

    // fill VRAM with fillValue
    videoData[i] = PINF;

    // Exit read
    asmWait(3);
  }

  // release the control, data and address lines by setting to INPUT
  model1->turnOffReadWriteRASLines();
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
  asmWait(3);

  // calcuate CRC
  crc.reset();
  crc.update(videoData, VIDEO_MEM_SIZE);
  checksum = crc.finalize();

  if (dumpVRAM)
  {
    _logger->info("--- Video buffer dump start ---");

    for (int i = 0; i < VIDEO_MEM_SIZE; ++i)
    {
      if (i % 64 == 0)
      { // Check if 64 characters have been printed
        _logger->info("%04X", 0x3C00 + i);
      }

      if (showInHex)
      {
        _logger->info("%02X", videoData[i]);
      }
      else
      {
        _logger->info("%c", videoData[i]);
      }
      if ((i + 1) % 64 == 0)
      {                    // Check if 64 characters have been printed
        _logger->info(""); // Print newline character
      }
    }
    _logger->info("--- Video buffer dump end ---");
    _logger->info("Checksum: %08X", checksum);
  }

  return checksum;
}

bool Video::checkVRAMFillChecksum(uint8_t fillValues[], int fillValuesCount)
{
  bool retVal = true;
  for (int i = 0; i < fillValuesCount; i++)
  {
    bool valid = false;
    uint32_t checksum = 0;
    // Serial.print("Filling VRAM with: ");
    // Serial.println(fillValues[i], HEX);
    fillVRAM(true, fillValues[i]);
    asmWait(65535, 50);
    checksum = readVRAM(false, false);
    valid = compareVRAMChecksum(checksum);
    if (!valid)
    {
      _logger->info("VRAM checksum did not match.");
      retVal = false;
      break;
    }
    else
    {
      _logger->info("VRAM checksum matched.");
    }
  }

  return retVal;
}

// Function to compare a passed checksum against the checksum table
bool Video::compareVRAMChecksum(uint32_t checksum)
{
  int checksumCounts = sizeof(vramChecksumTable) / sizeof(vramChecksumTable[0]);
  for (int i = 0; i < checksumCounts; i++)
  {
    if (vramChecksumTable[i] == checksum)
    {
      return true;
    }
  }
  return false;
}

/*
  Read byte from VRAM
  - Arduino address lines should be set for output
  - Arduino data lines should be set for input
*/
uint8_t Video::readByteVRAM(uint16_t memAddress)
{
  uint8_t data = 0;

  uint8_t lowerByte = (uint8_t)(memAddress & 0xFF); // Masking to get the lower byte
  uint8_t upperByte = (uint8_t)(memAddress >> 8);   // Shifting to get the upper byte

  PORTA = lowerByte; // Write the lower byte of the address to PORTA
  PORTC = upperByte; // Write the upper byte of the address to PORTC

  // RAS*
  pinMode(RAS_L, OUTPUT);
  digitalWrite(RAS_L, LOW);
  asmWait(1);

  // RD*
  pinMode(RD_L, OUTPUT);
  digitalWrite(RD_L, LOW);
  asmWait(3);

  // get data
  data = PINF;

  // prep for exit
  model1->turnOffReadWriteRASLines();

  return data;
}

/*
  Write byte to VRAM
  - Arduino address lines should be set for output
  - Arduino data lines should be set for output
*/
void Video::writeByteVRAM(uint16_t memAddress, uint8_t data)
{

  uint8_t lowerByte = (uint8_t)(memAddress & 0xFF); // Masking to get the lower byte
  uint8_t upperByte = (uint8_t)(memAddress >> 8);   // Shifting to get the upper byte

  PORTA = lowerByte; // Write the lower byte of the address to PORTA
  PORTC = upperByte; // Write the upper byte of the address to PORTC

  // RAS*
  pinMode(RAS_L, OUTPUT);
  digitalWrite(RAS_L, LOW);
  asmWait(1);

  // put data
  PORTF = data;

  // WR*
  pinMode(WR_L, OUTPUT);
  digitalWrite(WR_L, LOW);
  asmWait(3);

  // prep for exit
  model1->turnOffReadWriteRASLines();
}

// Checks if a lowercase mod exists in system, by first writing 20, then BF
// if no errors found then assumes that video memory is good. Then writes
// 6F, 7F, FF to video memory - if checksums pass then LC additional SRAM
// chip was installed and is working.
bool Video::lowercaseModExists()
{
  bool retVal = false;

  _logger->info("Checking for lowercase mod - requires additional SRAM chip and correct character ROM");

  uint8_t fillValues[] = {0x20, 0xBF};
  retVal = checkVRAMFillChecksum(fillValues, 2);

  if (!retVal)
  {
    _logger->info("Unable to clear VRAM, please run VRAM diagnostics to troubleshoot.");
    return retVal;
  }

  // at this point VRAM should be good, now check if bit 6 is on or off for these
  // If no lowercase mod: 6F -> 2F, 7F -> 3F, FF -> BF
  uint8_t fillValuesLC[] = {0x6F, 0x7F, 0xFF};

  retVal = checkVRAMFillChecksum(fillValuesLC, 3);

  return retVal;
}

// Move VRAM contents around in VRAM only
void Video::memmoveVRAM(unsigned int dest, unsigned int src, unsigned int n)
{
  model1->setAddressLinesToOutput(dest);
  if (dest < src)
  {
    for (unsigned int i = 0; i < n; i++)
    {
      model1->setDataLinesToInput();
      uint8_t data = readByteVRAM(src + i);
      model1->setDataLinesToOutput();
      writeByteVRAM(dest + i, data);
    }
  }
  else
  {
    for (int i = n - 1; i >= 0; i--)
    {
      model1->setDataLinesToInput();
      uint8_t data = readByteVRAM(src + i);
      model1->setDataLinesToOutput();
      writeByteVRAM(dest + i, data);
    }
  }
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
}

// Move screen contents up by 1 line
void Video::scrollScreenUp()
{
  memmoveVRAM(VIDEO_MEM_START, VIDEO_MEM_START + VIDEO_COLS, VIDEO_COLS * (VIDEO_ROWS - 1));
  fillVRAM(0x20, VIDEO_LAST_ROW, VIDEO_LAST_ROW + VIDEO_COLS);
}

// Print to screen from last cursor position
void Video::printToScreen(const char *str)
{
  int curPos = cursorPosition - VIDEO_MEM_START;
  int x = curPos % VIDEO_COLS;
  int y = curPos / VIDEO_COLS;

  printToScreen(str, x, y, true);
}

// Print to the TRS-80 screen at specific coordinates. Supports carriage returns and auto line feeds,
// along with placement of characters without impacting last written location
void Video::printToScreen(const char *str, unsigned int x, unsigned int y, bool updateCursorPosition)
{
  model1->setAddressLinesToOutput(VIDEO_MEM_START);
  model1->setDataLinesToOutput();

  if (y >= VIDEO_ROWS)
  {
    scrollScreenUp();
    y--;
    model1->setAddressLinesToOutput(VIDEO_MEM_START);
    model1->setDataLinesToOutput();
  }

  // pointer to memory
  unsigned int memdAddress = y * VIDEO_COLS + x + VIDEO_MEM_START;

  // process the string, requires string terminated with a '\0'
  while (*str != '\0')
  {
    if (*str == '\n')
    {
      y++;
      if (y >= VIDEO_ROWS)
      {
        scrollScreenUp();
        model1->setAddressLinesToOutput(memdAddress);
        model1->setDataLinesToOutput();
        y--;
      }
      x = 0;
      memdAddress = y * VIDEO_COLS + VIDEO_MEM_START;
    }
    else
    {
      if (x >= VIDEO_COLS)
      {
        x = 0;
        y++;
        if (y >= VIDEO_ROWS)
        {
          scrollScreenUp();
          model1->setAddressLinesToOutput(memdAddress);
          model1->setDataLinesToOutput();
          y--;
        }
        memdAddress = y * VIDEO_COLS + VIDEO_MEM_START;
      }
      writeByteVRAM(memdAddress, *str);
      // Serial.print(*str);
      x++;
      memdAddress++; // TODO: better as memdAddress = x * y?
    }
    str++;
  }

  // update the cursor positon or not
  if (updateCursorPosition)
  {
    cursorPosition = memdAddress;
  }

  model1->turnOffReadWriteRASLines();
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
}
