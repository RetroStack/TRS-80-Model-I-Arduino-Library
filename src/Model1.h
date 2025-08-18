/*
 * Model1.h - Class for accessing the TRS-80 Model 1 low-level signals
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#ifndef MODEL1_H
#define MODEL1_H

#include <Arduino.h>
#include "ILogger.h"
#include "AddressBus.h"
#include "DataBus.h"

enum PRINT_STYLE
{
    ASCII,
    HEXADECIMAL,
    BOTH
};

class Model1Class
{
private:
    ILogger *_logger;
    AddressBus _addressBus;
    DataBus _dataBus;

    volatile bool _mutability;
    uint8_t _nextMemoryRefreshRow;
    volatile bool _activeRefresh;
    int _timer;

    void _setMutable();
    void _setImmutable();
    void _setMutability(bool value);
    bool _isMutable();
    bool _checkMutability();

    void _refreshNextMemoryRow();

    void _initSystemControlSignals();
    void _initExternalControlSignals();

    void _setupMemoryRefreshTimer1();
    void _setupMemoryRefreshTimer2();

    void _activateBusControlSignals();
    void _deactivateBusControlSignals();
    void _resetBusControlSignals();

    void _activateBusAccessSignals();
    void _deactivateBusAccessSignals();
    void _resetBusAccessSignals();

    void _setInterruptRequestSignal(bool value);
    void _setTestSignal(bool value);
    void _setWaitSignal(bool value);

public:
    Model1Class(); // Constructor

    void begin(int refreshTimer = -1); // Initialize Model 1 interface
    void end();                        // Shutdown Model 1 interface

    void setLogger(ILogger &logger); // Set logger for debugging output

    void activateMemoryRefresh();   // Enable memory refresh cycles
    void deactivateMemoryRefresh(); // Disable memory refresh cycles

    // ---------- Address Space
    bool isROMAddress(uint16_t address);            // Check if address is in ROM space
    bool isUnusedAddress(uint16_t address);         // Check if address is unused
    bool isMemoryMappedIOAddress(uint16_t address); // Check if address is memory-mapped I/O
    bool isKeyboardAddress(uint16_t address);       // Check if address is keyboard interface
    bool isVideoAddress(uint16_t address);          // Check if address is video memory
    bool isSystemAddress(uint16_t address);         // Check if address is system space
    bool isLowerMemoryAddress(uint16_t address);    // Check if address is lower memory
    bool isHigherMemoryAddress(uint16_t address);   // Check if address is higher memory

    // ---------- Update
    void nextUpdate(); // Process next update cycle

    // ---------- Memory
    uint8_t readMemory(uint16_t address);             // Read byte from memory
    void writeMemory(uint16_t address, uint8_t data); // Write byte to memory
    // Returns a newly allocated buffer; caller must delete[] the result
    uint8_t *readMemory(uint16_t address, uint16_t length);                                             // Read memory block
    void writeMemory(uint16_t address, uint8_t *data, uint16_t length);                                 // Write memory block
    void writeMemory(uint16_t address, uint8_t *data, uint16_t length, uint16_t offset);                // Write memory block with offset
    void copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length);                       // Copy memory between addresses
    void fillMemory(uint8_t fill_data, uint16_t address, uint16_t length);                              // Fill memory with byte value
    void fillMemory(uint8_t *fill_data, uint16_t length, uint16_t start_address, uint16_t end_address); // Fill memory with pattern

    // ---------- IO
    uint8_t readIO(uint8_t address);             // Read from I/O port
    void writeIO(uint8_t address, uint8_t data); // Write to I/O port

    // ---------- System Control Signals
    bool readSystemResetSignal();          // Read system reset signal state
    bool readInterruptAcknowledgeSignal(); // Read interrupt acknowledge signal state

    // ---------- Interrupt Request Signal
    bool triggerInterrupt(uint8_t interrupt, uint16_t timeout = 1000); // Trigger interrupt with timeout
    void activateInterruptRequestSignal();                             // Activate interrupt request signal
    void deactivateInterruptRequestSignal();                           // Deactivate interrupt request signal

    // ---------- Test Signal
    bool hasActiveTestSignal();  // Check if test signal is active
    void activateTestSignal();   // Activate test signal
    void deactivateTestSignal(); // Deactivate test signal

    // ---------- Wait Signal
    void activateWaitSignal();   // Activate wait signal
    void deactivateWaitSignal(); // Deactivate wait signal

    // ---------- State
    char *getState();              // Get current state as string
    uint64_t getStateData();       // Get state data as 64-bit value
    uint64_t getStateConfigData(); // Get configuration state data
    void logState();               // Log current state to logger

    uint8_t getVersionMajor();    // Get major version number
    uint8_t getVersionMinor();    // Get minor version number
    uint8_t getVersionRevision(); // Get revision number
    char *getVersion();           // Get full version string

    void printMemoryContents(uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32);                // Print memory contents to serial
    void printMemoryContents(Print &output, uint16_t start, uint16_t length, PRINT_STYLE style = BOTH, bool relative = false, uint16_t bytesPerLine = 32); // Print memory contents to output
};

extern Model1Class Model1;

#endif /* MODEL1_H */
