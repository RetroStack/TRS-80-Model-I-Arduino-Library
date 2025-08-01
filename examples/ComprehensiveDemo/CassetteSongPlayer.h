#ifndef CASSETTESONGPLAYER_H
#define CASSETTESONGPLAYER_H

#include <MenuScreen.h>
#include <Cassette.h>

class CassetteSongPlayer : public MenuScreen
{
private:
    Cassette _cassette;

    // Song data structures
    struct SongData
    {
        const char *name;
        const int *melody;
        const float *durations;
        size_t length;
        int tempo;
    };

    static const int MAX_SONGS = 8;
    SongData _songs[MAX_SONGS];

public:
    CassetteSongPlayer();

protected:
    Screen *_getSelectedMenuItemScreen(int index) override;

private:
    void initializeSongs();
    void playSong(int songIndex);
};

// External song data declarations
extern const int imperialMelody[];
extern const float imperialDurations[];
extern const size_t imperialMelodyLength;

extern const int tetrisMelodyLead[];
extern const float tetrisDurationsLead[];
extern const size_t tetrisMelodyLength;

extern const int marioMelody[];
extern const float marioDurations[];
extern const size_t marioMelodyLength;

extern const int underworldMelody[];
extern const float underworldDurations[];
extern const size_t underworldMelodyLength;

extern const int simpsonsMelody[];
extern const float simpsonsDurations[];
extern const size_t simpsonsMelodyLength;

extern const int gameOfThronesMelody[];
extern const float gameOfThronesDurations[];
extern const size_t gameOfThronesMelodyLength;

extern const int doomMelody[];
extern const float doomDurations[];
extern const size_t doomMelodyLength;

extern const int pacmanMelody[];
extern const float pacmanDurations[];
extern const size_t pacmanMelodyLength;

#endif
