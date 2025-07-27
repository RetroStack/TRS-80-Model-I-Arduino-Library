#ifndef ROMCONTENTVIEWER_H
#define ROMCONTENTVIEWER_H

#include <ConsoleScreen.h>
#include <ROM.h>

class ROMContentViewer : public ConsoleScreen
{
private:
    ROM _rom;
    uint16_t _currentAddress;
    static const uint16_t BYTES_PER_LINE = 16;
    static const uint16_t LINES_PER_PAGE = 10;

public:
    ROMContentViewer();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void displayROMContent();
    void displayInstructions();
};

#endif
