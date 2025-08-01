#ifndef VIDEOFEATURESMENU_H
#define VIDEOFEATURESMENU_H

#include <MenuScreen.h>

class VideoFeaturesMenu : public MenuScreen
{
public:
    VideoFeaturesMenu();

protected:
    Screen *_getSelectedMenuItemScreen(int index) override;
};

#endif
