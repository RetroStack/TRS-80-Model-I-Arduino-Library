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

    // void writeMemory(uint16_t address, uint8_t data);
    // void writeMemory(uint16_t address, uint8_t * data, uint16_t length);
    // void writeMemory(uint16_t address, uint8_t * data, uint16_t length, uint16_t offset);
    // void copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length);
    // void fillMemory(uint8_t fill_data, uint16_t address, uint16_t length);
    // void fillMemory(uint8_t * fill_data, uint16_t length, uint16_t start_address, uint16_t end_address);

    // uint8_t readIO(uint8_t address);
    // void writeIO(uint8_t address, uint8_t data);

    // // ---------- System Control Signals
    // bool readSystemResetSignal();
    // bool readInterruptAcknowledgeSignal();

    // // ---------- Interrupt Request Signal
    // void activateInterruptRequestSignal();
    // void deactivateInterruptRequestSignal();

    // // ---------- Test Signal
    // void activateTestSignal();
    // void deactivateTestSignal();

    // // ---------- Wait Signal
    // void activateWaitSignal();
    // void deactivateWaitSignal();
}
