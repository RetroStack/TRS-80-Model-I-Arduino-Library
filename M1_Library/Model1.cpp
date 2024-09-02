// Main class for Test Harness library

#include "./Model1.h"

Model1::Model1(ILogger *logger)
{
  _logger = logger;

  _keyboard = new Keyboard(logger, this);
  _rom = new ROM(logger, this);
  _video = new Video(logger, this);

  _logger->info("Initalizing shield and entering TEST mode.");

  initControlPins();

  setAddressLinesToInput();
  setDataLinesToInput();
}

Keyboard *Model1::getKeyboard()
{
  return _keyboard;
}
ROM *Model1::getROM()
{
  return _rom;
}
Video *Model1::getVideo()
{
  return _video;
}

// Display status of control pins. WARNING - set all control pins to input, excluding TEST*
void Model1::displayCtrlPinStatus()
{
  const int pins[] = {BUSAK_L, CAS_L, CR1, CR2, IN_L, INTAK_L, INT_L, MUX_L, OUT_L, RAS_L, RD_L, RST_L, VID, WAIT_L, WR_L, CASS_IN, CASS_OUT, VID};
  const char *pinNames[] = {"BUSAK_L", "CAS_L", "CR1", "CR2", "IN_L", "INTAK_L", "INT_L", "MUX_L", "OUT_L", "RAS_L", "RD_L", "RST_L", "WAIT_L", "WR_L", "CASS_IN", "CASS_OUT", "VID"};
  const int pinCount = sizeof(pins) / sizeof(pins[0]);

  _logger->info("Displaying pin statuses.");

  for (int i = 0; i < pinCount - 1; ++i)
  {
    pinMode(pins[i], INPUT); // Set the pin mode to input
    int state = digitalRead(pins[i]);
    _logger->info("%s: %s", pinNames[i], state == HIGH ? "HIGH" : "LOW");
  }
}

// Init all ATMega pins, except for TEST_L pin as it should be set at start of program
// and this will allow for 'resetting' the pins without leaving TEST mode
void Model1::initControlPins()
{
  _logger->info("Settting all PINS except TEST* to INPUT.");
  pinMode(BUSAK_L, INPUT);
  pinMode(CAS_L, INPUT);
  pinMode(CR1, INPUT);
  pinMode(CR2, INPUT);
  pinMode(IN_L, INPUT);
  pinMode(INTAK_L, INPUT);
  pinMode(INT_L, INPUT);
  pinMode(MUX_L, INPUT);
  pinMode(OUT_L, INPUT);
  pinMode(RAS_L, INPUT);
  pinMode(RD_L, INPUT);
  pinMode(RST_L, INPUT);
  pinMode(WAIT_L, INPUT);
  pinMode(WR_L, INPUT);
}

// Set ATMega pins and ports to input, except for TEST*
void Model1::setAllPinsPortsToInput()
{
  _logger->info("Settting all PINS and PORTS to INPUT.");
  initControlPins();
  setAddressLinesToInput();
  setDataLinesToInput();
}

// Set ATMega pins mapped to bus address lines to INPUT
void Model1::setAddressLinesToInput()
{
  DDRA = 0x00; // lower 8
  DDRC = 0x00; // upper 8
}

// Set ATMega pins mapped to bus address lines to OUTPUT
void Model1::setAddressLinesToOutput(uint16_t memAddress)
{
  DDRA = 0xFF; // lower 8
  DDRC = 0xFF; // upper 8

  PORTA = (uint8_t)(memAddress & 0xFF);
  PORTC = (uint8_t)((memAddress & 0xFF00) >> 8);
}

// Set ATMega pins mapped to bus data lines to INPUT
void Model1::setDataLinesToInput()
{
  DDRF = 0x00; // 8 bits
}

// Set ATMega pins mapped to bus data lines to OUTPUT
void Model1::setDataLinesToOutput()
{
  DDRF = 0xFF; // 8 bits
}

// Set bus TEST* pin to active low or to high
void Model1::setTESTPin(int state)
{
  if (state == 0)
  {
    _logger->info("Setting TEST* to LOW (active).");
    pinMode(TEST_L, OUTPUT);
    digitalWrite(TEST_L, LOW);
  }
  else
  {
    _logger->info("Setting TEST* to HIGH (inactive).");
    pinMode(TEST_L, OUTPUT);
    digitalWrite(TEST_L, HIGH);
    pinMode(TEST_L, INPUT);
  }

  // A longer wait is needed since the M1 bus does not expose the BUSAK* line
  // this may need further tweaking.
  asmWait(20);
}

// Set RAS*, RD* and WR* pins to HIGH
void Model1::turnOffReadWriteRASLines()
{
  pinMode(WR_L, OUTPUT); // turn off write asap
  digitalWrite(WR_L, HIGH);
  pinMode(WR_L, INPUT);

  pinMode(RD_L, OUTPUT);
  digitalWrite(RD_L, HIGH);
  pinMode(RD_L, INPUT);

  pinMode(RAS_L, OUTPUT);
  digitalWrite(RAS_L, HIGH);
  pinMode(RAS_L, INPUT);

  asmWait(3);
}
