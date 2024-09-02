// Keyboard.h

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include "Common.h"
#include "Utils.h"

#define KEYBOARD_MEM_ADDRESS 0x3800
#define KEYBOARD_SHIFT_KEY   0x3880

class Model1; // Forward declaration

class Keyboard {
  public:
    Keyboard(Model1* model);
    
    Keyboard();
    uint8_t scanKeyboard();

  private:
    Model1* model1;
};

#endif // KEYBOARD_H
