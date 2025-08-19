/*
 * SecondContent.h - Second ContentScreen for demonstrating screen switching
 *
 * Simple second screen to show navigation between ContentScreens
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#ifndef SECOND_CONTENT_H
#define SECOND_CONTENT_H

#include <ContentScreen.h>

class SecondContent : public ContentScreen
{
public:
    SecondContent();

    // Override required ContentScreen methods
    void _drawContent() override;
    void loop() override;
    Screen *actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY) override;
};

#endif // SECOND_CONTENT_H
