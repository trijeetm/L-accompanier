//-----------------------------------------------------------------------------
// name: dm-audio.h
// desc: audio stuff
//
// author: Trijeet Mukhopadhyay (trijeetm@stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __DM_GLOBALS_H__
#define __DM_GLOBALS_H__

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "x-audio.h"
#include "y-entity.h"
#include "bk-sim.h"
// #include "RtAudio.h"

using namespace std;

// defines
#define DM_SRATE        44100
#define DM_FRAMESIZE    512
#define DM_NUMCHANNELS  2
#define DM_MAX_TEXTURES 32
#define DM_N_GRID_ELEMENTS 16
#define DM_POLYPHONY    8
#define DM_DRUMS        0
#define DM_PIANOS       1
#define DM_SYNTH        2
#define DM_BASS         0
#define DM_SNARE        1
#define DM_HIHATS       2
#define C_INPUT         0
#define C_METRONOME     1
#define C_DRUMMER       2
#define C_BASSIST       3
#define MIDI_IN         0
#define MIDI_THRU       1
#define TR_INVALID      -1
#define TR_MAJOR        0
#define TR_MINOR        1

struct triad {
    int type;
    int first, third, fifth;

    triad() {
        type = first = third = fifth = TR_INVALID;
    }

    triad(int typ, int f, int s, int t) {
        type = typ;
        first = f;
        third = s;
        fifth = t;
    }
};

struct note {
    bool on;
    int channel;
    int patch;
    float pitch;
    float duration;
    float velocity;

    note() {
        on = 0;
        channel = 0;
        patch = 0;
        pitch = 0;
        duration = 0;
        velocity = 0;
    }

    note(bool state, int c, int pat, float p, float d, float v) {
        on = state;
        channel = c;
        patch = pat;
        pitch = p;
        duration = d;
        velocity = v;
    }
};

struct noteStack {
    note notes[DM_POLYPHONY];
    int totalPatches;

    noteStack() {
        totalPatches = 0;
        for (int i = 0; i < DM_POLYPHONY; i++) {
            notes[i].on = false;
            notes[i].channel = 1;
            notes[i].patch = i;
            notes[i].pitch = 0;
            notes[i].duration = 0;
            notes[i].velocity = 0;
        }
    }
};

struct timeSignature {
    int beatUnit;
    int beatCount;

    timeSignature() {
        beatUnit = 4;
        beatCount = 4;
    }
};

//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals {
public:
    // top level root simulation
    static BKSim * sim;

    static int bpm;
    static timeSignature tSign;

    static SAMPLE now;
    static SAMPLE accumulator;
    static SAMPLE tempoPeriod;

    static noteStack drumGrid[DM_N_GRID_ELEMENTS];
    static noteStack pianoGrid[DM_N_GRID_ELEMENTS];
    static noteStack synthGrid[DM_N_GRID_ELEMENTS];
    static int activeSynth;

    static int playhead;

    static int currentPatch;

    static int currentPianoPatch;
    static int currentPianoNote;

    static int currentSynthPatch;
    static int currentSynthNote;

    static vector< vector< unsigned char > > midiBuffer;
    static int midiPlayhead;

    static int beatCounter; 
    static bool toggleMetronome;

    static int notesPlayed;
    static float noteIntensity;

    static vector<int> drumGfx;
    static const int nDrumTypes;

    static bool toggleScaleBuild;
    static vector< vector< unsigned char > > scaleBuffer;
    static int scalePlayhead;

    static std::map<int, bool> scaleMap;
    static int scaleRoot;

    static vector<triad> triads;

    static map<int, bool> currentNotes;

    static int currentBassNote;
  
    static int midiMode;

    static float drumGain;
    static float bassGain;
};


#endif
