#ifndef SIMPLE_BUTTON_H
#define SIMPLE_BUTTON_H

#include <ButtonScreen.h>

class SimpleButton : public ButtonScreen
{
public:
    SimpleButton();

protected:
    void _drawContent() override;
    Screen *_getSelectedButtonItemScreen(int index) override;
    const char *_getButtonItemConfigValue(uint8_t index) override;
    bool _isButtonItemEnabled(uint8_t index) const override;
};

#endif /* SIMPLE_BUTTON_H */
