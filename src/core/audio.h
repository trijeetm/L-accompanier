//---------------------------------------------------------------------------
// name: dm-audio.h
// desc: audio stuff
//
// author: Trijeet Mukhopadhyay (trijeetm@stanford.edu)
//   date: 2014
//---------------------------------------------------------------------------
#ifndef __DM_AUDIO_H__
#define __DM_AUDIO_H__

#include "globals.h"

// init audio
bool audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );

// init midi
bool midi_init();

// start audio
bool audio_start();

// play drum hits
void dm_playSynths();

// play some notes
// void 弹( float 音高, float velocity );


#endif
