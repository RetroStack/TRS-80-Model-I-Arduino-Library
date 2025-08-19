#include "CassetteSongData.h"
#include <Arduino.h>

const int pacmanMelody[] PROGMEM = {
    NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5,
    NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5,
    NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,

    NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5,
    NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5,
    NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5};

const float pacmanDurations[] PROGMEM = {
    0.0625, 0.0625, 0.0625, 0.0625,
    0.03125, 0.0625, 0.125, 0.0625,
    0.0625, 0.0625, 0.0625, 0.03125, 0.0625, 0.125,

    0.0625, 0.0625, 0.0625, 0.0625, 0.03125,
    0.0625, 0.125, 0.03125, 0.03125, 0.03125,
    0.03125, 0.03125, 0.03125, 0.03125, 0.03125, 0.0625, 0.125};

const size_t pacmanMelodyLength = sizeof(pacmanMelody) / sizeof(int);
