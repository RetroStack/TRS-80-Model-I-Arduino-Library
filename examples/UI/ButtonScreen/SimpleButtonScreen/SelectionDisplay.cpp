#include "SelectionDisplay.h"
#include "SimpleButton.h"

SelectionDisplay::SelectionDisplay(int option) : ConsoleScreen(), _selectedOption(option)
{
    setTitleF(F("Selection Result"));
}

void SelectionDisplay::_executeOnce()
{
    cls();
    println(F("You selected:"));
    println();

    switch (_selectedOption)
    {
    case 0:
        println(F("Audio Settings"));
        break;
    case 1:
        println(F("Video Settings"));
        break;
    case 2:
        println(F("System Info"));
        break;
    case 3:
        println(F("Debug Mode"));
        break;
    case 4:
        println(F("Network Config"));
        break;
    case 5:
        println(F("Reset Settings"));
        break;
    default:
        println(F("Unknown Option"));
        break;
    }

    println();
    println(F("Press MENU to return"));
}

Screen *SelectionDisplay::actionTaken(ActionTaken action, int8_t offsetX, int8_t offsetY)
{
    if (action & BUTTON_MENU)
    {
        return new SimpleButton(); // Return to the main SimpleButton screen
    }
    return ConsoleScreen::actionTaken(action, offsetX, offsetY);
}
