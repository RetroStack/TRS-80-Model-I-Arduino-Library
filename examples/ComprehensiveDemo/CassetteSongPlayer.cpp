#include "CassetteSongPlayer.h"
#include "CassetteSongData.h"
#include "CassetteMenu.h"
#include <Arduino.h>
#include "logger.h"

CassetteSongPlayer::CassetteSongPlayer() : MenuScreen()
{
    _setTitle("Song Player");

    // Create menu items dynamically - they'll be copied by _setMenuItems and these will be freed automatically
    const char *menuItems[] = {
        "Imperial March",
        "Tetris Theme",
        "Super Mario",
        "Pac-Man",
        "Underworld",
        "Simpsons Theme",
        "Game of Thrones",
        "Doom Theme"};

    _setMenuItems(menuItems, 8);

    // Set up logger for debugging
    _cassette.setLogger(logger);

    initializeSongs();

    Serial.println(F("Cassette Song Player initialized"));
}

void CassetteSongPlayer::initializeSongs()
{
    _songs[0] = {"Imperial March", imperialMelody, imperialDurations, imperialMelodyLength, 120};
    _songs[1] = {"Tetris Theme", tetrisMelodyLead, tetrisDurationsLead, tetrisMelodyLength, 140};
    _songs[2] = {"Super Mario", marioMelody, marioDurations, marioMelodyLength, 180};
    _songs[3] = {"Pac-Man", pacmanMelody, pacmanDurations, pacmanMelodyLength, 120};
    _songs[4] = {"Underworld", underworldMelody, underworldDurations, underworldMelodyLength, 150};
    _songs[5] = {"Simpsons Theme", simpsonsMelody, simpsonsDurations, simpsonsMelodyLength, 160};
    _songs[6] = {"Game of Thrones", gameOfThronesMelody, gameOfThronesDurations, gameOfThronesMelodyLength, 100};
    _songs[7] = {"Doom Theme", doomMelody, doomDurations, doomMelodyLength, 120};
}

void CassetteSongPlayer::playSong(int songIndex)
{
    if (songIndex >= 0 && songIndex < MAX_SONGS && _songs[songIndex].name != nullptr)
    {
        Serial.print(F("Playing song: "));
        Serial.println(_songs[songIndex].name);

        // Activate test signal for cassette operations
        Model1.deactivateMemoryRefresh();
        Model1.activateTestSignal();
        Serial.println(F("Test signal activated for song playback"));

        // Play the song using PROGMEM-aware function (this is blocking - will return when complete)
        _cassette.playSongPGM(_songs[songIndex].melody,
                              _songs[songIndex].durations,
                              _songs[songIndex].length,
                              _songs[songIndex].tempo);

        // Deactivate test signal after playback
        Model1.deactivateTestSignal();
        Model1.activateMemoryRefresh();
        Serial.println(F("Test signal deactivated after song playback"));

        Serial.print(_songs[songIndex].name);
        Serial.println(F(" playback complete"));
    }
}

Screen *CassetteSongPlayer::_getSelectedMenuItemScreen(int index)
{
    switch (index)
    {
    case 0: // Imperial March
        playSong(0);
        return nullptr; // Stay on this screen

    case 1: // Tetris Theme
        playSong(1);
        return nullptr;

    case 2: // Super Mario
        playSong(2);
        return nullptr;

    case 3: // Pac-Man
        playSong(3);
        return nullptr;

    case 4: // Underworld
        playSong(4);
        return nullptr;

    case 5: // Simpsons Theme
        playSong(5);
        return nullptr;

    case 6: // Game of Thrones
        playSong(6);
        return nullptr;

    case 7: // Doom Theme
        playSong(7);
        return nullptr;

    case -1: // Back to Main
        Serial.println(F("Returning to cassette menu from Song Player"));
        return new CassetteMenu();

    default:
        return nullptr;
    }
}
