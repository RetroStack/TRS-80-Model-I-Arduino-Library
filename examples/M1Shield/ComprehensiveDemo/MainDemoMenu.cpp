#include "MainDemoMenu.h"
#include "TerminalDemo.h"
#include "ROMFeaturesMenu.h"
#include "VideoFeaturesMenu.h"
#include "RAMTestSuite.h"
#include "KeyboardTester.h"
#include "CassetteSongPlayer.h"
#include <Model1.h>
#include <Arduino.h>

MainDemoMenu::MainDemoMenu() : MenuScreen()
{
    _setTitle("M1Shield Demo");

    // Initialize Model1 hardware
    Model1.begin();
    _testSignalActive = false;

    // Add configuration item for TEST signal
    addConfigItem("TEST Signal", "OFF");

    // Add main demo menu items
    addMenuItem("Terminal Demo");
    addMenuItem("ROM Features");
    addMenuItem("Video Features");
    addMenuItem("RAM Testing");
    addMenuItem("Keyboard Test");
    addMenuItem("Song Player");

    Serial.println("Main Demo Menu initialized");
}

void MainDemoMenu::updateTestSignalDisplay()
{
    updateConfigItem(0, _testSignalActive ? "ON" : "OFF");
}

void MainDemoMenu::toggleTestSignal()
{
    if (_testSignalActive)
    {
        Model1.deactivateTestSignal();
        _testSignalActive = false;
        Serial.println("TEST signal deactivated");
    }
    else
    {
        Model1.activateTestSignal();
        _testSignalActive = true;
        Serial.println("TEST signal activated");
    }
    updateTestSignalDisplay();
}

Screen *MainDemoMenu::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_JOYSTICK)
    {
        int selectedIndex = getSelectedIndex();

        // Handle config item (TEST signal toggle)
        if (selectedIndex == 0)
        {
            toggleTestSignal();
            return nullptr; // Stay on this screen
        }

        // Handle menu items (adjust for config item offset)
        int menuIndex = selectedIndex - 1;

        switch (menuIndex)
        {
        case 0: // Terminal Demo
            Serial.println("Opening Terminal Demo");
            return new TerminalDemo();

        case 1: // ROM Features
            Serial.println("Opening ROM Features Menu");
            return new ROMFeaturesMenu();

        case 2: // Video Features
            Serial.println("Opening Video Features Menu");
            return new VideoFeaturesMenu();

        case 3: // RAM Testing
            Serial.println("Opening RAM Test Suite");
            return new RAMTestSuite();

        case 4: // Keyboard Test
            Serial.println("Opening Keyboard Tester");
            return new KeyboardTester();

        case 5: // Song Player
            Serial.println("Opening Cassette Song Player");
            return new CassetteSongPlayer();

        default:
            return nullptr;
        }
    }

    // Let parent handle other actions (navigation, back button)
    return MenuScreen::actionTaken(action, offsetX, offsetY);
}
