#ifndef CASSETTESONGPLAYER_H
#define CASSETTESONGPLAYER_H

#include <MenuScreen.h>
#include <Cassette.h>

class CassetteSongPlayer : public MenuScreen
{
private:
    Cassette _cassette;
    bool _isPlaying;
    unsigned long _playStartTime;
    unsigned long _songDuration;
    int _currentSong;

    // Song data structures
    struct SongData
    {
        const char *name;
        int *melody;
        float *durations;
        size_t length;
        int tempo;
    };

    static const int MAX_SONGS = 8;
    SongData _songs[MAX_SONGS];

public:
    CassetteSongPlayer();
    void loop() override;
    Screen *actionTaken(ActionTaken action, uint8_t offsetX, uint8_t offsetY) override;

private:
    void initializeSongs();
    void playSong(int songIndex);
    void stopCurrentSong();
    void updateProgressBar();
    unsigned long calculateSongDuration(int songIndex);
};

// External song data declarations
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

#endif
