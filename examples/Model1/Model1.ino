#include <Arduino.h>
#include <Model1.h>

// Define an accessible variable for the Model 1 instance
Model1 *model1;

void setup()
{
    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();
}

void loop()
{
    // Reads a single byte from memory at location 0x3c00
    uint8_t byte1 = model1->readMemory(0x3c00);

    // Reads a block of length 300 from memory at location 0x3c00
    uint8_t *byte_array = model1->readMemory(0x3c00, 300);
    free(byte_array); // Remember to free up memory when you don't need it anymore

    // Writes a byte value of 0x20 (spacebar) to location 0x3c00 in the video RAM
    model1->writeMemory(0x3c00, 0x20);

    uint8_t data[] = {'a', 'b', 'c', 'd', 'e'};

    // Writes a block of 5 bytes to starting location 0x3c00 in the video RAM
    model1->writeMemory(0x3c00, data, 5);

    // Writes a block of 2 bytes to starting location 0x3c00 in the video RAM with offset of 1 in data
    // Bytes written: 'b', 'c'
    model1->writeMemory(0x3c00, data, 2, 1);

    // Copies the first 10 bytes in the video RAM to the second row of the video RAM (0x40 bytes later = 64 characters)
    model1->copyMemory(0x3c00, 0x3c40, 10);

    // Fills the memory at location 0x3c00 and length 20 with the 'a' character
    model1->fillMemory('a', 0x3c00, 20);

    const char *str = "HELLO WORLD!";

    // Fill the whole screen with "HELLO WORLD!"
    model1->fillMemory(str, strlen(str), 0x3c00, 0x3c00 + (16 * 64));

    // Reads from port 0x3e one byte
    uint8_t byte2 = model1->readIO(0x3e);

    // Write 128 to the port 0x3F
    model1->writeIO(0x3f, 128);

    // Determines wether the Model 1 system is in active reset
    bool isInReset = model1->readSystemResetSignal();

    // Requests interrupt 0x2a to be triggered by the Z80
    // wasTriggered determines wether the Z80 triggered the interrupt within timeout
    bool wasTriggered = model1->triggerInterrupt(0x2a);

    // Activates the wait signal for the Z80, freezing it
    model1->activateWaitSignal();

    // Deactivates the wait signal, freeing the Z80 up again to do other things
    model1->deactivateWaitSignal();

    // Deactivates the test signal
    // This will stop you from controlling the Model 1 busses.
    // model1->deactivateTestSignal();
}
