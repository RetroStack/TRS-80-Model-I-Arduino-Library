/*
  TODO:
    - need to test with numeric keypad
*/

#include "./Keyboard.h"
#include "./Model1.h"

#define KEYBOARD_MEM_ADDRESS 0x3800
#define KEYBOARD_SHIFT_KEY 0x3880

// Look-up table for uppercase characters
const uint8_t lookupTable[8][8] = {
    {'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G'},         // 3801
    {'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'},         // 3802
    {'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W'},         // 3804
    {'X', 'Y', 'Z', 0, 0, 0, 0, 0},                   // 3808
    {'0', '1', '2', '3', '4', '5', '6', '7'},         // 3810
    {'8', '9', ':', ';', ',', '-', '.', '/'},         // 3820
    {0x0D, 0x1F, 0x01, 0x5B, 0x0A, 0x08, 0x09, 0x20}, // 3840
};

Keyboard::Keyboard(ILogger *logger, Model1 *model1)
{
  _model1 = model1;
  _logger = logger;
}

// Decodes key from keyboard, supports shift key.
// Does not support multiple keys being held down.
uint8_t Keyboard::scan()
{
  Video *video = _model1->getVideo();

  _model1->setAddressLinesToOutput(KEYBOARD_MEM_ADDRESS);
  _model1->setDataLinesToInput();

  // Check if shift is pressed
  bool shift = video->readByteVRAM(KEYBOARD_SHIFT_KEY) & 1;

  // Roll thru the mapped keyboard addresses
  for (int i = 0; i < 7; i++)
  {
    uint16_t keyMemAddress = KEYBOARD_MEM_ADDRESS + (1 << i);
    uint8_t keyCode = video->readByteVRAM(keyMemAddress);

    // Roll thru the bit positions
    for (int bit = 0; bit < 8; bit++)
    {
      // See if any other keys are being held down and apply the shift
      // Key codes mapped to KEYBOARD LOOKUP TABLE at this link:
      // https://www.trs-80.com/wordpress/roms/mod-1-rom-disassembled-page-1/
      if (keyCode & (1 << bit))
      {
        asmWait(65535, 25); // prevent key bouncing
        uint8_t key = lookupTable[i][bit];
        if (shift)
        {
          if (key >= 'A' && key <= 'Z')
          {
            key += 32;
          }
          else
          {
            if (key == 0x5B)
            {
              key = 0x1B;
            } // up arrow
            else if (key == 0x0A)
            {
              key = 0x1A;
            } // down arrow
            else if (key == 0x08)
            {
              key = 0x18;
            } // left arrow
            else if (key == 0x09)
            {
              key = 0x19;
            } // right arrow
          }
        }
        return key;
      }
    }
  }
  return 0;
}
