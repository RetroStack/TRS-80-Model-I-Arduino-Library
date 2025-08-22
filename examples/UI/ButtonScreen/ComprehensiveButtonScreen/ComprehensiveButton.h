#ifndef COMPREHENSIVE_BUTTON_H
#define COMPREHENSIVE_BUTTON_H

#include <ButtonScreen.h>

// Global settings structure
struct GlobalSettings
{
    bool audioEnabled = true;
    uint8_t brightness = 75;
    uint8_t volume = 50;
    bool wifiEnabled = false;
    uint8_t contrast = 100;
    bool debugMode = false;
};

extern GlobalSettings settings;

class ComprehensiveButton : public ButtonScreen
{
public:
    ComprehensiveButton();

protected:
    void _drawContent() override;
    Screen *_getSelectedButtonItemScreen(int index) override;
    const char *_getButtonItemConfigValue(uint8_t index) override;
    bool _isButtonItemEnabled(uint8_t index) const override;
};

#endif /* COMPREHENSIVE_BUTTON_H */
