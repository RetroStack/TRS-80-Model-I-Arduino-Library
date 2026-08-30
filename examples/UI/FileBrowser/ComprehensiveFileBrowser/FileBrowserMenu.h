/*
 * FileBrowserMenu.h - Screens for the ComprehensiveFileBrowser example
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef FILE_BROWSER_MENU_H
#define FILE_BROWSER_MENU_H

#include <M1Shield.h>
#include <FileBrowser.h>
#include <MenuScreen.h>

// Main menu for selecting different browser configurations
class FileBrowserMenu : public MenuScreen
{
public:
    FileBrowserMenu() : MenuScreen()
    {
        // Set up menu items
        const char *items[] = {
            "Browse Root Directory",
            "Browse Logs (Restricted)",
            "Browse Data (Pre-select)",
            "Browse with Custom Types",
            "Browse Documents Only"};
        setMenuItems(items, 5);

        setTitleF(F("FileBrowser Examples"));

        const char *buttons[] = {"Exit", "Select >"};
        setButtonItems(buttons, 2);
    }

protected:
    Screen *_getSelectedMenuItemScreen(int index) override
    {
        switch (index)
        {
        case 0:
            // Basic browsing from root
            return new FileBrowser();

        case 1:
            // Browse logs directory with root restriction
            // User cannot navigate above /logs directory
            return new FileBrowser("/logs", "", true);

        case 2:
            // Browse data directory and pre-select "config.txt"
            return new FileBrowser("/data", "config.txt");

        case 3:
        {
            // Custom file type configuration
            FileBrowser *browser = new FileBrowser("/");
            browser->clearTextExtensions();
            browser->addTextExtension("cfg");
            browser->addTextExtension("ini");
            browser->addTextExtension("conf");
            browser->addTextExtension("log");
            browser->addTextExtension("txt");
            return browser;
        }

        case 4:
        {
            // Documents-only browser (restricted to documents folder)
            FileBrowser *browser = new FileBrowser("/documents", "", true);
            browser->clearTextExtensions();
            browser->addTextExtension("txt");
            browser->addTextExtension("md");
            browser->addTextExtension("doc");
            return browser;
        }

        default:
            return nullptr;
        }
    }

    const char *_getMenuItemConfigValue(uint8_t index) override
    {
        switch (index)
        {
        case 0:
            return "Full Access";
        case 1:
            return "Restricted";
        case 2:
            return "Pre-select";
        case 3:
            return "Custom Types";
        case 4:
            return "Docs Only";
        default:
            return nullptr;
        }
    }
};

#endif /* FILE_BROWSER_MENU_H */
