#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(int type) : ConsoleScreen(), _settingType(type)
{
    const char *titles[] = {"Audio", "Brightness", "Volume", "WiFi", "Contrast", "Debug"};
    if (type >= 0 && type < 6)
    {
        setTitle(titles[type]);
    }
}

void SettingsScreen::_executeOnce()
{
    cls();

    switch (_settingType)
    {
    case 0: // Audio
        println(F("Audio Settings"));
        println();
        print(F("Status: "));
        println(settings.audioEnabled ? F("ENABLED") : F("DISABLED"));
        println();
        println(F("UP/DOWN: Toggle"));
        break;

    case 1: // Brightness
        println(F("Brightness Control"));
        println();
        print(F("Level: "));
        print(settings.brightness);
        println(F("%"));
        println();
        println(F("LEFT/RIGHT: Adjust"));
        break;

    case 2: // Volume
        println(F("Volume Control"));
        println();
        if (!settings.audioEnabled)
        {
            println(F("Audio is disabled!"));
        }
        else
        {
            print(F("Level: "));
            print(settings.volume);
            println(F("%"));
            println();
            println(F("LEFT/RIGHT: Adjust"));
        }
        break;

    case 3: // WiFi
        println(F("Network Settings"));
        println();
        print(F("WiFi: "));
        println(settings.wifiEnabled ? F("CONNECTED") : F("DISABLED"));
        println();
        println(F("UP/DOWN: Toggle"));
        break;

    case 4: // Contrast
        println(F("Display Contrast"));
        println();
        print(F("Level: "));
        print(settings.contrast);
        println(F("%"));
        println();
        println(F("LEFT/RIGHT: Adjust"));
        break;

    case 5: // Debug
        println(F("Debug Mode"));
        println();
        print(F("Status: "));
        println(settings.debugMode ? F("ACTIVE") : F("INACTIVE"));
        println();
        println(F("UP/DOWN: Toggle"));
        break;
    }

    println();
    println(F("MENU: Return"));
}

Screen *SettingsScreen::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    bool needsRefresh = false;

    switch (_settingType)
    {
    case 0: // Audio toggle
        if (action & (BUTTON_UP | BUTTON_DOWN))
        {
            settings.audioEnabled = !settings.audioEnabled;
            needsRefresh = true;
        }
        break;

    case 1: // Brightness adjustment
        if (action & BUTTON_LEFT && settings.brightness > 0)
        {
            settings.brightness = max(0, settings.brightness - 5);
            needsRefresh = true;
        }
        if (action & BUTTON_RIGHT && settings.brightness < 100)
        {
            settings.brightness = min(100, settings.brightness + 5);
            needsRefresh = true;
        }
        break;

    case 2: // Volume adjustment
        if (settings.audioEnabled)
        {
            if (action & BUTTON_LEFT && settings.volume > 0)
            {
                settings.volume = max(0, settings.volume - 5);
                needsRefresh = true;
            }
            if (action & BUTTON_RIGHT && settings.volume < 100)
            {
                settings.volume = min(100, settings.volume + 5);
                needsRefresh = true;
            }
        }
        break;

    case 3: // WiFi toggle
        if (action & (BUTTON_UP | BUTTON_DOWN))
        {
            settings.wifiEnabled = !settings.wifiEnabled;
            needsRefresh = true;
        }
        break;

    case 4: // Contrast adjustment
        if (action & BUTTON_LEFT && settings.contrast > 0)
        {
            settings.contrast = max(0, settings.contrast - 5);
            needsRefresh = true;
        }
        if (action & BUTTON_RIGHT && settings.contrast < 100)
        {
            settings.contrast = min(100, settings.contrast + 5);
            needsRefresh = true;
        }
        break;

    case 5: // Debug toggle
        if (action & (BUTTON_UP | BUTTON_DOWN))
        {
            settings.debugMode = !settings.debugMode;
            needsRefresh = true;
        }
        break;
    }

    if (needsRefresh)
    {
        _executeOnce(); // Refresh display
        return nullptr; // Stay on this screen
    }

    // Handle exit
    if (action & BUTTON_MENU)
    {
        return nullptr; // Return to previous screen
    }

    return ConsoleScreen::actionTaken(action, offsetX, offsetY);
}
