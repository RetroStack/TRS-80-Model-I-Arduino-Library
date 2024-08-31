// Model1.h

#ifndef MODEL1_H
#define MODEL1_H

#include <Arduino.h>
#include "Common.h"
#include "Keyboard.h"
#include "ROM.h"
#include "Utils.h"
#include "Video.h"


class Model1 {
  public:
    Model1();
    void init();

    void displayCtrlPinStatus(bool silent=true);
    void initControlPins(bool silent=true);
    void setAddressLinesToInput();
    void setAddressLinesToOutput(uint16_t memAddress = 0x3C00);
    void setAllPinsPortsToInput(bool silent=true);
    void setDataLinesToInput();
    void setDataLinesToOutput();
    void setTESTPin(int state, bool silent=true);
    void turnOffReadWriteRASLines();
    
    Video    video;
    Keyboard keyboard;
    ROM      rom;

  private:
  protected:
};



#endif // MODEL1_H
