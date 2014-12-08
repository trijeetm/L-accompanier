//---------------------------------------------------------------------------
// name: dm-audio.cpp
// desc: audio stuff
//
// author: Trijeet Mukhopadhyay (trijeetm@stanford.edu)
//   date: 2014
//---------------------------------------------------------------------------
#include "globals.h"

BKSim * Globals::sim = NULL;

int Globals::bpm = 90;
timeSignature Globals::tSign = timeSignature();
SAMPLE Globals::now = 0;
SAMPLE Globals::accumulator = 0;
SAMPLE Globals::tempoPeriod = (1.0 / (Globals::bpm * Globals::tSign.beatUnit / 60.0)) * DM_SRATE;

noteStack Globals::drumGrid[DM_N_GRID_ELEMENTS]; 
noteStack Globals::pianoGrid[DM_N_GRID_ELEMENTS];
noteStack Globals::synthGrid[DM_N_GRID_ELEMENTS];
int Globals::activeSynth = DM_DRUMS;

int Globals::playhead = 0;

int Globals::currentPatch = 0;
// int Globals::currentNote = 35;

int Globals::currentPianoPatch = 0;
int Globals::currentPianoNote = 24;

int Globals::currentSynthPatch = 0;
int Globals::currentSynthNote = 48;

vector< vector< unsigned char > > Globals::midiBuffer;
int Globals::midiPlayhead = 0;

bool Globals::toggleMetronome = true;
int Globals::beatCounter = 0;

int Globals::notesPlayed = 0;
float Globals::noteIntensity = 0;

vector<int> Globals::drumGfx;
const int Globals::nDrumTypes = 3;

bool Globals::toggleScaleBuild = true;
vector< vector< unsigned char > > Globals::scaleBuffer;
int Globals::scalePlayhead = 0;
std::map<int, bool> Globals::scaleMap;
int Globals::scaleRoot = -1;
vector<triad> Globals::triads;

map<int, bool> Globals::currentNotes;

int Globals::currentBassNote = -1;

int Globals::midiMode = MIDI_IN;

float Globals::drumGain = 1;
float Globals::bassGain = 1;
