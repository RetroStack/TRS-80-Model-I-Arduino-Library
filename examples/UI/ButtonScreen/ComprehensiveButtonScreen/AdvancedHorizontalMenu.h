#ifndef ADVANCED_HORIZONTAL_MENU_H
#define ADVANCED_HORIZONTAL_MENU_H

#include <ButtonScreen.h>

class AdvancedHorizontalMenu : public ButtonScreen
{
private:
    unsigned long _lastUpdate;
    uint8_t _animationFrame;

public:
    AdvancedHorizontalMenu();

protected:
    void _drawContent() override;
    Screen *_getSelectedButtonItemScreen(int index) override;
    const char *_getButtonItemConfigValue(uint8_t index) override;
    bool _isButtonItemEnabled(uint8_t index) const override;
    void loop() override;
};

#endif /* ADVANCED_HORIZONTAL_MENU_H */
