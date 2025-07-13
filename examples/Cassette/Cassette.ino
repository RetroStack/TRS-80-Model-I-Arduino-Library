#include <Arduino.h>
#include <Model1.h>
#include <Cassette.h>

Model1 *model1;
Cassette *cassette;

extern int imperialMelody[];
extern float imperialDurations[];
extern const size_t imperialMelodyLength;

extern int tetrisMelodyLead[];
extern float tetrisDurationsLead[];
extern const size_t tetrisMelodyLength;

extern int marioMelody[];
extern float marioDurations[];
extern const size_t marioMelodyLength;

extern int underworldMelody[];
extern float underworldDurations[];
extern const size_t underworldMelodyLength;

extern int simpsonsMelody[];
extern float simpsonsDurations[];
extern const size_t simpsonsMelodyLength;

extern int gameOfThronesMelody[];
extern float gameOfThronesDurations[];
extern const size_t gameOfThronesMelodyLength;

extern int doomMelody[];
extern float doomDurations[];
extern const size_t doomMelodyLength;

extern int pacmanMelody[];
extern float pacmanDurations[];
extern const size_t pacmanMelodyLength;

void setup()
{
    // Initialize a Model 1 instance to access it
    model1 = new Model1();

    // Be sure to setup the hardware pins
    model1->begin();

    // Create an instance of the Cassette subsystem, supplying the Model 1 connection
    cassette = new Cassette(model1);

    // Activate the test signal for the Arduino to take over the Model 1 system.
    // You need to activate it, otherwise the Z80 will continue to be in control.
    // The Arduino will also be blocked from accessing the bus without activation.
    model1->activateTestSignal();
}

void loop()
{
    // Play various songs
    // Comment the ones you don't want out

    cassette->playSong(imperialMelody, imperialDurations, imperialMelodyLength, 120);
    delay(2000);

    cassette->playSong(tetrisMelodyLead, tetrisDurationsLead, tetrisMelodyLength, 140);
    delay(2000);

    cassette->playSong(marioMelody, marioDurations, marioMelodyLength, 180);
    delay(2000);

    cassette->playSong(underworldMelody, underworldDurations, underworldMelodyLength, 180);
    delay(2000);

    cassette->playSong(simpsonsMelody, simpsonsDurations, simpsonsMelodyLength, 160);
    delay(2000);

    cassette->playSong(gameOfThronesMelody, gameOfThronesDurations, gameOfThronesMelodyLength, 160);
    delay(2000);

    cassette->playSong(doomMelody, doomDurations, doomMelodyLength, 140);
    delay(2000);

    cassette->playSong(pacmanMelody, pacmanDurations, pacmanMelodyLength, 120);
    delay(2000);

    // Let's have a break!
    delay(5000);

    // Sets video in 32 character mode
    cassette->set32CharacterMode();
    delay(2000);

    // Sets video in 64 character mode
    cassette->set64CharacterMode();
    delay(2000);

    // You can also check wether it is in this mode
    bool is64Mode = cassette->is64CharacterMode();

    // Activates the remote driver
    cassette->activateRemote();
    delay(2000);

    // Deactivates the remote driver
    cassette->deactivateRemote();
    delay(2000);
}

int imperialMelody[] = {
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, REST,
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, REST,
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5,

    NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5,
    NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,

    NOTE_A5, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_GS5, NOTE_G5,
    NOTE_DS5, NOTE_D5, NOTE_DS5, REST, NOTE_A4, NOTE_DS5, NOTE_D5, NOTE_CS5,

    NOTE_C5, NOTE_B4, NOTE_C5, REST, NOTE_F4, NOTE_GS4, NOTE_F4, NOTE_A4,
    NOTE_C5, NOTE_A4, NOTE_C5, NOTE_E5,

    NOTE_A5, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_GS5, NOTE_G5,
    NOTE_DS5, NOTE_D5, NOTE_DS5, REST, NOTE_A4, NOTE_DS5, NOTE_D5, NOTE_CS5,

    NOTE_C5, NOTE_B4, NOTE_C5, REST, NOTE_F4, NOTE_GS4, NOTE_F4, NOTE_A4,
    NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4};

float imperialDurations[] = {
    0.375, 0.375, 0.0625, 0.0625, 0.0625, 0.0625, 0.125, 0.125,
    0.375, 0.375, 0.0625, 0.0625, 0.0625, 0.0625, 0.125, 0.125,
    0.25, 0.25, 0.25, 0.1875, 0.0625,

    0.25, 0.1875, 0.0625, 0.5,
    0.25, 0.25, 0.25, 0.1875, 0.0625,
    0.25, 0.1875, 0.0625, 0.5,

    0.25, 0.1875, 0.0625, 0.25, 0.1875, 0.0625,
    0.0625, 0.0625, 0.125, 0.125, 0.125, 0.25, 0.1875, 0.0625,

    0.0625, 0.0625, 0.0625, 0.125, 0.125, 0.25, 0.1875, 0.09375,
    0.25, 0.1875, 0.0625, 0.5,

    0.25, 0.1875, 0.0625, 0.25, 0.1875, 0.0625,
    0.0625, 0.0625, 0.125, 0.125, 0.125, 0.25, 0.1875, 0.0625,

    0.0625, 0.0625, 0.0625, 0.125, 0.125, 0.25, 0.1875, 0.09375,
    0.25, 0.1875, 0.0625, 0.5};

const size_t imperialMelodyLength = sizeof(imperialMelody) / sizeof(int);

int tetrisMelodyLead[] = {
    NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4,
    NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4, REST,

    NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_E5,
    NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4, REST,

    NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_C4, NOTE_A3, NOTE_GS3, NOTE_B3,
    NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_A4,
    NOTE_GS4, REST};

float tetrisDurationsLead[] = {
    // part 1
    0.25, 0.125, 0.125, 0.25, 0.125, 0.125, 0.25, 0.125,
    0.125, 0.25, 0.125, 0.125, 0.25, 0.125, 0.125, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25,

    0.375, 0.125, 0.25, 0.125, 0.125, 0.375, 0.125, 0.25,
    0.125, 0.125, 0.25, 0.125, 0.125, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25,

    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.25, 0.25,
    0.75, 0.25};

const size_t tetrisMelodyLength = sizeof(tetrisMelodyLead) / sizeof(int);

int marioMelody[] = {
    NOTE_E5, NOTE_E5, REST, NOTE_E5,
    REST, NOTE_C5, NOTE_E5, REST,
    NOTE_G5, REST, REST, REST,
    NOTE_G4, REST, REST, REST,

    NOTE_C5, REST, REST, NOTE_G4,
    REST, REST, NOTE_E4, REST,
    REST, NOTE_A4, REST, NOTE_B4,
    REST, NOTE_AS4, NOTE_A4, REST,

    NOTE_G4, NOTE_E5, NOTE_G5,
    NOTE_A5, REST, NOTE_F5, NOTE_G5,
    REST, NOTE_E5, REST, NOTE_C5,
    NOTE_D5, NOTE_B4, REST, REST,

    NOTE_C5, REST, REST, NOTE_G4,
    REST, REST, NOTE_E4, REST,
    REST, NOTE_A4, REST, NOTE_B4,
    REST, NOTE_AS4, NOTE_A4, REST,

    NOTE_G4, NOTE_E5, NOTE_G5,
    NOTE_A5, REST, NOTE_F5, NOTE_G5,
    REST, NOTE_E5, REST, NOTE_C5,
    NOTE_D5, NOTE_B4, REST, REST};

float marioDurations[] = {
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,

    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,

    0.1111111, 0.1111111, 0.1111111,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,

    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,

    0.1111111, 0.1111111, 0.1111111,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333};

const size_t marioMelodyLength = sizeof(marioMelody) / sizeof(int);

int underworldMelody[] = {
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
    NOTE_AS3, NOTE_AS4, REST,
    REST,
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
    NOTE_AS3, NOTE_AS4, REST,
    REST,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
    NOTE_DS3, NOTE_DS4, REST,
    REST,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
    NOTE_DS3, NOTE_DS4, REST,
    REST, NOTE_DS4, NOTE_CS4, NOTE_D4,
    NOTE_CS4, NOTE_DS4,
    NOTE_DS4, NOTE_GS3,
    NOTE_G3, NOTE_CS4,
    NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
    NOTE_GS4, NOTE_DS4, NOTE_B3,
    NOTE_AS3, NOTE_A3, NOTE_GS3,
    REST, REST, REST};

float underworldDurations[] = {
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.1666667,
    0.3333333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.1666667,
    0.3333333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.1666667,
    0.3333333,
    0.0833333, 0.0833333, 0.0833333, 0.0833333,
    0.0833333, 0.0833333, 0.1666667,
    0.1666667, 0.0555556, 0.0555556, 0.0555556,
    0.1666667, 0.1666667,
    0.1666667, 0.1666667,
    0.1666667, 0.1666667,
    0.0555556, 0.0555556, 0.0555556, 0.0555556, 0.0555556, 0.0555556,
    0.1, 0.1, 0.1,
    0.1, 0.1, 0.1,
    0.3333333, 0.3333333, 0.3333333};

const size_t underworldMelodyLength = sizeof(underworldMelody) / sizeof(int);

int simpsonsMelody[] = {
    NOTE_C4,
    NOTE_E4,
    NOTE_FS4,
    NOTE_A4,
    NOTE_G4,
    NOTE_E4,
    NOTE_C4,
    NOTE_G3,
    NOTE_FS3,
    NOTE_FS3,
    NOTE_FS3,
    NOTE_G3};

float simpsonsDurations[] = {
    0.5,   // C4 (half)
    0.25,  // E4 (quarter)
    0.25,  // FS4 (quarter)
    0.125, // A4 (eighth)
    0.5,   // G4 (half)
    0.25,  // E4 (quarter)
    0.25,  // C4 (quarter)
    0.25,  // G3 (quarter)
    0.125, // FS3 (eighth)
    0.125, // FS3 (eighth)
    0.125, // FS3 (eighth)
    0.25   // G3 (quarter)
};

const size_t simpsonsMelodyLength = sizeof(simpsonsMelody) / sizeof(int);

int gameOfThronesMelody[] = {
    NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
    NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
    NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
    NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
    NOTE_G4, NOTE_C4,

    NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
    NOTE_D4,
    NOTE_F4, NOTE_AS3,
    NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
    NOTE_DS4, NOTE_D4, NOTE_C4,

    NOTE_G4, NOTE_C4,

    NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
    NOTE_D4,
    NOTE_F4, NOTE_AS3,
    NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
    NOTE_DS4, NOTE_D4, NOTE_C4,
    NOTE_G4, NOTE_C4,
    NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,

    NOTE_D4,
    NOTE_F4, NOTE_AS3,
    NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_AS3,
    NOTE_C4,
    NOTE_C5,
    NOTE_AS4,
    NOTE_C4,
    NOTE_G4,
    NOTE_DS4,
    NOTE_DS4, NOTE_F4,
    NOTE_G4,

    NOTE_C5,
    NOTE_AS4,
    NOTE_C4,
    NOTE_G4,
    NOTE_DS4,
    NOTE_DS4, NOTE_D4,
    NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
    NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,

    REST, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5,
    NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5};

float gameOfThronesDurations[] = {
    0.125, 0.125, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,
    0.125, 0.125, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,
    0.125, 0.125, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,
    0.125, 0.125, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,
    0.25, 0.25,

    0.0625, 0.0625, 0.25, 0.25, 0.0625, 0.0625,
    1.0,
    0.25, 0.25,
    0.0625, 0.0625, 0.25, 0.25,
    0.0625, 0.0625, 1.0,

    0.25, 0.25,

    0.0625, 0.0625, 0.25, 0.25, 0.0625, 0.0625,
    1.0,
    0.25, 0.25,
    0.0625, 0.0625, 0.25, 0.25,
    0.0625, 0.0625, 1.0,
    0.25, 0.25,
    0.0625, 0.0625, 0.25, 0.25, 0.0625, 0.0625,

    0.5,
    0.25, 0.25,
    0.125, 0.125, 0.125, 0.125,
    1.0,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.25, 0.25,
    1.0,

    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.25, 0.25,
    0.125, 0.125, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,
    0.125, 0.125, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,

    0.25, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625,
    0.125, 0.0625, 0.0625, 0.0625, 0.125, 0.125, 0.0625, 0.0625};

const size_t gameOfThronesMelodyLength = sizeof(gameOfThronesMelody) / sizeof(int);

int doomMelody[] = {
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_FS3, NOTE_D3, NOTE_B2, NOTE_A3, NOTE_FS3, NOTE_B2, NOTE_D3, NOTE_FS3, NOTE_A3, NOTE_FS3, NOTE_D3, NOTE_B2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_B3, NOTE_G4, NOTE_B4,

    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3,

    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_A3, NOTE_F3, NOTE_D3, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_A3, NOTE_F3, NOTE_D3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_CS3, NOTE_CS3, NOTE_CS4, NOTE_CS3, NOTE_CS3, NOTE_B3, NOTE_CS3, NOTE_CS3,
    NOTE_A3, NOTE_CS3, NOTE_CS3, NOTE_G3, NOTE_CS3, NOTE_CS3, NOTE_GS3, NOTE_A3,
    NOTE_B2, NOTE_B2, NOTE_B3, NOTE_B2, NOTE_B2, NOTE_A3, NOTE_B2, NOTE_B2,
    NOTE_G3, NOTE_B2, NOTE_B2, NOTE_F3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_B3, NOTE_G4, NOTE_B4,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_FS3, NOTE_DS3, NOTE_B2, NOTE_FS3, NOTE_DS3, NOTE_B2, NOTE_G3, NOTE_D3, NOTE_B2, NOTE_DS4, NOTE_DS3, NOTE_B2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_E4, NOTE_B3, NOTE_G3, NOTE_G4, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_G3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_A3, NOTE_F3, NOTE_D3, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_A3, NOTE_F3, NOTE_D3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_B3, NOTE_G3, NOTE_E3, NOTE_B2, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_B3, NOTE_G3, NOTE_B3, NOTE_G3, NOTE_E3};

float doomDurations[] = {
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625,

    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125,
    0.125, 0.125, 0.125, 0.5};

const size_t doomMelodyLength = sizeof(doomMelody) / sizeof(int);

int pacmanMelody[] = {
    NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5,
    NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5,
    NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,

    NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5,
    NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5,
    NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5};

float pacmanDurations[] = {
    0.0625, 0.0625, 0.0625, 0.0625,
    0.03125, 0.0625, 0.125, 0.0625,
    0.0625, 0.0625, 0.0625, 0.03125, 0.0625, 0.125,

    0.0625, 0.0625, 0.0625, 0.0625, 0.03125,
    0.0625, 0.125, 0.03125, 0.03125, 0.03125,
    0.03125, 0.03125, 0.03125, 0.03125, 0.03125, 0.0625, 0.125};

const size_t pacmanMelodyLength = sizeof(pacmanMelody) / sizeof(int);
