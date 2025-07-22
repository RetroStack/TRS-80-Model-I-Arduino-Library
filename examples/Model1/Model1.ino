#include <Arduino.h>
#include <Model1.h>

void setup()
{
    // Be sure to setup the hardware pins
    Model1.begin();

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    Model1.activateTestSignal();
}

void loop()
{
    // Reads a single byte from memory at location 0x3c00
    uint8_t byte1 = Model1.readMemory(0x3c00);

    // Reads a block of length 300 from memory at location 0x3c00
    uint8_t *byte_array = Model1.readMemory(0x3c00, 300);
    delete[] byte_array; // Remember to free up memory when you don't need it anymore

    // Writes a byte value of 0x20 (spacebar) to location 0x3c00 in the video RAM
    Model1.writeMemory(0x3c00, 0x20);

    delay(2000);

    uint8_t data[] = {'a', 'b', 'c', 'd', 'e'};

    // Writes a block of 5 bytes to starting location 0x3c00 in the video RAM
    Model1.writeMemory(0x3c00, data, 5);

    delay(2000);

    // Writes a block of 2 bytes to starting location 0x3c00 in the video RAM with offset of 1 in data
    // Bytes written: 'b', 'c'
    Model1.writeMemory(0x3c00, data, 2, 1);

    delay(2000);

    // Copies the first 10 bytes in the video RAM to the second row of the video RAM (0x40 bytes later = 64 characters)
    Model1.copyMemory(0x3c00, 0x3c40, 10);

    delay(2000);

    // Fills the memory at location 0x3c00 and length 20 with the 'a' character
    Model1.fillMemory('a', 0x3c00, 20);

    delay(2000);

    const char *str = "HELLO WORLD!";

    // Fill the whole screen with "HELLO WORLD!"
    Model1.fillMemory((uint8_t *)str, strlen(str), 0x3c00, 16 * 64);

    delay(2000);

    // Reads from port 0x3e one byte
    uint8_t byte2 = Model1.readIO(0x3e);

    // Write 128 to the port 0x3F
    Model1.writeIO(0x3f, 128);

    // Determines whether the Model 1 system is in active reset
    bool isInReset = Model1.readSystemResetSignal();

    // Requests interrupt 0x2a to be triggered by the Z80
    // wasTriggered determines whether the Z80 triggered the interrupt within timeout
    bool wasTriggered = Model1.triggerInterrupt(0x2a);

    // Activates the wait signal for the Z80, freezing it
    Model1.activateWaitSignal();

    // Deactivates the wait signal, freeing the Z80 up again to do other things
    Model1.deactivateWaitSignal();

    // Deactivates the test signal
    // This will stop you from controlling the Model 1 busses.
    // Model1.deactivateTestSignal();
}
