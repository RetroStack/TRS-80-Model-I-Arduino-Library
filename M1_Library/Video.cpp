// Supports video operations, lowercase. SRAM/VRAM operations are used for ROM as well (for now)

#include "./Video.h"
#include "./Model1.h"

Video::Video(ILogger *logger, Model1 *model)
{
  model1 = model;
  _logger = logger;
}

// clear screen
void Video::cls()
{
  _cursorPosition = VIDEO_MEM_START;
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
  model1->setAddressLinesToOutput(VIDEO_MEM_START);
  model1->setDataLinesToOutput();

  // Fill VRAM
  for (uint16_t i = start; i <= end; i++)
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

// Prints to video memory using address
void Video::printToScreen(const char *str, uint16_t startAddress)
{
  if (startAddress < VIDEO_MEM_START || startAddress > (VIDEO_MEM_START + VIDEO_MEM_SIZE))
  {
    _logger->info("Invalid range. Must be between %04X and %04X.", VIDEO_MEM_START, (VIDEO_MEM_START + VIDEO_MEM_SIZE));
    return;
  }

  // setup address and data lines for write
  model1->setAddressLinesToOutput(VIDEO_MEM_START);
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
uint8_t *Video::readVRAM()
{
  _logger->info("Reading VRAM...");

  uint8_t videoData[VIDEO_MEM_SIZE];

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

  return videoData;
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
  int curPos = _cursorPosition - VIDEO_MEM_START;
  int x = curPos % VIDEO_COLS;
  int y = curPos / VIDEO_COLS;

  printToScreen(str, x, y, true);
}

// Print to the TRS-80 screen at specific coordinates. Supports carriage returns and auto line feeds,
// along with placement of characters without impacting last written location
void Video::printToScreen(const char *str, uint8_t x, uint8_t y, bool updateCursorPosition)
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
    _cursorPosition = memdAddress;
  }

  model1->turnOffReadWriteRASLines();
  model1->setAddressLinesToInput();
  model1->setDataLinesToInput();
}
