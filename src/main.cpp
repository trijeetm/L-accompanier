//---------------------------------------------------------------------------
// name: main.cpp
// desc: audio stuff
//
// author: Trijeet Mukhopadhyay (trijeetm@stanford.edu)
//   date: 2014
//---------------------------------------------------------------------------

#include <iostream>

#include "globals.h"
#include "audio.h"
#include "gfx.h"

using namespace std;

int main( int argc, char ** argv ) {
    // set tempo and time signature from command line
    if (argc >= 4) {
        Globals::bpm = atoi(argv[1]);
        Globals::tSign.beatCount = atoi(argv[2]);
        Globals::tSign.beatUnit = atoi(argv[3]);
    }

    // check for midi pref
    if (argc == 5) {
        if (strcmp(argv[4], "--midi-in") == 0)
            Globals::midiMode = MIDI_IN;
        if (strcmp(argv[4], "--midi-thru") == 0)
            Globals::midiMode = MIDI_THRU;
    }
    
    cerr << "BPM = " << Globals::bpm << endl;
    cerr << "Time Signature " << Globals::tSign.beatCount << "/" << Globals::tSign.beatUnit << endl;
    // start real-time audio
    if( !audio_init( DM_SRATE, DM_FRAMESIZE, DM_NUMCHANNELS ) )
    {
        // error message
        cerr << "[accompanier]: cannot initialize real-time audio I/O..." << endl;
        return -1;
    }

    // invoke graphics setup and loop
    if( !gfx_init( argc, argv ) )
    {
        // error message
        cerr << "[accompanier]: cannot initialize graphics/data system..." << endl;
        return -1;
    }

    // start real-time midi
    if( !midi_init() )
    {
        // error message
        cerr << "[accompanier]: cannot initialize real-time midi I/O..." << endl;
        return -1;
    }
    
    // start audio
    if( !audio_start() )
    {
        // error message
        cerr << "[accompanier]: cannot start real-time audio I/O..." << endl;
        return -1;
    }
    
    // start gfx and hand over control
    gfx_loop();
    
    return 0;
}