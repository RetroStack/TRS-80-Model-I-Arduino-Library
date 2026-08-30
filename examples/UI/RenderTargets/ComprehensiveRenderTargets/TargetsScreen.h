/*
 * TargetsScreen.h - Lists the registered render targets and toggles them
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef TARGETS_SCREEN_H
#define TARGETS_SCREEN_H

#include <ContentScreen.h>

class TargetsScreen : public ContentScreen
{
private:
    uint8_t _selected; // Index of the highlighted render target

public:
    TargetsScreen();

protected:
    void _drawContent() override;

public:
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif /* TARGETS_SCREEN_H */
