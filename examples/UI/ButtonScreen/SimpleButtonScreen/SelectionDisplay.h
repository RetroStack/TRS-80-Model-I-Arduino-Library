#ifndef SELECTION_DISPLAY_H
#define SELECTION_DISPLAY_H

#include <ConsoleScreen.h>

class SelectionDisplay : public ConsoleScreen
{
private:
    int _selectedOption;

public:
    SelectionDisplay(int option);

protected:
    void _executeOnce() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif /* SELECTION_DISPLAY_H */
