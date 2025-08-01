#ifndef CASSETTEREMOTECONFIG_H
#define CASSETTEREMOTECONFIG_H

#include <MenuScreen.h>
#include <Cassette.h>

class CassetteRemoteConfig : public MenuScreen
{
private:
    Cassette _cassette;

public:
    CassetteRemoteConfig();

protected:
    Screen *_getSelectedMenuItemScreen(int index) override;

private:
    void activateRemote();
    void deactivateRemote();
};

#endif
