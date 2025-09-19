/*
 * ComprehensiveFileBrowser.ino - Advanced SD Card File Browser Example
 *
 * This example demonstrates advanced FileBrowser features including:
 * - Directory restrictions (root directory limiting)
 * - Pre-selection of specific files
 * - Custom file type extensions
 * - Starting from specific directories
 * - Multiple browser configurations
 *
 * Features demonstrated:
 * - Root directory restriction
 * - Target file pre-selection and auto-scroll
 * - Custom text file extensions
 * - Directory-specific browsing
 * - Menu integration for multiple browsers
 *
 * Hardware: Arduino Mega 2560 + M1Shield + SD Card
 *
 * Authors: Marcel Erz (RetroStack)
 * License: MIT
 */

#include <M1Shield.h>
#include <FileBrowser.h>
#include <MenuScreen.h>

// Uncomment ONE of these display provider lines based on your hardware:
#include <Display_ILI9341.h>
Display_ILI9341 displayProvider;

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

void setup()
{
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("=== Comprehensive FileBrowser Example ===");

    // Initialize M1Shield
    M1Shield.begin(displayProvider);

    // Create and set the main menu
    M1Shield.setScreen(new FileBrowserMenu());

    Serial.println("FileBrowser menu loaded and ready!");
    Serial.println();
    Serial.println("Example Configurations:");
    Serial.println("1. Basic root browsing - Full SD card access");
    Serial.println("2. Restricted browsing - Limited to /logs directory");
    Serial.println("3. Pre-selection - Opens /data and highlights config.txt");
    Serial.println("4. Custom types - Additional file extensions for text viewer");
    Serial.println("5. Document focus - Only browse /documents with document types");
    Serial.println();
    Serial.println("Setup Required:");
    Serial.println("- Create /logs directory with some .log files");
    Serial.println("- Create /data directory with config.txt file");
    Serial.println("- Create /documents directory with .txt/.md files");
    Serial.println("- Add various file types to test extension handling");
}

void loop()
{
    // Handle all M1Shield operations
    M1Shield.loop();
}
