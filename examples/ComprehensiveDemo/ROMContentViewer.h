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

public:
    ROMContentViewer();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

protected:
    void _executeOnce() override;

private:
    void displayROMContent();
    uint16_t getLinesPerPage() const;
};

#endif
