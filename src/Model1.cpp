/*
 * Model1.cpp - Class for accessing the TRS-80 Model 1 low-level signals
 * Authors: Ven Reddy, Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include "Model1.h"
#include "utils.h"
#include "Model1LowLevel.h"

// Refresh trigger
//
// Fasted possible with current code:
//   (71+1) x 62.5ns = 4.5us -> 4.5us * 128 rows => 0.576ms
//
// Reasonable:
//   (89+1) x 62.5ns = 5.625us -> 5.625us * 128 rows => 0.720ms
//
// Reasonable:
//   (100+1) x 62.5ns = 6.3125us -> 6.3125us * 128 rows => 0.808ms
//
// Minimum needed:
//   (124+1) x 62.5ns = 7.8125us -> 7.8125us * 128 rows => 1.0ms
#define CTC_TRIGGER 89

// Version constants
#define M1_VERSION_MAJOR 1
#define M1_VERSION_MINOR 3
#define M1_VERSION_REVISION 0

// Define global instance
Model1Class Model1;

/**
 * Constructor for accessing the Model 1
 */
Model1Class::Model1Class()
{
    _logger = nullptr;
    _timer = -1; // Set to default off

    // Defines the mutability of the bus systems and signals (e.g. activate TEST signal)
    _mutability = false;

    // Initializes memory refresh to default values
    deactivateMemoryRefresh();
}

/**
 * Hardware initialization
 */
void Model1Class::begin(int refreshTimer)
{
    _addressBus.begin();
    _dataBus.begin();

    _initSystemControlSignals();
    _initExternalControlSignals();

    _deactivateBusControlSignals();
    _deactivateBusAccessSignals();

    if (refreshTimer != -1)
    {
        _timer = refreshTimer;
    }

    if (refreshTimer == 1)
    {
        _setupMemoryRefreshTimer1();
    }
    else if (refreshTimer == 2)
    {
        _setupMemoryRefreshTimer2();
    }
    else
    {
        deactivateMemoryRefresh();
    }
}

/**
 * Hardware deinitialization
 */
void Model1Class::end()
{
    _addressBus.end();
    _dataBus.end();

    _deactivateBusControlSignals();
    _deactivateBusAccessSignals();

    deactivateMemoryRefresh();
}

/**
 * Sets a logger being used.
 */
void Model1Class::setLogger(ILogger &logger)
{
    _logger = &logger;
    _addressBus.setLogger(logger);
    _dataBus.setLogger(logger);
}

// ----------------------------------------
// ---------- Address Space
// ----------------------------------------

/**
 * Checks whether an address is in the ROM address space
 */
bool Model1Class::isROMAddress(uint16_t address)
{
    return (address <= 0x2FFF);
}

/**
 * Checks whether an address is in the unused address space
 */
bool Model1Class::isUnusedAddress(uint16_t address)
{
    return (address >= 0x3000 && address <= 0x37DF);
}

/**
 * Checks whether an address is in the memory-mapped IO address space
 */
bool Model1Class::isMemoryMappedIOAddress(uint16_t address)
{
    return (address >= 0x37E0 && address <= 0x37FF);
}

/**
 * Checks whether an address is in the keyboard address space
 */
bool Model1Class::isKeyboardAddress(uint16_t address)
{
    return (address >= 0x3800 && address <= 0x3BFF); // memory space is shadowed from 0x3900 to 0x3BFF (3x)
}

/**
 * Checks whether an address is in the video address space
 */
bool Model1Class::isVideoAddress(uint16_t address)
{
    return (address >= 0x3C00 && address <= 0x3FFF);
}

/**
 * Checks whether an address is in the system address space
 */
bool Model1Class::isSystemAddress(uint16_t address)
{
    return (address >= 0x4000 && address <= 0x41FF);
}

/**
 * Checks whether an address is in the lower memory address space
 */
bool Model1Class::isLowerMemoryAddress(uint16_t address)
{
    return (address >= 0x4200 && address <= 0x7FFF);
}

/**
 * Checks whether an address is in the higher memory address space
 */
bool Model1Class::isHigherMemoryAddress(uint16_t address)
{
    return (address >= 0x8000 && address <= 0xFFFF);
}

// ----------------------------------------
// ---------- Mutability
// ----------------------------------------

/**
 * Marks bus systems and signals as mutable
 */
void Model1Class::_setMutable()
{
    _setMutability(true);
}

/**
 * Marks bus systems and signals as immutable
 */
void Model1Class::_setImmutable()
{
    _setMutability(false);
}

/**
 * Sets the mutability of the bus systems and signals
 */
void Model1Class::_setMutability(bool value)
{
    _mutability = value;
}

/**
 * Checks whether the system is mutable
 */
bool Model1Class::_isMutable()
{
    return _mutability;
}

/**
 * Runs a check whether the system is mutable.
 */
bool Model1Class::_checkMutability()
{
    bool mutability = _isMutable();
    if (!_isMutable())
    {
        if (_logger)
            _logger->err("System is not mutable, but a request to access the system was made.");
    }
    return mutability;
}

// ----------------------------------------
// ---------- Refresh
// ----------------------------------------

/**
 * Sets up a predefined memory refresh interrupt for Timer 1
 */
void Model1Class::_setupMemoryRefreshTimer1()
{
    uint8_t oldSREG = SREG;
    noInterrupts();

    // Reset and init
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 &= ~(1 << OCIE1A);

    OCR1A = CTC_TRIGGER;

    TCCR1B |= (1 << WGM12); // Turn on CTC mode
    TCCR1B |= (1 << CS10);  // Set prescaler to 1

    SREG = oldSREG;
}

/**
 * Sets up a predefined memory refresh interrupt for Timer 2
 */
void Model1Class::_setupMemoryRefreshTimer2()
{
    uint8_t oldSREG = SREG;
    noInterrupts();

    // Reset and init
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    TIMSK2 &= ~(1 << OCIE2A);

    OCR2A = CTC_TRIGGER;

    TCCR2A |= (1 << WGM21); // Turn on CTC mode
    TCCR2B |= (1 << CS20);  // Set prescaler to 1

    SREG = oldSREG;
}

/**
 * Activates the refresh timer
 */
void Model1Class::activateMemoryRefresh()
{
    _activeRefresh = true;
    if (_timer == 1)
    {
        TCNT1 = 0;
        TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
    }
    else if (_timer == 2)
    {
        TCNT2 = 0;
        TIMSK2 |= (1 << OCIE2A); // Enable timer compare interrupt
    }
}

/**
 * Deactivates the refresh timer
 */
void Model1Class::deactivateMemoryRefresh()
{
    if (_timer == 1)
    {
        TIMSK1 &= ~(1 << OCIE1A); // Disable timer compare interrupt
    }
    else if (_timer == 2)
    {
        TIMSK2 &= ~(1 << OCIE2A); // Disable timer compare interrupt
    }
    _activeRefresh = false;
}

/**
 * Executing the next update for the model 1
 */
void Model1Class::nextUpdate()
{
    _refreshNextMemoryRow();
}

/**
 * Refreshes the next row-address for dynamic RAM.
 *
 * NOTE: The function keeps track of the address counting. Make sure the bus is mutable before calling this.
 */
void Model1Class::_refreshNextMemoryRow()
{
    // This function expects mutibility as well as that the refresh is activated
    uint8_t currentRefreshRow = _nextMemoryRefreshRow;

    // Prepare next address; quicker way to manage overflow
    _nextMemoryRefreshRow = (_nextMemoryRefreshRow + 1) & 0x7F;

    // Set address
    _addressBus.writeRefreshAddress(currentRefreshRow);

    // Timing of various signals
    Model1LowLevel::writeRAS(LOW); // 45ns (62.5ns, but when the pulse is down)
    asmNoop();                     // 125ns
    asmNoop();                     // 125ns
    asmNoop();                     // 125ns

    // Reset, leaving address as-is
    Model1LowLevel::writeRAS(HIGH); // 45ns (62.5ns, but when the pulse is down)
}

// ----------------------------------------
// ---------- Memory
// ----------------------------------------

/**
 * Reads from a memory address
 */
uint8_t Model1Class::readMemory(uint16_t address)
{
    // Verification of access
    if (!_checkMutability())
        return 0;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Set address and then data
    _addressBus.writeMemoryAddress(address);

    // Timing of various signals
    Model1LowLevel::writeRAS(LOW);
    Model1LowLevel::writeRD(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);
    asmWait(3); // 772 ns

    // Read data
    uint8_t data = _dataBus.readData();

    // Reset, leaving address as-is
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeRD(HIGH);
    Model1LowLevel::writeRAS(HIGH);
    Model1LowLevel::writeMUX(LOW);

    SREG = oldSREG;

    return data;
}

/**
 * Writes to a memory address
 */
void Model1Class::writeMemory(uint16_t address, uint8_t data)
{
    // Verification of access
    if (!_checkMutability())
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Configure bus
    _dataBus.setAsWritable();

    // Set address and then data
    _addressBus.writeMemoryAddress(address);
    _dataBus.writeData(data);

    // Timing of various signals
    Model1LowLevel::writeRAS(LOW);
    asmNoop();
    asmNoop();
    asmNoop();
    Model1LowLevel::writeWR(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);
    asmWait(1); // 252 ns

    // Reset, leaving address as-is, removing data
    Model1LowLevel::writeWR(HIGH);
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeRAS(HIGH);
    Model1LowLevel::writeMUX(LOW);
    _dataBus.setAsReadable();

    SREG = oldSREG;
}

/**
 * Reads a block of data from a memory address
 */
uint8_t *Model1Class::readMemory(uint16_t address, uint16_t length)
{
    if (length == 0)
        return nullptr;

    uint8_t *buffer = new uint8_t[length];
    for (uint16_t i = 0; i < length; i++)
    {
        buffer[i] = readMemory(address + i);
    }

    return buffer;
}

/**
 * Writes a block of data to a memory address
 */
void Model1Class::writeMemory(uint16_t address, uint8_t *data, uint16_t length)
{
    writeMemory(address, data, length, 0);
}

/**
 * Writes a block of data to a memory address by providing an offset of the original data
 */
void Model1Class::writeMemory(uint16_t address, uint8_t *data, uint16_t length, uint16_t offset)
{
    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(address + i, data[offset + i]);
    }
}

/**
 * Copy a block of memory from a source to destination by providing the length of data to be copied
 */
void Model1Class::copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length)
{
    if (length == 0)
        return;
    if (dst_address == src_address)
        return;

    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(dst_address + i, readMemory(src_address + i));
    }
}

/**
 * Fill a specific block of memory with a byte
 */
void Model1Class::fillMemory(uint8_t fill_data, uint16_t address, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(address + i, fill_data);
    }
}

/**
 * Fill a specific block of memory with a byte-array, repeated until the end address
 */
void Model1Class::fillMemory(uint8_t *fill_data, uint16_t length, uint16_t address, uint16_t address_length)
{
    for (uint16_t i = 0; i < address_length; i += length)
    {
        for (uint16_t j = 0; j < length; j++)
        {
            writeMemory(address + i + j, fill_data[j]);

            // Check if we reached the end address
            if (i + j >= address_length)
                return;
        }
    }
}

// ----------------------------------------
// ---------- IO
// ----------------------------------------

/**
 * Reads from an IO address
 */
uint8_t Model1Class::readIO(uint8_t address)
{
    // Verification of access
    if (!_checkMutability())
        return 0;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Set address and then data
    _addressBus.writeIOAddress(address);

    // Timing of various signals
    Model1LowLevel::writeIN(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);

    // Read data
    uint8_t data = _dataBus.readData();

    // Reset, leaving address as-is
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeIN(HIGH);
    Model1LowLevel::writeMUX(LOW);

    SREG = oldSREG;

    return data;
}

/**
 * Writes to an IO address
 */
void Model1Class::writeIO(uint8_t address, uint8_t data)
{
    // Verification of access
    if (!_checkMutability())
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Configure bus
    _dataBus.setAsWritable();

    // Set address and then data
    _addressBus.writeMemoryAddress(address);
    _dataBus.writeData(data);

    // Timing of various signals
    Model1LowLevel::writeOUT(LOW);
    Model1LowLevel::writeMUX(HIGH);
    Model1LowLevel::writeCAS(LOW);
    asmWait(1); // 252 ns

    // Reset, leving address as-is, removing data
    Model1LowLevel::writeCAS(HIGH);
    Model1LowLevel::writeOUT(HIGH);
    Model1LowLevel::writeMUX(LOW);
    _dataBus.setAsReadable();

    SREG = oldSREG;
}

// ----------------------------------------
// ---------- System Control Signals
// ----------------------------------------

/**
 * Initializes the system control signals
 *
 * These are read-only signals.
 */
void Model1Class::_initSystemControlSignals()
{
    Model1LowLevel::writeSYS_RES(LOW);
    Model1LowLevel::writeINT_ACK(LOW);

    Model1LowLevel::configWriteSYS_RES(INPUT);
    Model1LowLevel::configWriteINT_ACK(INPUT);
}

/**
 * Reads whether a system reset has occurred.
 */
bool Model1Class::readSystemResetSignal()
{
    return Model1LowLevel::readSYS_RES() == LOW ? true : false;
}

/**
 * Reads whether the CPU has acknowledged an interrupt request.
 */
bool Model1Class::readInterruptAcknowledgeSignal()
{
    return Model1LowLevel::readINT_ACK() == LOW ? true : false;
}

// ----------------------------------------
// ---------- Memory Control Signals
// ----------------------------------------

/**
 * Activates bus control signals
 */
void Model1Class::_activateBusControlSignals()
{
    _resetBusControlSignals();

    Model1LowLevel::configWriteRAS(OUTPUT);
    Model1LowLevel::configWriteMUX(OUTPUT);
    Model1LowLevel::configWriteCAS(OUTPUT);
}

/**
 * Deactivates bus control signals
 */
void Model1Class::_deactivateBusControlSignals()
{
    Model1LowLevel::configWriteRAS(INPUT);
    Model1LowLevel::configWriteMUX(INPUT);
    Model1LowLevel::configWriteCAS(INPUT);
}

/**
 * Resets the bus control signals to the default configuration
 */
void Model1Class::_resetBusControlSignals()
{
    Model1LowLevel::writeRAS(HIGH);
    Model1LowLevel::writeMUX(LOW);
    Model1LowLevel::writeCAS(HIGH);
}

// ----------------------------------------
// ---------- Memory Access Signals
// ----------------------------------------

/**
 * Activates the bus access signals
 */
void Model1Class::_activateBusAccessSignals()
{
    _resetBusAccessSignals();

    Model1LowLevel::configWriteRD(OUTPUT);
    Model1LowLevel::configWriteWR(OUTPUT);

    Model1LowLevel::configWriteIN(OUTPUT);
    Model1LowLevel::configWriteOUT(OUTPUT);
}

/**
 * Deactivates the bus access signals
 */
void Model1Class::_deactivateBusAccessSignals()
{
    Model1LowLevel::configWriteRD(INPUT);
    Model1LowLevel::configWriteWR(INPUT);

    Model1LowLevel::configWriteIN(INPUT);
    Model1LowLevel::configWriteOUT(INPUT);
}

/**
 * Resets the bus access signals to default
 */
void Model1Class::_resetBusAccessSignals()
{
    Model1LowLevel::writeRD(HIGH);
    Model1LowLevel::writeWR(HIGH);
    Model1LowLevel::writeIN(HIGH);
    Model1LowLevel::writeOUT(HIGH);
}

// ----------------------------------------
// ---------- External Control Signals
// ----------------------------------------

/**
 * Initializes the external control signals
 *
 * These are write-only signals.
 */
void Model1Class::_initExternalControlSignals()
{
    Model1LowLevel::writeINT(HIGH);
    Model1LowLevel::writeTEST(HIGH);
    Model1LowLevel::writeWAIT(HIGH);

    Model1LowLevel::configWriteINT(OUTPUT);
    Model1LowLevel::configWriteTEST(OUTPUT);
    Model1LowLevel::configWriteWAIT(OUTPUT);
}

// ---------- Interrupt Request Signal

/**
 * Sets the interrupt request signal
 */
void Model1Class::_setInterruptRequestSignal(bool value)
{
    Model1LowLevel::writeINT(value ? LOW : HIGH);
}

/**
 * Triggers an interrupt within the Model 1
 *
 * NOTE: The timeout unit is in about microseconds.
 */
bool Model1Class::triggerInterrupt(uint8_t interrupt, uint16_t timeout)
{
    activateInterruptRequestSignal();

    for (uint16_t i = 0; i < timeout; i++)
    {
        if (Model1LowLevel::readINT_ACK() == LOW)
        {
            _dataBus.setAsWritable();
            _dataBus.writeData(interrupt);

            asmWait(3);
            deactivateInterruptRequestSignal();
            asmWait(3);

            _dataBus.setAsReadable();

            return true;
        }
        asmNoop();
    }

    deactivateInterruptRequestSignal();

    return false; // CPU did not respond within timeout
}

/**
 * Activates the interrupt request signal
 */
void Model1Class::activateInterruptRequestSignal()
{
    if (Model1LowLevel::readINT() == LOW)
    {
        if (_logger)
            _logger->warn("INT* signal already active.");
        return;
    }

    _setInterruptRequestSignal(true);
}

/**
 * Deactivates the interrupt request signal
 */
void Model1Class::deactivateInterruptRequestSignal()
{
    if (Model1LowLevel::readINT() == HIGH)
    {
        if (_logger)
            _logger->warn("INT* signal already deactive.");
        return;
    }

    _setInterruptRequestSignal(false);
}

// ---------- Test Signal

/**
 * Sets *TEST system to define control of the Model 1
 */
void Model1Class::_setTestSignal(bool value)
{
    Model1LowLevel::writeTEST(value ? LOW : HIGH);
}

/**
 * Returns wether the test signal is active
 */
bool Model1Class::hasActiveTestSignal()
{
    return Model1LowLevel::readTEST() == LOW;
}

/**
 * Activates the *TEST signal and let's this system take over.
 */
void Model1Class::activateTestSignal()
{
    if (Model1LowLevel::readTEST() == LOW)
    {
        if (_logger)
            _logger->warn("TEST* signal already active.");
        return;
    }

    // Activate the TEST* signal
    _setTestSignal(true);

    // Wait to avoid contention
    asmWait(16); // Wait 4us. Only need 3 (value 12) for 5T, but this is to be safe

    // Set the signals as active from external system
    _addressBus.setAsWritable();
    _dataBus.setAsReadable();
    _activateBusControlSignals();
    _activateBusAccessSignals();

    // Mark bus as mutable, enabling write requests from this code
    _setMutable();

    // Activate background services
    if (_timer != -1)
    {
        activateMemoryRefresh();
    }
}

/**
 * Deactivates the *TEST signal and hands the system back to the CPU.
 */
void Model1Class::deactivateTestSignal()
{
    if (Model1LowLevel::readTEST() == HIGH)
    {
        if (_logger)
            _logger->warn("TEST* signal already deactive.");
        return;
    }

    // Deactivate background services
    if (_timer != -1)
    {
        deactivateMemoryRefresh();
    }

    // Set bus as immutable, blocking write requests from this code
    _setImmutable();

    // Set the signals as inactive from external system
    _deactivateBusAccessSignals();
    _deactivateBusControlSignals();
    _dataBus.setAsReadable();
    _addressBus.setAsReadable();

    // Deactivate TEST* signal
    _setTestSignal(false);

    // Wait to avoid contention
    asmWait(16); // Wait 4us. Only need 3 (value 12) for 5T, but this is to be safe
}

// ---------- Wait Signal

/**
 * Sets the *WAIT signal to hold CPU
 */
void Model1Class::_setWaitSignal(bool value)
{
    Model1LowLevel::writeWAIT(value ? LOW : HIGH);
}

/**
 * Activates the *WAIT signal to hold the CPU
 */
void Model1Class::activateWaitSignal()
{
    if (Model1LowLevel::readWAIT() == LOW)
    {
        if (_logger)
            _logger->warn("WAIT* signal already active.");
        return;
    }

    _setWaitSignal(true);
}

/**
 * Deactivates the *WAIT signal, giving the CPU a "full-speed-ahead"
 */
void Model1Class::deactivateWaitSignal()
{
    if (Model1LowLevel::readWAIT() == HIGH)
    {
        if (_logger)
            _logger->warn("WAIT* signal already deactive.");
        return;
    }

    _setWaitSignal(false);
}

// ---------- State

/**
 * Returns the current state as string
 */
char *Model1Class::getState()
{
    char *addrStatus = _addressBus.getState();
    char *dataStatus = _dataBus.getState();

    const int LEN = 255;
    char *buffer = new char[LEN];
    snprintf(
        buffer,
        LEN,
        "Mut<%c>, RfshEn<%c>, RfshRow<%3.d>, RD<%c>(%d), WR<%c>(%d), IN<%c>(%d), OUT<%c>(%d), %s, %s, RAS<%c>(%d), CAS<%c>(%d), MUX<%c>(%d), SYS_RES<%c>(%d), INT_ACK<%c>(%d), INT<%c>(%d), TEST<%c>(%d), WAIT<%c>(%d)",

        _mutability ? 'T' : 'F',
        _activeRefresh ? 'T' : 'F',
        _nextMemoryRefreshRow,

        pinStatus(Model1LowLevel::configReadRD()), Model1LowLevel::readRD(),
        pinStatus(Model1LowLevel::configReadWR()), Model1LowLevel::readWR(),
        pinStatus(Model1LowLevel::configReadIN()), Model1LowLevel::readIN(),
        pinStatus(Model1LowLevel::configReadOUT()), Model1LowLevel::readOUT(),

        addrStatus,
        dataStatus,

        pinStatus(Model1LowLevel::configReadRAS()), Model1LowLevel::readRAS(),
        pinStatus(Model1LowLevel::configReadCAS()), Model1LowLevel::readCAS(),
        pinStatus(Model1LowLevel::configReadMUX()), Model1LowLevel::readMUX(),

        pinStatus(Model1LowLevel::configReadSYS_RES()), Model1LowLevel::readSYS_RES(),
        pinStatus(Model1LowLevel::configReadINT_ACK()), Model1LowLevel::readINT_ACK(),

        pinStatus(Model1LowLevel::configReadINT()), Model1LowLevel::readINT(),
        pinStatus(Model1LowLevel::configReadTEST()), Model1LowLevel::readTEST(),
        pinStatus(Model1LowLevel::configReadWAIT()), Model1LowLevel::readWAIT());

    delete[] addrStatus;
    delete[] dataStatus;

    return buffer;
}

/**
 * Returns the current state as packed data
 *
 * Bit layout (64-bit):
 * Bits 63-48: Address (16 bits) - Memory address bus
 * Bits 47-40: Data (8 bits) - Data bus value
 * Bits 39-32: Memory control signals (8 bits) - RD, WR, IN, OUT, RAS, CAS, MUX, (1 spare)
 * Bits 31-24: System signals (8 bits) - SYS_RES, INT_ACK, INT, TEST, WAIT, (3 spare)
 * Bits 23-0:  Reserved for future use (24 bits)
 */
uint64_t Model1Class::getStateData()
{
    return Model1LowLevel::getStateData();
}

/**
 * Returns the current configuration state as packed data
 *
 * Bit layout (64-bit):
 * Bits 63-48: Address Bus Config (16 bits) - Memory address bus pin configurations
 * Bits 47-40: Data Bus Config (8 bits) - Data bus pin configurations
 * Bits 39-32: Memory control signal configs (8 bits) - RD, WR, IN, OUT, RAS, CAS, MUX, (1 spare)
 * Bits 31-24: System signal configs (8 bits) - SYS_RES, INT_ACK, INT, TEST, WAIT, (3 spare)
 * Bits 23-0:  Reserved for future use (24 bits)
 *
 * Note: Configuration values represent pin direction (INPUT=0, OUTPUT=1)
 */
uint64_t Model1Class::getStateConfigData()
{
    return Model1LowLevel::getStateConfigData();
}

/**
 * Logs the current state
 */
void Model1Class::logState()
{
    if (_logger)
    {
        char *state = getState();
        _logger->info("State: %s", state);
        delete[] state;
    }
}

// ---------- Version

/**
 * Gets the major version number
 */
uint8_t Model1Class::getVersionMajor()
{
    return M1_VERSION_MAJOR;
}

/**
 * Gets the minor version number
 */
uint8_t Model1Class::getVersionMinor()
{
    return M1_VERSION_MINOR;
}

/**
 * Gets the revision version number
 */
uint8_t Model1Class::getVersionRevision()
{
    return M1_VERSION_REVISION;
}

/**
 * Returns a version string
 */
char *Model1Class::getVersion()
{
    const int LEN = 255;
    char *buffer = new char[LEN];
    snprintf(
        buffer,
        LEN,
        "%d.%d.%d",
        M1_VERSION_MAJOR,
        M1_VERSION_MINOR,
        M1_VERSION_REVISION);
    return buffer;
}

/**
 * Prints memory contents in a human readable format
 */
void Model1Class::printMemoryContents(uint16_t start, uint16_t length, PRINT_STYLE style, bool relative, uint16_t bytesPerLine)
{
    if (!_logger)
        return;

    printMemoryContents(*_logger, start, length, style, relative, bytesPerLine);
}

/**
 * Prints the contents of a memory location in a human-readable format
 */
void Model1Class::printMemoryContents(Print &output, uint16_t start, uint16_t length, PRINT_STYLE style, bool relative, uint16_t bytesPerLine)
{
    if (bytesPerLine == 0 || bytesPerLine > 60)
    {
        if (_logger)
            _logger->err("Unsupported value for bytesPerLine wth %d.", bytesPerLine);
        return;
    }

    uint8_t *buffer = (uint8_t *)malloc(bytesPerLine);
    if (!buffer)
    {
        if (_logger)
            _logger->err("Cannot allocate byte buffer.");
        return;
    }

    uint16_t lineLength = 6; // "0000: "
    if (style == HEXADECIMAL || style == BOTH)
        lineLength += 3 * bytesPerLine;
    if (style == BOTH)
        lineLength += 3; // " |" and closing '|'
    if (style == ASCII || style == BOTH)
        lineLength += bytesPerLine;
    lineLength += 1;

    char *lineBuffer = (char *)malloc(lineLength);
    if (!lineBuffer)
    {
        if (_logger)
            _logger->err("Cannot allocate line buffer.");
        free(buffer);
        return;
    }

    for (uint16_t offset = 0; offset < length; offset += bytesPerLine)
    {
        uint16_t lineLengthActual = (offset + bytesPerLine <= length)
                                        ? bytesPerLine
                                        : (length - offset);

        // Load from ROM
        for (uint16_t i = 0; i < lineLengthActual; ++i)
        {
            buffer[i] = readMemory(start + offset + i);
        }

        // Build the line string
        char *p = lineBuffer;

        // Address
        uint16_t addr = relative ? offset : (start + offset);
        p += sprintf(p, "%04X: ", addr);

        // Hex bytes
        if (style == BOTH || style == HEXADECIMAL)
        {
            for (uint16_t i = 0; i < bytesPerLine; ++i)
            {
                if (i < lineLengthActual)
                {
                    p += sprintf(p, "%02X ", buffer[i]);
                }
                else
                {
                    p += sprintf(p, "   "); // Padding for short lines
                }
            }
        }

        if (style == BOTH)
        {
            p += sprintf(p, " |");
        }

        // ASCII representation
        if (style == BOTH || style == ASCII)
        {
            for (uint16_t i = 0; i < lineLengthActual; ++i)
            {
                uint8_t b = buffer[i];
                if (b >= 32 && b <= 126)
                {
                    *p++ = b; // Printable ASCII
                }
                else
                {
                    *p++ = '.'; // Non-printable
                }
            }
        }

        if (style == BOTH)
        {
            *p++ = '|';
        }

        *p = '\0'; // Null-terminate

        // Print the entire line
        output.println(lineBuffer);
    }

    free(buffer);
    free(lineBuffer);
}