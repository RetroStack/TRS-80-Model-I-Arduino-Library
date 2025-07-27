#include "CassetteSongPlayer.h"
#include "CassetteSongData.h"
#include "MainDemoMenu.h"
#include <Arduino.h>

CassetteSongPlayer::CassetteSongPlayer() : MenuScreen()
{
    _setTitle("Song Player");

    _isPlaying = false;
    _currentSong = -1;
    _playStartTime = 0;
    _songDuration = 0;

    initializeSongs();

    // Add menu items for each song
    for (int i = 0; i < MAX_SONGS; i++)
    {
        if (_songs[i].name != nullptr)
        {
            addMenuItem(_songs[i].name);
        }
    }

    addMenuItem("Stop Playing");
    addMenuItem("Back to Main");

    Serial.println("Cassette Song Player initialized");
}

void CassetteSongPlayer::initializeSongs()
{
    _songs[0] = {"Imperial March", imperialMelody, imperialDurations, imperialMelodyLength, 120};
    _songs[1] = {"Tetris Theme", tetrisMelodyLead, tetrisDurationsLead, tetrisMelodyLength, 140};
    _songs[2] = {"Super Mario", marioMelody, marioDurations, marioMelodyLength, 180};
    _songs[3] = {"Pac-Man", pacmanMelody, pacmanDurations, pacmanMelodyLength, 120};

    // Set remaining slots to null
    for (int i = 4; i < MAX_SONGS; i++)
    {
        _songs[i] = {nullptr, nullptr, nullptr, 0, 0};
    }
}

void CassetteSongPlayer::loop()
{
    MenuScreen::loop();

    if (_isPlaying)
    {
        updateProgressBar();

        // Check if song finished
        unsigned long elapsed = millis() - _playStartTime;
        if (elapsed >= _songDuration)
        {
            stopCurrentSong();
        }
    }
}

void CassetteSongPlayer::playSong(int songIndex)
{
    if (songIndex >= 0 && songIndex < MAX_SONGS && _songs[songIndex].name != nullptr)
    {
        stopCurrentSong(); // Stop any currently playing song

        Serial.print("Playing song: ");
        Serial.println(_songs[songIndex].name);

        _currentSong = songIndex;
        _isPlaying = true;
        _playStartTime = millis();
        _songDuration = calculateSongDuration(songIndex);

        // Start playing the song
        _cassette.playSong(_songs[songIndex].melody,
                           _songs[songIndex].durations,
                           _songs[songIndex].length,
                           _songs[songIndex].tempo);

        // Update display to show playing status
        updateMenuItem(songIndex, String(_songs[songIndex].name) + " ♪");

        // Show progress bar
        drawProgressBar(0, 5);
    }
}

void CassetteSongPlayer::stopCurrentSong()
{
    if (_isPlaying && _currentSong >= 0)
    {
        Serial.println("Stopping current song");

        // Restore original song name
        updateMenuItem(_currentSong, _songs[_currentSong].name);

        _isPlaying = false;
        _currentSong = -1;

        // Clear progress bar
        clearProgressBar();
    }
}

void CassetteSongPlayer::updateProgressBar()
{
    if (_isPlaying && _songDuration > 0)
    {
        unsigned long elapsed = millis() - _playStartTime;
        int progress = (int)((elapsed * 100) / _songDuration);

        if (progress > 100)
            progress = 100;

        drawProgressBar(progress, 5);
    }
}

unsigned long CassetteSongPlayer::calculateSongDuration(int songIndex)
{
    if (songIndex < 0 || songIndex >= MAX_SONGS || _songs[songIndex].name == nullptr)
        return 0;

    unsigned long totalDuration = 0;
    int tempo = _songs[songIndex].tempo;

    for (size_t i = 0; i < _songs[songIndex].length; i++)
    {
        // Convert note duration to milliseconds
        // Duration is in beats, tempo is beats per minute
        float beatDuration = 60000.0 / tempo; // milliseconds per beat
        totalDuration += (unsigned long)(_songs[songIndex].durations[i] * beatDuration);
    }

    return totalDuration;
}

Screen *CassetteSongPlayer::actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY)
{
    if (action & BUTTON_JOYSTICK)
    {
        int selectedIndex = getSelectedIndex();

        // Check if it's a song selection
        if (selectedIndex < MAX_SONGS && _songs[selectedIndex].name != nullptr)
        {
            playSong(selectedIndex);
            return nullptr; // Stay on this screen
        }

        // Adjust index for non-song menu items
        int adjustedIndex = selectedIndex - MAX_SONGS;

        switch (adjustedIndex)
        {
        case 0: // Stop Playing
            stopCurrentSong();
            return nullptr;

        case 1: // Back to Main
            stopCurrentSong();
            Serial.println("Returning to main menu from Song Player");
            return new MainDemoMenu();

        default:
            return nullptr;
        }
    }

    if (action & BUTTON_MENU)
    {
        stopCurrentSong();
        Serial.println("Returning to main menu from Song Player");
        return new MainDemoMenu();
    }

    return MenuScreen::actionTaken(action, offsetX, offsetY);
}
