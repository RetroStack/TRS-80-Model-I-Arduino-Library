# Cassette Class

The `Cassette` class provides control over the TRS-80 Model I cassette interface, reading bits from the input, allowing you to play audio signals to the cassette port, control the cassette motor relay, and switch video character modes. This class builds on the `Model1` class to issue I/O commands.

## Table of Contents

- [Constructor](#constructor)
- [Setters](#setters)

  - [setLogger](#void-setlogger-ilogger-logger)

- [Methods](#methods)

  - [update](#void-update)
  - [writeRaw](#void-writerawbool-bool)
  - [readRaw](#bool-readraw)
  - [play](#void-playuint16_t-uint32_t)
  - [playSong](#void-playsongint-float-size_t-int)
  - [playSongPGM](#void-playsongpgmconst-int-const-float-size_t-int)
  - [activateRemote](#void-activateremote)
  - [deactivateRemote](#void-deactivateremote)

- [Video Methods](#video-methods)
  - [setCharGenA](#void-setchargena)
  - [setCharGenB](#void-setchargenb)
  - [is64CharacterMode](#bool-is64charactermode)
  - [set32CharacterMode](#void-set32charactermode)
  - [set64CharacterMode](#void-set64charactermode)
- [Notes](#notes)
- [Example](#example)

## Constructor

```cpp
Cassette()
```

## Setters

### `void setLogger(ILogger &logger)`

Sets the logger used for errors and warnings.

_This is often useful for debugging as it tells what went wrong._

## Methods

### `void update()`

Synchronizes the internal cassette state with the hardware I/O port.

_This should be done ones for initialization or whenever a change is expected._

### `void writeRaw(bool value1, bool value2)`

Sets the two cassette audio output bits directly.

Here a list of output values you can get at the cassette output:

- `value1=false`, `value2=false` => ~0.46V
- `value1=false`, `value2=true` => ~0V
- `value1=true`, `value2=false` => ~0.85V
- `value1=true`, `value2=true` => ~0V

### `bool readRaw()`

Reads a raw bit from the cassette input port.

**Returns:** `true` if the input signal is high, `false` if low.

_This method allows direct reading of the cassette input signal for custom tape decoding routines._

### `void play(uint16_t frequency, uint32_t durationMs)`

Plays a tone at the specified frequency and duration.

### `void playSong(int* melody, float* durations, size_t numNotes, int bpm)`

Plays a melody sequence given note frequencies and note durations.

**Parameters:**

- `melody`: Array of note frequencies (use NOTE\_\* constants)
- `durations`: Array of note durations as fractions (0.25 = quarter note, 0.5 = half note, etc.)
- `numNotes`: Number of notes in the melody
- `bpm`: Beats per minute for tempo control

### `void playSongPGM(const int* melody, const float* durations, size_t numNotes, int bpm)`

Plays a melody sequence stored in program memory (PROGMEM) to save RAM.

**Parameters:**

- `melody`: Array of note frequencies stored in PROGMEM (use NOTE\_\* constants)
- `durations`: Array of note durations stored in PROGMEM as fractions
- `numNotes`: Number of notes in the melody
- `bpm`: Beats per minute for tempo control

_Use this method when storing large melodies to conserve RAM by keeping the data in program memory._

### `void activateRemote()`

Activates the cassette motor relay to start tape movement.

### `void deactivateRemote()`

Deactivates the cassette motor relay.

## Video Methods

The cassette interface also includes video-related methods to control display modes and select character generators. Tandy designed this I/O port to reuse existing flip-flop circuits instead of dedicating a separate port, combining cassette and video functions in a single register.

### `void setCharGenA()`

Selects Character Generator A. Requires hardware jumpers configured accordingly.

_This only works on the Japanese (TEC) versions of the Model 1._

### `void setCharGenB()`

Selects Character Generator B. Requires hardware jumpers configured accordingly.

_This only works on the Japanese (TEC) versions of the Model 1._

### `bool is64CharacterMode()`

Returns `true` if the video is in 64-character mode.

### `void set32CharacterMode()`

Switches video mode to 32 characters per line.

### `void set64CharacterMode()`

Switches video mode to 64 characters per line. (default)

## Notes

- You must call `model1.activateTestSignal()` before using cassette operations.
- Some methods require correct hardware configuration (jumpers) to take effect.

## Example

```cpp
#include <Model1.h>
#include <Cassette.h>

Cassette cassette;

void setup() {
  Serial.begin(115200);

  Model1.begin();

  Model1.activateTestSignal();

  // Play a 1 kHz tone for 2 seconds
  cassette.play(1000, 2000);

  // Switch to 32-character mode
  cassette.set32CharacterMode();

  Model1.deactivateTestSignal();
}

void loop() {
}
```

## Note Constants

A set of note frequency constants is also available for use with `play` and `playSong`, for example `NOTE_C4`, `NOTE_A4`, `NOTE_G5`, etc. These constants cover notes from B0 (31 Hz) up to DS8 (4978 Hz).

These constants are defined as macros mapping note names to their frequencies in Hertz. You can use them like this:

```cpp
cassette.play(NOTE_A4, 500);
```

## More Examples

Here are more specific examples:

```cpp
// Play middle C for 1 second
cassette.play(NOTE_C4, 1000);

// Play a short beep at 880 Hz (A5) for 200 ms
cassette.play(NOTE_A5, 200);

// Play a melody
int melody[] = { NOTE_E4, NOTE_D4, NOTE_C4 };
float durations[] = { 0.25, 0.25, 0.5 };
cassette.playSong(melody, durations, 3, 120);

// Read cassette input
bool inputSignal = cassette.readRaw();
if (inputSignal) {
  Serial.println("Cassette input is HIGH");
}

// Play a melody from PROGMEM to save RAM
const int PROGMEM pgmMelody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5 };
const float PROGMEM pgmDurations[] = { 0.25, 0.25, 0.25, 0.5 };
cassette.playSongPGM(pgmMelody, pgmDurations, 4, 100);
```

## Full List of Note Constants

| Note     | Frequency (Hz) |
| -------- | -------------- |
| NOTE_B0  | 31             |
| NOTE_C1  | 33             |
| NOTE_CS1 | 35             |
| NOTE_D1  | 37             |
| NOTE_DS1 | 39             |
| NOTE_E1  | 41             |
| NOTE_F1  | 44             |
| NOTE_FS1 | 46             |
| NOTE_G1  | 49             |
| NOTE_GS1 | 52             |
| NOTE_A1  | 55             |
| NOTE_AS1 | 58             |
| NOTE_B1  | 62             |
| NOTE_C2  | 65             |
| NOTE_CS2 | 69             |
| NOTE_D2  | 73             |
| NOTE_DS2 | 78             |
| NOTE_E2  | 82             |
| NOTE_F2  | 87             |
| NOTE_FS2 | 93             |
| NOTE_G2  | 98             |
| NOTE_GS2 | 104            |
| NOTE_A2  | 110            |
| NOTE_AS2 | 117            |
| NOTE_B2  | 123            |
| NOTE_C3  | 131            |
| NOTE_CS3 | 139            |
| NOTE_D3  | 147            |
| NOTE_DS3 | 156            |
| NOTE_E3  | 165            |
| NOTE_F3  | 175            |
| NOTE_FS3 | 185            |
| NOTE_G3  | 196            |
| NOTE_GS3 | 208            |
| NOTE_A3  | 220            |
| NOTE_AS3 | 233            |
| NOTE_B3  | 247            |
| NOTE_C4  | 262            |
| NOTE_CS4 | 277            |
| NOTE_D4  | 294            |
| NOTE_DS4 | 311            |
| NOTE_E4  | 330            |
| NOTE_F4  | 349            |
| NOTE_FS4 | 370            |
| NOTE_G4  | 392            |
| NOTE_GS4 | 415            |
| NOTE_A4  | 440            |
| NOTE_AS4 | 466            |
| NOTE_B4  | 494            |
| NOTE_C5  | 523            |
| NOTE_CS5 | 554            |
| NOTE_D5  | 587            |
| NOTE_DS5 | 622            |
| NOTE_E5  | 659            |
| NOTE_F5  | 698            |
| NOTE_FS5 | 740            |
| NOTE_G5  | 784            |
| NOTE_GS5 | 831            |
| NOTE_A5  | 880            |
| NOTE_AS5 | 932            |
| NOTE_B5  | 988            |
| NOTE_C6  | 1047           |
| NOTE_CS6 | 1109           |
| NOTE_D6  | 1175           |
| NOTE_DS6 | 1245           |
| NOTE_E6  | 1319           |
| NOTE_F6  | 1397           |
| NOTE_FS6 | 1480           |
| NOTE_G6  | 1568           |
| NOTE_GS6 | 1661           |
| NOTE_A6  | 1760           |
| NOTE_AS6 | 1865           |
| NOTE_B6  | 1976           |
| NOTE_C7  | 2093           |
| NOTE_CS7 | 2217           |
| NOTE_D7  | 2349           |
| NOTE_DS7 | 2489           |
| NOTE_E7  | 2637           |
| NOTE_F7  | 2794           |
| NOTE_FS7 | 2960           |
| NOTE_G7  | 3136           |
| NOTE_GS7 | 3322           |
| NOTE_A7  | 3520           |
| NOTE_AS7 | 3729           |
| NOTE_B7  | 3951           |
| NOTE_C8  | 4186           |
| NOTE_CS8 | 4435           |
| NOTE_D8  | 4699           |
| NOTE_DS8 | 4978           |
