#include <Arduino.h>
#include <Model1.h>
#include <Video.h>

// Define an accessible variable for the Model 1 instance
Model1 *model1;

Video *video;

// 1K video memory mirror
uint8_t videoMemory[1024];

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Clear video memory mirror
    memset(videoMemory, 0, sizeof(videoMemory));

    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Initialize the video interface
    video = new Video(model1);
}

void loop()
{
    // Quickly activate TEST signal
    model1->activateTestSignal();

    // Read all bytes of the video memory (1k at 0x3C00)
    // And record it in a local array for display since we want to keep this quick.
    for (uint16_t i = 0; i < 1024; i++)
    {
        videoMemory[i] = model1->readMemory(0x3C00 + i);
    }

    // Deactivate TEST signal
    model1->deactivateTestSignal();

    Serial.println("---- Video Memory Dump ----");

    // Start printing all characters one-by-one from the array
    for (uint16_t y = 0; y < 16; y++)
    {
        for (uint16_t x = 0; x < 64; x++)
        {
            // Calculate the x/y address in a two dimensional array
            int addr = x + (y * 64);

            // Convert from byte to character
            char character = (char)videoMemory[addr];

            // Convert from Model 1 character set to the local system character set
            character = video->convertModel1CharacterToLocal(character);

            // Print the character to the serial port
            Serial.print(character);
        }
        Serial.println(); // Finish the line
    }

    // Let's pause this for 1 second to then fetch a new screenshot
    delay(1000);
}
