#ifndef MAINDEMOMENU_H
#define MAINDEMOMENU_H

#include <MenuScreen.h>

class MainDemoMenu : public MenuScreen
{
private:
    bool _testSignalActive;

public:
    MainDemoMenu();
    void updateTestSignalDisplay();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void toggleTestSignal();
};

#endif
