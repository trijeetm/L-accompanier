//-----------------------------------------------------------------------------
// name: dm-audio.cpp
//
// author: Trijeet Mukhopadhyay (trijeetm@stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#include "audio.h"
#include "x-audio.h"
#include "y-fluidsynth.h"
#include "y-echo.h"
#include "x-fun.h"
#include "RtMidi.h"
#include <iostream>
#include <time.h>
#include <math.h>

#define INTENSITY_SLEW      0.5
#define INTENSITY_SAMPLING_RATE      1
#define INTENSITY_FACTOR    0.0625

using namespace std;

// globals
YFluidSynth * g_synth;
// mYEcho * g_echo;
Vector3D g_intensitySlew(Globals::noteIntensity, Globals::noteIntensity, INTENSITY_SLEW);

int chooseRandomSample(int samples[], int size) {
    return samples[random() % size];
}

int randInt(int low, int high) {
    return low + (random() % (high - low));
}

void metronome() {
    g_synth->programChange(C_METRONOME, 69);
    if (Globals::toggleMetronome)
        if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) == 0) {
            // cerr << "x";
            g_synth->noteOn(C_METRONOME, 81, 127);
        }
        else if (Globals::beatCounter % Globals::tSign.beatUnit == 0) {
            // cerr << ".";
            g_synth->noteOn(C_METRONOME, 80, 110);
        }
}

vector<int> g_notePlayedHistory;

void calculateNoteIntensity() {
    // old implementation
    if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit * INTENSITY_SAMPLING_RATE) == 0) {
        Globals::noteIntensity = Globals::notesPlayed * 1.0 / (Globals::tSign.beatCount * Globals::tSign.beatUnit * INTENSITY_SAMPLING_RATE);
        // / clamp it
        if (Globals::noteIntensity > 3.5)
            Globals::noteIntensity = 3.5;
        // cerr << endl << Globals::noteIntensity;
        Globals::notesPlayed = 0;
    } 
    return;

    // limit size
    if (g_notePlayedHistory.size() > (Globals::tSign.beatCount * Globals::tSign.beatUnit * 16))
        g_notePlayedHistory.erase(g_notePlayedHistory.begin() + (Globals::tSign.beatCount * Globals::tSign.beatUnit * 16), g_notePlayedHistory.begin() + g_notePlayedHistory.size());

    // calculate
    if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit * 1) == 0) {
        int notesPlayed = 0;
        for (int i = 0; i < (Globals::tSign.beatCount * Globals::tSign.beatUnit * 1); i++) {
            if (i >= g_notePlayedHistory.size())
                break;
            notesPlayed += g_notePlayedHistory[i];
        }
        Globals::noteIntensity = notesPlayed / (Globals::tSign.beatCount * Globals::tSign.beatUnit * 1.0);
        // clamp it
        // if (Globals::noteIntensity > 3.5)
            // Globals::noteIntensity = 3.5;
        cerr << endl << notesPlayed;
    }

}

bool g_drumRollInProgress = false;
int g_drumRollBeats = 0;

void drumRoll() {
    int hiHats[3] = {42, 44, 46};
    int nHiHats = 3;
    int snares[2] = {38, 40};
    int nSnares = 2;
    int basses[2] = {35, 36};
    int nBasses = 2;
    int toms[5] = {41, 43, 45, 47, 48};
    int nToms = 5;

    g_synth->programChange(C_DRUMMER, 69);

    float drumGain = Globals::drumGain;

    // init drum roll
    if (!g_drumRollInProgress) {
        g_drumRollInProgress = true;
        g_drumRollBeats = 0;
    }

    // cerr << "drum roll!" << endl;
    int bassHeuristic, TSHueristic;

    float intensityFactor = INTENSITY_FACTOR;

    if (g_intensitySlew.value >= intensityFactor * 2) {
        // cerr << "I1" << endl;
        bassHeuristic = 7;
        TSHueristic = 8;
    }
    if (g_intensitySlew.value >= intensityFactor * 3) {
        // cerr << "I2" << endl;
        bassHeuristic = 6;
        TSHueristic = 7;
    }
    if (g_intensitySlew.value >= intensityFactor * 4) {
        // cerr << "I3" << endl;
        bassHeuristic = 6;
        TSHueristic = 6;
    }
    if (g_intensitySlew.value >= intensityFactor * 6) {
        // cerr << "I4" << endl;
        bassHeuristic = 5;
        TSHueristic = 6;
    }
    if (g_intensitySlew.value >= intensityFactor * 8) {
        // cerr << "I5" << endl;
        bassHeuristic = 4;
        TSHueristic = 5;
    }
    if (g_intensitySlew.value >= intensityFactor * 12) {
        // cerr << "I6" << endl;
        bassHeuristic = 3;
        TSHueristic = 4;
    }

    // bass drums
    if (randInt(0, 10) > bassHeuristic) {
        g_synth->noteOn(C_DRUMMER, chooseRandomSample(basses, nBasses), randInt(90, 120) * drumGain);
        Globals::drumGfx[DM_BASS] = 1;
    }

    // toms  snares
    if (randInt(0, 10) > TSHueristic) {
        g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(95, 110) * drumGain);
        Globals::drumGfx[DM_SNARE] = 1;
    }
    if (randInt(0, 10) > TSHueristic) {
        g_synth->noteOn(C_DRUMMER, chooseRandomSample(toms, nToms), randInt(95, 110) * drumGain);
        Globals::drumGfx[DM_TOMS] = 1;
    }

    // inc drum roll beat
    g_drumRollBeats++;

    // terminate drum roll
    if (g_drumRollBeats >= (Globals::tSign.beatCount * Globals::tSign.beatUnit)) {
        g_drumRollInProgress = false;
    }
}

void drummer() {

    g_intensitySlew.goal = Globals::noteIntensity;
    g_intensitySlew.interp2(INTENSITY_SLEW);
    // cerr << "Intensity: " << g_intensitySlew.value << endl;

    // bass drum 1 - 35
    // bass drum 2 - 36
    // snare 1 - 38
    // snare 2 - 40
    // hi hat closed - 42
    // hi hat pedal - 44
    // hi hat open - 46
    int hiHats[3] = {42, 44, 46};
    int nHiHats = 3;
    int snares[2] = {38, 40};
    int nSnares = 2;
    int basses[2] = {35, 36};
    int nBasses = 2;
    int cymbals[4] = {49, 51, 55, 57};
    int nCymbals = 4;

    float drumGain = Globals::drumGain;

    float intensityFactor = INTENSITY_FACTOR;

    // reset g_synth
    g_synth->programChange(C_DRUMMER, 69);

    // reset drum gfx
    for (int i = 0; i < Globals::nDrumTypes; i++)
        Globals::drumGfx[i] = 0;

    // drum roll
    if (((Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit * (4 * ((randInt(0, 2) % 2) + 1))) == 0) || (g_drumRollInProgress)) && (g_intensitySlew.value > intensityFactor * 2)) {
        drumRoll();
    }

    if (g_drumRollInProgress)
        return;

    // cymbals
    if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit * 2) == 0) {
        if (randInt(0, 10) > 3) {   
            if (g_intensitySlew.value > intensityFactor * 2) {
                // cerr << "cymbals!" << endl;
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(cymbals, nCymbals), randInt(65, 85) * drumGain);
                Globals::drumGfx[DM_CYMBAL] = 1;
            }
            if (g_intensitySlew.value > intensityFactor * 4) {
                // cerr << "cymbals!!" << endl;
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(cymbals, nCymbals), randInt(75, 90) * drumGain);
                Globals::drumGfx[DM_CYMBAL] = 1;
            }
            if (g_intensitySlew.value > intensityFactor * 6) {
                // cerr << "cymbals!!!" << endl;
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(cymbals, nCymbals), randInt(85, 105) * drumGain);
                Globals::drumGfx[DM_CYMBAL] = 1;
            }
        }
    }
    if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) == 0) {
        if (randInt(0, 10) > 6) {
            if (g_intensitySlew.value > intensityFactor * 4) {
                // cerr << "cymbals!!" << endl;
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(cymbals, nCymbals), randInt(55, 80) * drumGain);
                Globals::drumGfx[DM_CYMBAL] = 1;
            }
            if (g_intensitySlew.value > intensityFactor * 6) {
                // cerr << "cymbals!!!" << endl;
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(cymbals, nCymbals), randInt(65, 95) * drumGain);
                Globals::drumGfx[DM_CYMBAL] = 1;
            }
            if (g_intensitySlew.value > intensityFactor * 12) {
                // cerr << "cymbals!!!" << endl;
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(cymbals, nCymbals), randInt(80, 105) * drumGain);
                Globals::drumGfx[DM_CYMBAL] = 1;
            }
        }
    }

    if (g_intensitySlew.value > intensityFactor * 0.0005) {
        // hi hat at first beat
        // cerr << "hi hat at first beat" << endl;
        if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats - 1), randInt(110, 126) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 1) {
        // bass and hi hat at first beat
        // cerr << "bass and hi hat at first beat" << endl;
        if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats - 1), randInt(110, 126) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(basses, nBasses), randInt(110, 126) * drumGain);
            Globals::drumGfx[DM_BASS] = 1;
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 2) {
        // hi hat at downbeats
        // cerr << "hi hat at downbeats" << endl;
        if (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats - 1), randInt(85, 100) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 3.5) {
        // snare and bass2 at few upbeats
        // cerr << "snare and bass2 at few upbeats" << endl;
        if ( 
            (Globals::beatCounter % Globals::tSign.beatUnit == 0) && 
            (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) != 0) && 
            (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) != 0) ) {
            if (randInt(0, 10) > 8) {
                g_synth->noteOn(C_DRUMMER, 38, randInt(80, 90) * drumGain);
                Globals::drumGfx[DM_BASS] = 1;
            }

            if (randInt(0, 10) > 8) {
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(80, 90) * drumGain);
                Globals::drumGfx[DM_SNARE] = 1;
            }
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 4) {
        // hi hats all beats
        // cerr << "hi hats all beats" << endl;
        if (Globals::beatCounter % Globals::tSign.beatUnit == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats), randInt(80, 90) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 4.75) {
        // bass at downbeats
        // cerr << "bass at downbeats" << endl;
        if (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(basses, nBasses), randInt(85, 100) * drumGain);    
            Globals::drumGfx[DM_BASS] = 1;
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 5.5) {
        // snare and bass2 at some upbeats
        // cerr << "snare and bass2 at some upbeats" << endl;
        if ( 
            (Globals::beatCounter % Globals::tSign.beatUnit == 0) && 
            (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) != 0) && 
            (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) != 0) ) {
            if (randInt(0, 10) > 5) {
                g_synth->noteOn(C_DRUMMER, 38, randInt(80, 90) * drumGain);
                Globals::drumGfx[DM_BASS] = 1;
            }

            if (randInt(0, 10) > 6) {
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(80, 90) * drumGain);
                Globals::drumGfx[DM_SNARE] = 1;
            }
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 7) {
        // snare and bass2 at some more upbeats
        // cerr << "snare and bass2 at some more upbeats" << endl;
        if ( 
            (Globals::beatCounter % Globals::tSign.beatUnit == 0) && 
            (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) != 0) && 
            (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) != 0) ) {
            if (randInt(0, 10) > 3) {
                g_synth->noteOn(C_DRUMMER, 38, randInt(75, 90) * drumGain);
                Globals::drumGfx[DM_BASS] = 1;
            }

            if (randInt(0, 10) > 2) {
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(75, 90) * drumGain);
                Globals::drumGfx[DM_SNARE] = 1;
            }
        }
    }

    if (g_intensitySlew.value >= intensityFactor * 8) {
        // rand hi hat at every second beat event
        // cerr << "rand hi hat at every second beat event" << endl;
        if ((Globals::beatCounter % 2) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats), randInt(50, randInt(80, 95)) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
            if (randInt(0, 10) > 5) {
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(70, 100) * drumGain);
                Globals::drumGfx[DM_SNARE] = 1;
            }
        }
    } 

    if (g_intensitySlew.value >= intensityFactor * 12) {
        // rand hi hat at every beat event
        // cerr << "rand hi hat at every beat event" << endl;
        if ((Globals::beatCounter % 1) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats), randInt(45, randInt(70, 95)) * drumGain);
            // disabled for gfx sanity
            // Globals::drumGfx[DM_HIHATS] = 1;
        }
    } 

    if (g_intensitySlew.value >= intensityFactor * 16) {
        // rand hi hat at every beat event
        // cerr << "rand hi hat at every beat event" << endl;
        if ((Globals::beatCounter % 1) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats), randInt(45, randInt(95, 115)) * drumGain);
            // disabled for gfx sanity
            // Globals::drumGfx[DM_HIHATS] = 1;
        }

        if ( 
            (Globals::beatCounter % Globals::tSign.beatUnit == 0) && 
            (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) != 0) && 
            (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) != 0) ) {
            if (randInt(0, 10) > 2) {
                g_synth->noteOn(C_DRUMMER, 38, randInt(90, 110) * drumGain);
                Globals::drumGfx[DM_BASS] = 1;
            }

            if (randInt(0, 10) > 1) {
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(90, 110) * drumGain);
                Globals::drumGfx[DM_SNARE] = 1;
            }
        }

        // some snare at downbeats
        // cerr << "bass at downbeats" << endl;
        if (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(75, 90) * drumGain);
            Globals::drumGfx[DM_SNARE] = 1;
        }
    } 
}

int g_lastBassNotePlayed = -1;
triad g_currTriadInUse = triad();

void bassist() {
    int registerOffset = (12 * randInt(2, 4));
    int bassNotes[] = { Globals::scaleRoot, Globals::scaleRoot, Globals::scaleRoot };
    g_synth->programChange(C_BASSIST, 24);
    int currentNoteLowest = -1;
    // find lowest current note, if exists
    for (int i = 0; i < Globals::currentNotes.size(); i++) {
        if (Globals::currentNotes[i]) {
            currentNoteLowest = i % 12;
            break;
        }
    } 
    if ((currentNoteLowest == -1) || (!Globals::scaleMap[currentNoteLowest]))
        currentNoteLowest = Globals::scaleRoot;

    // check for triad membership of lowest current note
    vector<triad> parentTriads;
    for (int i = 0; i < Globals::triads.size(); i++) {
        triad currTriad = Globals::triads[i];
        if (currentNoteLowest == currTriad.first) {
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
        }
        if (currentNoteLowest == currTriad.third) {
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
        }
        if (currentNoteLowest == currTriad.fifth) {
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
        }
    }

    // choose current triad
    if (Globals::beatCounter % (Globals::tSign.beatUnit / 2) == 0) {
        // cerr << "triad: " << endl;
        if (!parentTriads.empty())
            g_currTriadInUse = parentTriads[randInt(0, parentTriads.size())];
        else 
            g_currTriadInUse = triad(TR_MAJOR, Globals::scaleRoot, Globals::scaleRoot, Globals::scaleRoot);
        Globals::triadGfx = g_currTriadInUse;
        Globals::triadGfx.type = TR_GFX;
        // cerr << g_currTriadInUse.type << "\t" << g_currTriadInUse.first << "\t" << g_currTriadInUse.third << "\t" << g_currTriadInUse.fifth << "\t" << endl;
    }


    int currentBassNote;
    if (g_currTriadInUse.type == TR_INVALID)
        currentBassNote = Globals::scaleRoot;
    else {
        int decider = randInt(0, 50);
        if (decider > 15) 
            currentBassNote = g_currTriadInUse.first;
        else if (decider > 5) 
            currentBassNote = g_currTriadInUse.fifth;
        else if (decider >= 0) 
            currentBassNote = g_currTriadInUse.third;
    }

    // add register offset 
    currentBassNote += registerOffset;

    if (Globals::drumGfx[DM_BASS] || Globals::drumGfx[DM_SNARE]) {
        if ((currentBassNote != g_lastBassNotePlayed) && (g_lastBassNotePlayed != -1)) {
            g_synth->noteOff(C_BASSIST, g_lastBassNotePlayed);
            Globals::currentBassNote = -1;
        }
        // cerr << currentNote << endl;
        if (g_drumRollInProgress) {
            if (randInt(0, 10) >= 5) {
                Globals::currentBassNote = currentBassNote % 12;
                g_synth->noteOn(C_BASSIST, currentBassNote, randInt(105, 127) * Globals::bassGain);
                g_lastBassNotePlayed = currentBassNote; 
            }
        }
        else {
            Globals::currentBassNote = currentBassNote % 12;
            g_synth->noteOn(C_BASSIST, currentBassNote, randInt(105, 127) * Globals::bassGain);
            g_lastBassNotePlayed = currentBassNote; 
        }
        
    }
}

//--------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//--------------------------------------------------------------------------
static void audio_callback(SAMPLE * buffer, unsigned int numFrames, void * userData) {
    Globals::now += numFrames;

    // cerr << ".";

    // XFun::srand();

    if (Globals::toggleScaleBuild) {
        Globals::triads.clear();
        g_synth->programChange(C_INPUT, 24);
        for (Globals::scalePlayhead; Globals::scalePlayhead < Globals::scaleBuffer.size(); Globals::scalePlayhead++) {
            vector< unsigned char > midiMessage = Globals::scaleBuffer[Globals::scalePlayhead];
            if (midiMessage[0] == 144) {
                g_synth->noteOn(C_INPUT, midiMessage[1], midiMessage[2]);
                // Globals::notesPlayed++;
            }
            else if (midiMessage[0] == 128)
                g_synth->noteOff(C_INPUT, midiMessage[1]);
        }
    }

    else {
        // calculate triads if it doesn't exist
        if (Globals::triads.empty()) {
            // cerr << "start build" << endl;
            for (int i = 0; i < Globals::scaleMap.size(); i++) {
                triad newTriad = triad();
                if (Globals::scaleMap[i]) {
                    // cerr << "building triad" << endl;

                    // cerr << "1" << endl;    
                    newTriad.first = i;

                    // cerr << "2" << endl;    
                    if (Globals::scaleMap[(i + 4) % Globals::scaleMap.size()]) {
                        newTriad.third = (i + 4) % Globals::scaleMap.size();
                        newTriad.type = TR_MAJOR;
                    }
                    else if (Globals::scaleMap[(i + 3) % Globals::scaleMap.size()]) {
                        newTriad.third = (i + 3) % Globals::scaleMap.size();
                        newTriad.type = TR_MINOR;
                    }
                    else 
                        newTriad.type = TR_INVALID;

                    // cerr << "3" << endl;    
                    if (Globals::scaleMap[(i + 7) % Globals::scaleMap.size()])
                        newTriad.fifth = (i + 7) % Globals::scaleMap.size();
                    else 
                        newTriad.type = TR_INVALID;

                    // cerr << newTriad.type << "\t" << newTriad.first << "\t" << newTriad.third << "\t" << newTriad.fifth << "\t" << endl;
                }
                if (newTriad.type != TR_INVALID) 
                    Globals::triads.push_back(newTriad);
            }
            // cerr << "triads size: " << Globals::triads.size() << endl;
            if (Globals::triads.empty()) {
                Globals::triads.push_back(triad(TR_MAJOR, Globals::scaleRoot, Globals::scaleRoot, Globals::scaleRoot));
            }
            // cerr << "Triads:" << endl;
            // for (int i = 0; i < Globals::triads.size(); i++)
            //     cerr << Globals::triads[i].type << "\t" << Globals::triads[i].first << "\t" << Globals::triads[i].third << "\t" << Globals::triads[i].fifth << "\t" << endl;
        }
        // beat event
        Globals::tempoPeriod = (1.0 / (Globals::bpm * (Globals::tSign.beatUnit) / 60.0)) * DM_SRATE;
        if (Globals::accumulator > Globals::tempoPeriod) {
            Globals::accumulator -= Globals::tempoPeriod;
            // calculate note intensity
            calculateNoteIntensity();
            // fire metronome
            if (Globals::toggleMetronome)
                metronome();
            // fire drummer
            drummer();
            // fire bassist
            bassist();
            // advance beat
            Globals::beatCounter++;
        }
        Globals::accumulator += numFrames;
        // play live midi
        g_synth->programChange(C_INPUT, 0);
        bool _notesPlayed = false;
        for (Globals::midiPlayhead; Globals::midiPlayhead < Globals::midiBuffer.size(); Globals::midiPlayhead++) {
            vector< unsigned char > midiMessage = Globals::midiBuffer[Globals::midiPlayhead];
            if (midiMessage[0] == 144) {
                if (Globals::midiMode == MIDI_IN) {
                    g_synth->noteOn(C_INPUT, midiMessage[1], midiMessage[2]);
                }
                Globals::currentNotes[midiMessage[1]] = true;
                Globals::notesPlayed++;
            }
            else if (midiMessage[0] == 128) {
                g_synth->noteOff(C_INPUT, midiMessage[1]);
                Globals::currentNotes[midiMessage[1]] = false;
            }
            // if (midiMessage[0] == 144) {
            //     cerr << "inserting 1" << endl;
            //     g_notePlayedHistory.insert(g_notePlayedHistory.begin(), 1);
            //     _notesPlayed = true;
            // }
            // else
                // g_notePlayedHistory.insert(g_notePlayedHistory.begin(), 0);
        }
        // if (!_notesPlayed) {
            // cerr << "inserting 0" << endl;
            // g_notePlayedHistory.insert(g_notePlayedHistory.begin(), 0);   
        // }
    }

    // synthesize it
    g_synth->synthesize2( buffer, numFrames );
    // echo it
    // g_echo->synthesize2( buffer, numFrames );
}




//--------------------------------------------------------------------------
// name: dm_audio_init()
// desc: initialize audio system
//--------------------------------------------------------------------------
bool audio_init( unsigned int srate, unsigned int frameSize, unsigned channels ) {
    if( !XAudioIO::init( 0, 0, srate, frameSize, channels, audio_callback, NULL ) )
    {
        // done
        return false;
    }

    // XFun::srand();

    // init drum gfx
    for (int i = 0; i < Globals::nDrumTypes; i++)
        Globals::drumGfx.push_back(0);

    // init scaleMap
    for (int i = 0; i < 12; i++)
        Globals::scaleMap[i] = false;

    // init current notes map
    for (int i = 0; i < 128; i++) 
        Globals::currentNotes[i] = false;

    // init g_synth
    g_synth = new YFluidSynth();
    g_synth->init( srate, 32 );
    g_synth->load( "data/sfonts/rocking8m11e copy.sf2", "" );
    g_synth->programChange(0, 0);
    
    // // allocate echo
    // g_echo = new YEcho( srate );
    // g_echo->setDelay( 0, .25 );
    // g_echo->setDelay( 1, .5 );
    
    return true;
}

bool chooseMidiPort( RtMidiIn *rtmidi )
{
  // std::cout << "\nWould you like to open a virtual input port? [y/n] ";

  std::string keyHit;
  // std::getline( std::cin, keyHit );
  // if ( keyHit == "y" ) {
  //   rtmidi->openVirtualPort();
  //   return true;
  // }

  std::string portName;
  unsigned int i = 0, nPorts = rtmidi->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No input ports available!" << std::endl;
    return false;
  }

  if ( nPorts == 1 ) {
    std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
  }
  else {
    // temp hack to choose port 0
    i = 0;

    // for ( i=0; i<nPorts; i++ ) {
    //   portName = rtmidi->getPortName(i);
    //   std::cout << "  Input port #" << i << ": " << portName << '\n';
    // }

    // do {
    //   std::cout << "\nChoose a port number: ";
    //   std::cin >> i;
    // } while ( i >= nPorts );
    // std::getline( std::cin, keyHit );  // used to clear out stdin
  }

  rtmidi->openPort( i );

  return true;
}

int g_midi_correction = -1;

void midi_callback(double deltatime, std::vector< unsigned char > *message, void *userData) {
    vector< unsigned char > midi = *message;
    // noteOn range correction for channels
    if (((int)midi[0] >= 144) && ((int)midi[0] < 144 + 16))
        midi[0] = 144;
    // noteOff range correction for channels
    if (((int)midi[0] >= 128) && ((int)midi[0] < 128 + 16))
        midi[0] = 128;
    // build scale
    if (Globals::toggleScaleBuild) {
        // cerr << "build scale mode" << endl;
        // vector< unsigned char > midi = *message;
        if ((int)midi[0] == 144) {
            if (Globals::scaleRoot == -1)
                Globals::scaleRoot = (int)midi[1] % 12;
            Globals::scaleMap[(int)midi[1] % 12] = true;
        }
        Globals::scaleBuffer.push_back(midi);
    }
    // push to midi buffer
    else {
        // cerr << "live midi" << endl;
        // vector< unsigned char > midi = *message;
        // system dependant correction?
        midi[0] = (int)midi[0];
        Globals::midiBuffer.push_back(midi);
    }
    // Globals::midiPlayhead++;
    // unsigned int nBytes = message->size();
    // for ( unsigned int i=0; i<nBytes; i++ ) {
    //     cerr << "Byte " << i << " = " << (int)message->at(i) << ", ";

    // }
    // if ( nBytes > 0 )
    //     cerr << "stamp = " << deltatime << std::endl;
}

bool midi_init() {
    RtMidiIn *midiin = new RtMidiIn();

    // Call function to select port.
    if ( chooseMidiPort( midiin ) == false ) goto cleanup;

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.    
    midiin->setCallback( &midi_callback );

    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );

    // wait for input
    // std::cout << "\nReading MIDI input... press <enter> to quit.\n";
    // char input;
    // std::cin.get(input);

    return true;

    // Clean up
    cleanup:
        delete midiin;
        return false;
}


//--------------------------------------------------------------------------
// name: dm_audio_start()
// desc: start audio system
//--------------------------------------------------------------------------
bool audio_start() {
    XFun::srand();

    // start the audio
    if( !XAudioIO::start() ) {
        // done
        return false;
    }
    
    return true;
}
