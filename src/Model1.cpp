#include "Model1.h"
#include "utils.h"
#include "port_config.h"
#include "port_macros.h"

// Version constants
const uint8_t VERSION_MAJOR = 0;
const uint8_t VERSION_MINOR = 9;
const uint8_t VERSION_REVISION = 2;

/**
 * Tracking the first instance create for interrupt/event handling
 */
Model1 *globalModel1 = nullptr;

/**
 * Constructor for accessing the Model 1
 *
 * @param logger
 */
Model1::Model1(ILogger *logger)
{
    _logger = logger;

    _addressBus = new AddressBus(logger);
    _dataBus = new DataBus(logger);

    // Register the first Model 1 initialized for interrupt/event handling
    if (globalModel1)
    {
        if (_logger)
            _logger->warn("Global Model 1 already setup. You can only have one.");
    }
    else
    {
        globalModel1 = this;
    }

    // Defines the mutability of the bus systems and signals (e.g. activate TEST signal)
    _mutability = false;

    // Initializes memory refresh to default values
    _deactivateMemoryRefresh();
}

/**
 * Hardware initialization
 */
void Model1::begin(bool memoryRefresh = false)
{
    _addressBus->begin();
    _dataBus->begin();

    _initSystemControlSignals();
    _initExternalControlSignals();

    _deactivateBusControlSignals();
    _deactivateBusAccessSignals();

    _setupMemoryInterrupts();
    _setupIOInterrupts();

    if (memoryRefresh)
    {
        _setupMemoryRefresh();
    }
}

/**
 * Hardware deinitialization
 */
void Model1::end()
{
    _removeMemoryInterrupts();
    _removeIOInterrupts();
}

/**
 * Deconstructor
 */
Model1::~Model1()
{
    if (this == globalModel1)
    {
        globalModel1 = nullptr;
    }
    end();

    delete _addressBus;
    delete _dataBus;
}

/**
 * Constructs the event data
 */
EventData *Model1::_createEventData(uint8_t type)
{
    // Remember the old interrupt values
    uint8_t oldSREG = SREG;

    // Turn off interrupts (if it isn't already)
    noInterrupts();

    EventData *data = new EventData;
    data->type = type;
    data->address = _addressBus->readMemoryAddress();
    data->data = _dataBus->readData();

    // Restore previous interrupt configuration (may need interrupts to be off still)
    SREG = oldSREG;

    return data;
}

// ----------------------------------------
// ---------- Address Space
// ----------------------------------------

/**
 * Checks wether an address is in the ROM address space
 */
bool Model1::isROMAddress(uint16_t address)
{
    return (address <= 0x2FFF);
}

/**
 * Checks wether an address is in the unused address space
 */
bool Model1::isUnusedAddress(uint16_t address)
{
    return (address >= 0x3000 && address <= 0x37DF);
}

/**
 * Checks wether an address is in the memory-mapped IO address space
 */
bool Model1::isMemoryMappedIOAddress(uint16_t address)
{
    return (address >= 0x37E0 && address <= 0x37FF);
}

/**
 * Checks wether an address is in the keyboard address space
 */
bool Model1::isKeyboardAddress(uint16_t address)
{
    return (address >= 0x3800 && address <= 0x3BFF); // memory space is shadowed from 0x3900 to 0x3BFF (3x)
}

/**
 * Checks wether an address is in the video address space
 */
bool Model1::isVideoAddress(uint16_t address)
{
    return (address >= 0x3C00 && address <= 0x3FFF);
}

/**
 * Checks wether an address is in the system address space
 */
bool Model1::isSystemAddress(uint16_t address)
{
    return (address >= 0x4000 && address <= 0x41FF);
}

/**
 * Checks wether an address is in the lower memory address space
 */
bool Model1::isLowerMemoryAddress(uint16_t address)
{
    return (address >= 0x4200 && address <= 0x7FFF);
}

/**
 * Checks wether an address is in the higher memory address space
 */
bool Model1::isHigherMemoryAddress(uint16_t address)
{
    return (address >= 0x8000 && address <= 0xFFFF);
}

// ----------------------------------------
// ---------- Mutability
// ----------------------------------------

/**
 * Marks bus systems and signals as mutable
 */
void Model1::_setMutable()
{
    _setMutability(true);
}

/**
 * Marks bus systems and signals as immutable
 */
void Model1::_setImmutable()
{
    _setMutability(false);
}

/**
 * Sets the mutability of the bus systems and signals
 */
void Model1::_setMutability(bool value)
{
    _mutability = value;
}

/**
 * Checks wether
 */
bool Model1::_isMutable()
{
    return _mutability;
}

/**
 * Runs a check wheter the system is mutable.
 */
bool Model1::_checkMutability()
{
    if (!_isMutable())
    {
        if (_logger)
            _logger->err("System is not mutable, but a request to access the system was made.");
    }
    return _mutability;
}

// ----------------------------------------
// ---------- Refresh
// ----------------------------------------

/**
 * Sets up a predefined memory refresh interrupt
 */
void Model1::_setupMemoryRefresh()
{
    uint8_t oldSREG = SREG;
    noInterrupts();

    // Reset and init
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 250; // ~15.6us -> 15.6us * 128 rows => !0.002ms

    TCCR1B |= (1 << WGM12);  // Turn on CTC mode
    TCCR1B |= (1 << CS10);   // Set prescaler to 1
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt

    SREG = oldSREG;
}

/**
 * Activates the refresh timer
 */
void Model1::_activateMemoryRefresh()
{
    _activeRefresh = true;
}

/**
 * Deactivates the refresh timer
 */
void Model1::_deactivateMemoryRefresh()
{
    _activeRefresh = false;
    _nextMemoryRefreshRow = 0;
}

/**
 * Refreshes the next row-address for dynamic RAM.
 *
 * NOTE: The function keeps track of the address counting.
 */
void Model1::refreshNextMemoryRow()
{
    if (!_activeRefresh)
        return;
    if (!_mutability) // Need direct check here to improve performance; instead _checkMutability
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    uint8_t currentRefreshRow = _nextMemoryRefreshRow;

    // Prepare next address
    _nextMemoryRefreshRow++;
    if (_nextMemoryRefreshRow >= 128)
    {
        _nextMemoryRefreshRow = 0;
    }

    // Set address and then data
    _addressBus->writeRefreshAddress(currentRefreshRow);

    // Timing of various signals
    pinWrite(RAS, LOW);
    asmWait(1);

    // Reset, leaving address as-is
    pinWrite(RAS, HIGH);

    SREG = oldSREG;
}

// ----------------------------------------
// ---------- Memory
// ----------------------------------------

/**
 * Reads from a memory address
 */
uint8_t Model1::readMemory(uint16_t address)
{
    // Verification of access
    if (!_checkMutability())
        return 0;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Set address and then data
    _addressBus->writeMemoryAddress(address);

    // Timing of various signals
    pinWrite(RAS, LOW);
    pinWrite(RD, LOW);
    pinWrite(MUX, HIGH);
    pinWrite(CAS, LOW);
    asmWait(3);

    // Read data
    uint8_t data = _dataBus->readData();

    // Reset, leaving address as-is
    pinWrite(RD, HIGH);
    pinWrite(RAS, HIGH);
    pinWrite(MUX, LOW);
    pinWrite(CAS, HIGH);

    SREG = oldSREG;

    return data;
}

/**
 * Writes to a memory address
 */
void Model1::writeMemory(uint16_t address, uint8_t data)
{
    // Verification of access
    if (!_checkMutability())
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Configure bus
    _dataBus->setAsWritable();

    // Set address and then data
    _addressBus->writeMemoryAddress(address);
    _dataBus->writeData(data);

    // Timing of various signals
    pinWrite(RAS, LOW);
    asmNoop();
    asmNoop();
    asmNoop();
    pinWrite(WR, LOW);
    pinWrite(MUX, HIGH);
    pinWrite(CAS, LOW);
    asmWait(1);

    // Reset, leaving address as-is, removing data
    pinWrite(WR, HIGH);
    pinWrite(RAS, HIGH);
    pinWrite(MUX, LOW);
    pinWrite(CAS, HIGH);
    _dataBus->setAsReadable();

    SREG = oldSREG;
}

/**
 * Reads a block of data from a memory address
 */
uint8_t *Model1::readMemory(uint16_t address, uint16_t length)
{
    if (length == 0)
        return;

    uint8_t *buffer = new uint8_t(length);
    for (uint16_t i = 0; i < length; i++)
    {
        buffer[i] = readMemory(address + i);
    }

    return buffer;
}

/**
 * Writes a block of data to a memory address
 */
void Model1::writeMemory(uint16_t address, uint8_t *data, uint16_t length)
{
    writeMemory(address, data, length, 0);
}

/**
 * Writes a block of data to a memory address by providing an offset of the original data
 */
void Model1::writeMemory(uint16_t address, uint8_t *data, uint16_t length, uint16_t offset)
{
    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(address + i, data[offset + i]);
    }
}

/**
 * Copy a block of memory from a source to destination by providing the length of data to be copied
 */
void Model1::copyMemory(uint16_t src_address, uint16_t dst_address, uint16_t length)
{
    if (length == 0)
        return;
    if (dst_address == src_address)
        return;

    if (dst_address < src_address)
    {
        for (uint16_t i = 0; i < length; i++)
        {
            writeMemory(dst_address + i, readMemory(src_address + i));
        }
    }
    else
    {
        for (uint16_t i = length - 1; i >= 0; i--)
        {
            writeMemory(dst_address + i, readMemory(src_address + i));
        }
    }
}

/**
 * Fill a specific block of memory with a byte
 */
void Model1::fillMemory(uint8_t fill_data, uint16_t address, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        writeMemory(address + i, fill_data);
    }
}

/**
 * Fill a specific block of memory with a byte-array, repeated until the end address
 */
void Model1::fillMemory(uint8_t *fill_data, uint16_t length, uint16_t start_address, uint16_t end_address)
{
    for (uint16_t i = start_address; i <= end_address; i += length)
    {
        for (uint16_t j = 0; j < length; j++)
        {
            writeMemory(i + j, fill_data[j]);

            // Check if we reached the end address
            if (i + j >= end_address)
                return;
        }
    }
}

/**
 * Internal memory read interrupt handler
 */
void handleMemoryRead()
{
    if (globalModel1)
        globalModel1->triggerMemoryReadEvent();
}

/**
 * Internal memory write interrupt handler
 */
void handleMemoryWrite()
{
    if (globalModel1)
        globalModel1->triggerMemoryWriteEvent();
}

/**
 * Sets up all the memory related interrupts
 */
void Model1::_setupMemoryInterrupts()
{
    attachInterrupt(digitalPinToInterrupt(PIN_RD), handleMemoryRead, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_WR), handleMemoryWrite, FALLING);
}

/**
 * Removes all the memory related interrupts
 */
void Model1::_removeMemoryInterrupts()
{
    detachInterrupt(digitalPinToInterrupt(PIN_RD));
    detachInterrupt(digitalPinToInterrupt(PIN_WR));
}

/**
 * Sets a callback for memory read access
 */
void Model1::setMemoryReadCallback(EventMemoryReadCallback callback)
{
    _memoryReadCallback = callback;
}

/**
 * Triggers the memory read event
 */
void Model1::triggerMemoryReadEvent()
{
    if (_memoryReadCallback)
    {
        EventData *data = _createEventData(EVENT_MEMORY_READ);
        _memoryReadCallback(*data);
        free(data);
    }
}

/**
 * Sets a callback for memory write access
 */
void Model1::setMemoryWriteCallback(EventMemoryWriteCallback callback)
{
    _memoryWriteCallback = callback;
}

/**
 * Triggers the memory write event
 */
void Model1::triggerMemoryWriteEvent()
{
    if (_memoryWriteCallback)
    {
        EventData *data = _createEventData(EVENT_MEMORY_WRITE);
        _memoryWriteCallback(*data);
        free(data);
    }
}

// ----------------------------------------
// ---------- IO
// ----------------------------------------

/**
 * Reads from an IO address
 */
uint8_t Model1::readIO(uint8_t address)
{
    // Verification of access
    if (!_checkMutability())
        return 0;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Set address and then data
    _addressBus->writeIOAddress(address);

    // Timing of various signals
    pinWrite(IN, LOW);
    pinWrite(MUX, HIGH);
    pinWrite(CAS, LOW);

    // Read data
    uint8_t data = _dataBus->readData();

    // Reset, leaving address as-is
    pinWrite(IN, HIGH);
    pinWrite(MUX, LOW);
    pinWrite(CAS, HIGH);

    SREG = oldSREG;

    return data;
}

/**
 * Writes to an IO address
 */
void Model1::writeIO(uint8_t address, uint8_t data)
{
    // Verification of access
    if (!_checkMutability())
        return;

    uint8_t oldSREG = SREG;
    noInterrupts();

    // Configure bus
    _dataBus->setAsWritable();

    // Set address and then data
    _addressBus->writeMemoryAddress(address);
    _dataBus->writeData(data);

    // Timing of various signals
    pinWrite(OUT, LOW);
    pinWrite(MUX, HIGH);
    pinWrite(CAS, LOW);
    asmWait(1);

    // Reset, leving address as-is, removing data
    pinWrite(OUT, HIGH);
    pinWrite(MUX, LOW);
    pinWrite(CAS, HIGH);
    _dataBus->setAsReadable();

    SREG = oldSREG;
}

/**
 * Internal IO read interrupt handler
 */
void handleIORead()
{
    if (globalModel1)
        globalModel1->triggerIOReadEvent();
}

/**
 * Internal IO write interrupt handler
 */
void handleIOWrite()
{
    if (globalModel1)
        globalModel1->triggerIOWriteEvent();
}

/**
 * Sets up all the IO related interrupts
 */
void Model1::_setupIOInterrupts()
{
    attachInterrupt(digitalPinToInterrupt(PIN_IN), handleIORead, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_OUT), handleIOWrite, FALLING);
}

/**
 * Removes all the IO related interrupts
 */
void Model1::_removeIOInterrupts()
{
    detachInterrupt(digitalPinToInterrupt(PIN_IN));
    detachInterrupt(digitalPinToInterrupt(PIN_OUT));
}

/**
 * Sets a callback for IO read access
 */
void Model1::setIOReadCallback(EventIOReadCallback callback)
{
    _ioReadCallback = callback;
}

/**
 * Triggers the IO read event
 */
void Model1::triggerIOReadEvent()
{
    if (_ioReadCallback)
    {
        EventData *data = _createEventData(EVENT_IO_READ);
        _ioReadCallback(*data);
        free(data);
    }
}

/**
 * Sets a callback for IO write access
 */
void Model1::setIOWriteCallback(EventIOWriteCallback callback)
{
    _ioWriteCallback = callback;
}

/**
 * Triggers the IO write event
 */
void Model1::triggerIOWriteEvent()
{
    if (_ioWriteCallback)
    {
        EventData *data = _createEventData(EVENT_IO_WRITE);
        _ioWriteCallback(*data);
        free(data);
    }
}

// ----------------------------------------
// ---------- System Control Signals
// ----------------------------------------

/**
 * Initializes the system control signals
 *
 * These are read-only signals.
 */
void Model1::_initSystemControlSignals()
{
    pinConfigWrite(SYS_RES, INPUT);
    pinConfigWrite(INT_ACK, INPUT);
}

/**
 * Reads wether a system reset has occurred.
 */
bool Model1::readSystemResetSignal()
{
    return pinRead(SYS_RES) == LOW ? true : false;
}

/**
 * Reads wether the CPU has acknowledged an interrupt request.
 */
bool Model1::readInterruptAcknowledgeSignal()
{
    return pinRead(INT_ACK) == LOW ? true : false;
}

// ----------------------------------------
// ---------- Memory Control Signals
// ----------------------------------------

/**
 * Activates bus control signals
 */
void Model1::_activateBusControlSignals()
{
    _resetBusControlSignals();

    pinConfigWrite(RAS, OUTPUT);
    pinConfigWrite(MUX, OUTPUT);
    pinConfigWrite(CAS, OUTPUT);
}

/**
 * Deactivates bus control signals
 */
void Model1::_deactivateBusControlSignals()
{
    pinConfigWrite(RAS, INPUT);
    pinConfigWrite(MUX, INPUT);
    pinConfigWrite(CAS, INPUT);
}

/**
 * Resets the bus control signals to the default configuration
 */
void Model1::_resetBusControlSignals()
{
    pinWrite(RAS, HIGH);
    pinWrite(MUX, LOW);
    pinWrite(CAS, HIGH);
}

// ----------------------------------------
// ---------- Memory Access Signals
// ----------------------------------------

/**
 * Activates the bus access signals
 */
void Model1::_activateBusAccessSignals()
{
    _resetBusAccessSignals();

    pinConfigWrite(RD, OUTPUT);
    pinConfigWrite(WR, OUTPUT);

    pinConfigWrite(IN, OUTPUT);
    pinConfigWrite(OUT, OUTPUT);
}

/**
 * Deactivates the bus access signals
 */
void Model1::_deactivateBusAccessSignals()
{
    pinConfigWrite(RD, INPUT);
    pinConfigWrite(WR, INPUT);

    pinConfigWrite(IN, INPUT);
    pinConfigWrite(OUT, INPUT);
}

/**
 * Resetts the bus access signals to default
 */
void Model1::_resetBusAccessSignals()
{
    pinWrite(RD, HIGH);
    pinWrite(WR, HIGH);
    pinWrite(IN, HIGH);
    pinWrite(OUT, HIGH);
}

// ----------------------------------------
// ---------- External Control Signals
// ----------------------------------------

/**
 * Initializes the external control signals
 *
 * These are write-only signals.
 */
void Model1::_initExternalControlSignals()
{
    pinWrite(INT, HIGH);
    pinWrite(TEST, HIGH);
    pinWrite(WAIT, HIGH);

    pinConfigWrite(INT, OUTPUT);
    pinConfigWrite(TEST, OUTPUT);
    pinConfigWrite(WAIT, OUTPUT);
}

// ---------- Interrupt Request Signal

/**
 * Sets the interrupt request signal
 */
void Model1::_setInterruptRequestSignal(bool value)
{
    if (value)
    {
        pinWrite(INT, LOW);
    }
    else
    {
        pinWrite(INT, HIGH);
    }
}

/**
 * Triggers an interrupt within the Model 1
 *
 * NOTE: The timeout unit is in about microseconds.
 */
bool Model1::triggerInterrupt(uint8_t interrupt, uint16_t timeout = 1000)
{
    activateInterruptRequestSignal();

    for (uint16_t i = 0; i < timeout; i++)
    {
        if (pinRead(INT_ACK) == LOW)
        {
            _dataBus->setAsWritable();
            _dataBus->writeData(interrupt);

            asmWait(3);
            deactivateInterruptRequestSignal();
            asmWait(3);

            _dataBus->setAsReadable();

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
void Model1::activateInterruptRequestSignal()
{
    if (pinRead(INT) == LOW)
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
void Model1::deactivateInterruptRequestSignal()
{
    if (pinRead(INT) == HIGH)
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
void Model1::_setTestSignal(bool value)
{
    if (value)
    {
        pinWrite(TEST, LOW);
    }
    else
    {
        pinWrite(TEST, HIGH);
    }
}

/**
 * Activates the *TEST signal and let's this system take over.
 */
void Model1::activateTestSignal()
{
    if (pinRead(TEST) == LOW)
    {
        if (_logger)
            _logger->warn("TEST* signal already active.");
        return;
    }

    // Activate the TEST* signal
    _setTestSignal(true);

    // Set the signals as active from external system
    _addressBus->setAsWritable();
    _dataBus->setAsReadable();
    _activateBusControlSignals();
    _activateBusAccessSignals();

    // Mark bus as mutable, enabling write requests from this code
    _setMutable();

    // Activate background services
    _activateMemoryRefresh();
}

/**
 * Deactivates the *TEST signal and hands the system back to the CPU.
 */
void Model1::deactivateTestSignal()
{
    if (pinRead(TEST) == HIGH)
    {
        if (_logger)
            _logger->warn("TEST* signal already deactive.");
        return;
    }

    // Deactivate background services
    _deactivateMemoryRefresh();

    // Set bus as immutable, blocking write requests from this code
    _setImmutable();

    // Set the signals as inactive from external system
    _deactivateBusAccessSignals();
    _deactivateBusControlSignals();
    _dataBus->setAsReadable();
    _addressBus->setAsReadable();

    // Deactivate TEST* signal
    _setTestSignal(false);
}

// ---------- Wait Signal

/**
 * Sets the *WAIT signal to hold CPU
 */
void Model1::_setWaitSignal(bool value)
{
    if (value)
    {
        pinWrite(WAIT, LOW);
    }
    else
    {
        pinWrite(WAIT, HIGH);
    }
}

/**
 * Activates the *WAIT signal to hold the CPU
 */
void Model1::activateWaitSignal()
{
    if (pinRead(WAIT) == LOW)
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
void Model1::deactivateWaitSignal()
{
    if (pinRead(WAIT) == HIGH)
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
char *Model1::getState()
{
    char *addrStatus = _addressBus->getState();
    char *dataStatus = _dataBus->getState();

    const int LEN = 255;
    char *buffer = new char[LEN];
    snprintf(
        buffer,
        LEN,
        "Mut<%c>, RfshEn<%c>, RfshRow<%3.d>, RD<%c>(%d), WR<%c>(%d), IN<%c>(%d), OUT<%c>(%d), %s, %s, RAS<%c>(%d), CAS<%c>(%d), MUX<%c>(%d), SYS_RES<%c>(%d), INT_ACK<%c>(%d), INT<%c>(%d), TEST<%c>(%d), WAIT<%c>(%d)",

        _mutability ? 'T' : 'F',
        _activeRefresh ? 'T' : 'F',
        _nextMemoryRefreshRow,

        pinStatus(pinConfigRead(RD)),
        pinRead(RD),
        pinStatus(pinConfigRead(WR)), pinRead(WR),
        pinStatus(pinConfigRead(IN)), pinRead(IN),
        pinStatus(pinConfigRead(OUT)), pinRead(OUT),

        addrStatus,
        dataStatus,

        pinStatus(pinConfigRead(RAS)), pinRead(RAS),
        pinStatus(pinConfigRead(CAS)), pinRead(CAS),
        pinStatus(pinConfigRead(MUX)), pinRead(MUX),

        pinStatus(pinConfigRead(SYS_RES)), pinRead(SYS_RES),
        pinStatus(pinConfigRead(INT_ACK)), pinRead(INT_ACK),

        pinStatus(pinConfigRead(INT)), pinRead(INT),
        pinStatus(pinConfigRead(TEST)), pinRead(TEST),
        pinStatus(pinConfigRead(WAIT)), pinRead(WAIT));

    free(addrStatus);
    free(dataStatus);

    return buffer;
}

/**
 * Logs the current state
 */
void Model1::logState()
{
    if (_logger)
    {
        char *state = getState();
        _logger->info("State: %s", state);
        free(state);
    }
}

// ---------- Version

/**
 * Gets the major version number
 */
uint8_t Model1::getVersionMajor()
{
    return VERSION_MAJOR;
}

/**
 * Gets the minor version number
 */
uint8_t Model1::getVersionMinor()
{
    return VERSION_MINOR;
}

/**
 * Gets the revision version number
 */
uint8_t Model1::getVersionRevision()
{
    return VERSION_REVISION;
}

/**
 * Returns a version string
 */
char *Model1::getVersion()
{
    const int LEN = 255;
    char *buffer = new char[LEN];
    snprintf(
        buffer,
        LEN,
        "%d.%d.%d",
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_REVISION);
    return buffer;
}
