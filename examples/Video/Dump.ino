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

    video = new Video(model1);
}

void loop()
{
    model1->activateWaitSignal();
    model1->activateTestSignal();
    for (uint16_t i = 0; i < 1024; i++)
    {
        videoMemory[i] = model1->readMemory(0x3C00 + i);
    }
    model1->deactivateTestSignal();
    model1->deactivateWaitSignal();

    Serial.println("---- Video Memory Dump ----");

    for (uint16_t y = 0; y < 16; y++)
    {
        for (uint16_t x = 0; x < 64; x++)
        {
            Serial.print(video->convertModel1CharacterToLocal((char)videoMemory[x + (y * 64)]));
        }
        Serial.println();
    }

    delay(1000);
}
