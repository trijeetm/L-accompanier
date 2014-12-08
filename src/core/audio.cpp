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

using namespace std;

// globals
YFluidSynth * g_synth;
// mYEcho * g_echo;

void dm_playSynths() {
    // bool noteplayed = false;
    // int drumNotesPlayed = 0;
    // // clear notes
    // for (int i = 0; i < DM_POLYPHONY; i++) {
    //     if (Globals::pianoGrid[Globals::playhead].notes[i].on) {
    //         g_synth->allNotesOff(1);
    //     }
    //     if (Globals::synthGrid[Globals::playhead].notes[i].on) {
    //         g_synth->allNotesOff(2);
    //     }
    // }
    // for (int i = 0; i < DM_POLYPHONY; i++) {
    //     if (Globals::drumGrid[Globals::playhead].notes[i].on) {
    //         // play midi drums
    //         g_synth->programChange(0, 69);
    //         if (i % 4 == 0)
    //             g_synth->noteOn(0, Globals::drumGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(7, 120));
    //         else if (i % 2 == 0)
    //             g_synth->noteOn(0, Globals::drumGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(15, 100));
    //         else
    //             g_synth->noteOn(0, Globals::drumGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(15, 75));
    //         noteplayed = true;
    //         drumNotesPlayed++;
    //     }
    //     if (Globals::pianoGrid[Globals::playhead].notes[i].on) {
    //         // play midi pianos
    //         g_synth->programChange(1, 36);
    //         if (i % 4 == 0) {
    //             g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(27, 100));
    //             g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch + 12, XFun::rand2i(27, 100));
    //             // g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch + 7, XFun::rand2i(27, 100));
    //         }
    //         else if (i % 2 == 0) {
    //             g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(20, 95));
    //             g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch + 12, XFun::rand2i(20, 95));
    //             // g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch + 7, XFun::rand2i(20, 95));
    //         }
    //         else {
    //             g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(25, 70));
    //             g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch + 12, XFun::rand2i(25, 70));
    //             // g_synth->noteOn(1, Globals::pianoGrid[Globals::playhead].notes[i].pitch + 7, XFun::rand2i(30, 75));
    //         }
    //         // noteplayed = true;
    //         // drumNotesPlayed++;
    //     }
    //     if (Globals::synthGrid[Globals::playhead].notes[i].on) {
    //         // play midi synth chords
    //         g_synth->programChange(2, 4);
    //         if (i % 4 == 0) {
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(27, 100));
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch + 12, XFun::rand2i(27, 100));
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch + 7, XFun::rand2i(27, 100));
    //         }
    //         else if (i % 2 == 0) {
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(20, 95));
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch + 12, XFun::rand2i(20, 95));
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch + 7, XFun::rand2i(20, 95));
    //         }
    //         else {
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch, XFun::rand2i(25, 70));
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch + 12, XFun::rand2i(25, 70));
    //             g_synth->noteOn(2, Globals::synthGrid[Globals::playhead].notes[i].pitch + 7, XFun::rand2i(30, 75));
    //         }
    //         // noteplayed = true;
    //         // drumNotesPlayed++;
    //     }
    // }
    // Globals::drumGrid[Globals::playhead].totalPatches = drumNotesPlayed;
    // Globals::pianoGrid[Globals::playhead].totalPatches = drumNotesPlayed;
    // Globals::synthGrid[Globals::playhead].totalPatches = drumNotesPlayed;
    // // if (noteplayed)
    // //     cerr << "•" << "\t";
    // // else
    // //     cerr << "x" << "\t";
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

int chooseRandomSample(int samples[], int size) {
    return samples[random() % size];
}

int randInt(int low, int high) {
    return low + (random() % (high - low));
}

void drummer() {
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

    float drumGain = Globals::drumGain;

    // reset g_synth
    g_synth->programChange(C_DRUMMER, 69);

    // reset drum gfx
    for (int i = 0; i < Globals::nDrumTypes; i++)
        Globals::drumGfx[i] = 0;

    if (Globals::noteIntensity >= 0.05) {
        // bass and hi hat at first beat
        if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats - 1), randInt(110, 126) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
            g_synth->noteOn(C_DRUMMER, 35, randInt(110, 126) * drumGain);
            Globals::drumGfx[DM_BASS] = 1;
        }
    }

    if (Globals::noteIntensity >= 0.1) {
        // hi hat at downbeats
        if (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats - 1), randInt(85, 100) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
        }
    }

    if (Globals::noteIntensity >= 0.25) {
        // hi hats all beats
        if (Globals::beatCounter % Globals::tSign.beatUnit == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats - 1), randInt(80, 90) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
        }
    }

    if (Globals::noteIntensity >= 0.35) {
        // bass at downbeats
        if (Globals::beatCounter % ((Globals::tSign.beatCount * Globals::tSign.beatUnit) / 2) == 0) {
            g_synth->noteOn(C_DRUMMER, 35, randInt(85, 100) * drumGain);    
            Globals::drumGfx[DM_BASS] = 1;
        }
    }

    if (Globals::noteIntensity >= 0.5) {
        // snare and bass2 at some upbeats
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

    if (Globals::noteIntensity >= 0.6) {
        // snare and bass2 at some more upbeats
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

    if (Globals::noteIntensity >= 0.65) {
        // rand hi hat at every second beat event
        if ((Globals::beatCounter % 2) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats), randInt(50, randInt(80, 95)) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
            if (randInt(0, 10) > 5) {
                g_synth->noteOn(C_DRUMMER, chooseRandomSample(snares, nSnares), randInt(70, 100) * drumGain);
                Globals::drumGfx[DM_SNARE] = 1;
            }
        }
    } 

    if (Globals::noteIntensity >= 1.0) {
        // rand hi hat at every beat event
        if ((Globals::beatCounter) == 0) {
            g_synth->noteOn(C_DRUMMER, chooseRandomSample(hiHats, nHiHats), randInt(45, randInt(70, 95)) * drumGain);
            Globals::drumGfx[DM_HIHATS] = 1;
        }
    } 
}

int g_lastBassNotePlayed = -1;
triad g_currTriadInUse = triad();

void bassist() {
    int registerOffset = (12 * randInt(2, 3));
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
    if (currentNoteLowest == -1)
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
        }
        if (currentNoteLowest == currTriad.third) {
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
        }
        if (currentNoteLowest == currTriad.fifth) {
            parentTriads.push_back(currTriad);
            parentTriads.push_back(currTriad);
        }
    }

    // choose current triad

    int currentBassNote;
    if (g_currTriadInUse.type == TR_INVALID)
        currentBassNote = Globals::scaleRoot;
    else {
        int decider = randInt(0, 20);
        if (decider > 15) 
            currentBassNote = g_currTriadInUse.fifth;
        else if (decider > 10) 
            currentBassNote = g_currTriadInUse.third;
        else if (decider >= 0) 
            currentBassNote = g_currTriadInUse.first;
    }

    // add register offset 
    currentBassNote += registerOffset;

    if ((g_lastBassNotePlayed != currentBassNote) && (g_lastBassNotePlayed != -1)) {
        g_synth->noteOff(C_BASSIST, g_lastBassNotePlayed);
        Globals::currentBassNote = -1;
    }
    if (Globals::drumGfx[DM_BASS] || Globals::drumGfx[DM_SNARE]) {
        // cerr << currentNote << endl;
        Globals::currentBassNote = currentBassNote % 12;
        g_synth->noteOn(C_BASSIST, currentBassNote, randInt(80, 127) * Globals::bassGain);
    }
    g_lastBassNotePlayed = currentBassNote;
}

vector<int> g_notePlayedHistory;

void calculateNoteIntensity() {
    // old implementation
    if (Globals::beatCounter % (Globals::tSign.beatCount * Globals::tSign.beatUnit * 1) == 0) {
        Globals::noteIntensity = Globals::notesPlayed * 1.0 / (Globals::tSign.beatCount * Globals::tSign.beatUnit * 1.0);
        cerr << endl << Globals::noteIntensity;
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
            for (int i = 0; i < Globals::scaleMap.size(); i++) {
                triad newTriad = triad();
                if (Globals::scaleMap[i]) {
                    newTriad.first = i;

                    if (Globals::scaleMap[(i + 4) % 12]) {
                        newTriad.third = (i + 4) % 12;
                        newTriad.type = TR_MAJOR;
                    }
                    else if (Globals::scaleMap[(i + 3) % 12]) {
                        newTriad.third = (i + 3) % 12;
                        newTriad.type = TR_MINOR;
                    }
                    else 
                        newTriad.type = TR_INVALID;

                    if (Globals::scaleMap[(i + 7) % 12])
                        newTriad.fifth = (i + 7) % 12;
                    else 
                        newTriad.type = TR_INVALID;
                }
                if (newTriad.type != TR_INVALID) 
                    Globals::triads.push_back(newTriad);
            }
            cerr << "Triads:" << endl;
            for (int i = 0; i < Globals::triads.size(); i++)
                cerr << Globals::triads[i].type << "\t" << Globals::triads[i].first << "\t" << Globals::triads[i].third << "\t" << Globals::triads[i].fifth << "\t" << endl;
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
    // i = 0;

    for ( i=0; i<nPorts; i++ ) {
      portName = rtmidi->getPortName(i);
      std::cout << "  Input port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nPorts );
    std::getline( std::cin, keyHit );  // used to clear out stdin
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
        vector< unsigned char > midi = *message;
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
        vector< unsigned char > midi = *message;
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
