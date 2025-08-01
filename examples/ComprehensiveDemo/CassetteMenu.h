#ifndef CASSETTEMENU_H
#define CASSETTEMENU_H

#include <MenuScreen.h>
#include <Cassette.h>

class CassetteMenu : public MenuScreen
{
private:
    Cassette _cassette;
    bool _remoteActive;
    bool _charGenA;     // Track current character generator (true = A, false = B)
    bool _is64CharMode; // Track current character mode state

public:
    CassetteMenu();
    bool open() override;
    void close() override;

protected:
    Screen *_getSelectedMenuItemScreen(int index) override;
    const char *_getMenuItemConfigValue(uint8_t index) override;

private:
    void toggleRemote();
    void toggleCharacterMode();
    void toggleCharacterGen();
};

#endif
