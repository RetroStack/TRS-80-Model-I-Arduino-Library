#ifndef VIDEOFEATURESMENU_H
#define VIDEOFEATURESMENU_H

#include <MenuScreen.h>

class VideoFeaturesMenu : public MenuScreen
{
public:
    VideoFeaturesMenu();
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;
};

#endif
