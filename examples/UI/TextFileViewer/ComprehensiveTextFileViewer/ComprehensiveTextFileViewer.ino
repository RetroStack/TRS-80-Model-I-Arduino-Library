#include <M1Shield.h>
#include <TextFileViewer.h>
#include <MenuScreen.h>

// ============================================
// Display Provider Configuration
// ============================================
// Uncomment ONE of the following display configurations

// ST7789 240x240 (Round displays)
// #include <Display_ST7789_240x240.h>
// Display_ST7789_240x240 displayProvider;

// ST7789 320x240 (Rectangular displays)
#include <Display_ST7789_320x240.h>
Display_ST7789_320x240 displayProvider;

// ST7735 128x160 (Small TFT displays)
// #include <Display_ST7735.h>
// Display_ST7735 displayProvider;

// ILI9341 240x320 (Common TFT displays)
// #include <Display_ILI9341.h>
// Display_ILI9341 displayProvider;

// HX8357 320x480 (Large TFT displays)
// #include <Display_HX8357.h>
// Display_HX8357 displayProvider;

// ============================================
// File Viewer Application
// ============================================
// Note: For dynamic file browsing and selection, see FileBrowser class
// which provides interactive directory navigation and file selection.

class FileViewerMenu : public MenuScreen
{
public:
    FileViewerMenu() : MenuScreen()
    {
        setTitleF(F("File Viewer"));

        // Menu items for different file types
        const char *menuItems[] = {
            "View README.TXT",
            "View CONFIG.TXT",
            "View DEBUG.LOG",
            "View SYSTEM.INI",
            "Exit"};

        setMenuItems(menuItems, 5);
    }

protected:
    Screen *_getSelectedMenuItemScreen(int index) override
    {
        switch (index)
        {
        case 0:
            return createFileViewer("readme.txt", F("README File"), false);
        case 1:
            return createFileViewer("config.txt", F("Configuration"), false);
        case 2:
            return createFileViewer("debug.log", F("Debug Log"), true); // Auto-paging for logs
        case 3:
            return createFileViewer("system.ini", F("System Settings"), false);
        case 4:
            return nullptr; // Exit
        }
        return nullptr;
    }

private:
    TextFileViewer *createFileViewer(const char *filename, const __FlashStringHelper *title, bool autoPaging)
    {
        TextFileViewer *viewer = new TextFileViewer(filename);

        // Configure the viewer
        viewer->setTitleF(title);
        viewer->setAutoPaging(autoPaging);

        // Different colors for different file types
        if (autoPaging)
        {
            viewer->setTextColor(M1Shield.convertColor(0x07E0)); // Green for logs
        }
        else
        {
            viewer->setTextColor(M1Shield.convertColor(0xFFFF)); // White for regular files
        }

        viewer->setTextSize(1); // Small text for more content

        return viewer;
    }
};

// Enhanced TextFileViewer with custom features
class EnhancedTextFileViewer : public TextFileViewer
{
private:
    bool showStatus;
    unsigned long lastRefresh;
    static const unsigned long REFRESH_INTERVAL = 2000; // 2 seconds

public:
    EnhancedTextFileViewer(const char *filename) : TextFileViewer(filename)
    {
        showStatus = true;
        lastRefresh = 0;
    }

    void setShowStatus(bool show)
    {
        showStatus = show;
    }

protected:
    void _drawContent() override
    {
        // Draw the file content
        TextFileViewer::_drawContent();

        // Add status information if enabled
        if (showStatus)
        {
            drawStatusInfo();
        }
    }

    void loop() override
    {
        TextFileViewer::loop();

        // Auto-refresh for log files
        if (isAutoPagingEnabled() && millis() - lastRefresh > REFRESH_INTERVAL)
        {
            if (refreshFile())
            {
                lastRefresh = millis();
            }
        }
    }

private:
    void drawStatusInfo()
    {
        Adafruit_GFX &gfx = M1Shield.getGFX();
        
        uint16_t y = _getContentTop() + _getContentHeight() - 20;
        uint16_t x = _getContentLeft() + 5;

        // Status background
        gfx.fillRect(x - 2, y - 2, 200, 16, M1Shield.convertColor(0x2104)); // Dark background

        // Status text
        String status = "Page " + String(getCurrentPage()) + "/" + String(getTotalPages());
        if (isAutoPagingEnabled())
        {
            status += " [AUTO]";
        }

        gfx.setTextColor(M1Shield.convertColor(0xFFE0)); // Yellow text
        gfx.setTextSize(1);
        gfx.setCursor(x, y);
        gfx.print(status);
    }
};

void setup()
{
    Serial.begin(115200);

    // Initialize the M1Shield with display
    M1Shield.begin(displayProvider);

    // Start with the file menu
    M1Shield.setScreen(new FileViewerMenu());
}

void loop()
{
    M1Shield.loop();
}
