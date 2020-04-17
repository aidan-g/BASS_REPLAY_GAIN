#ifndef BASSREPLAYGAIN_H
#define BASSREPLAYGAIN_H

#include "../bass/bass.h"

#ifndef BASSREPLAYGAINDEF
#define BASSREPLAYGAINDEF(f) WINAPI f
#endif

//The maximum number of channel handles that can be submitted as a batch.
#define BATCH_SLOTS 256

typedef struct REPLAY_GAIN_INFO {
	HSTREAM handle;
	FLOAT peak;
	FLOAT gain;
} REPLAY_GAIN_INFO;

typedef struct REPLAY_GAIN_BATCH_INFO {
	FLOAT peak;
	FLOAT gain;
	REPLAY_GAIN_INFO items[BATCH_SLOTS];
} REPLAY_GAIN_BATCH_INFO;

__declspec(dllexport)
BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Init)();

__declspec(dllexport)
BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Process)(HSTREAM Handle, REPLAY_GAIN_INFO* Result);

__declspec(dllexport)
BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_ProcessBatch)(HSTREAM Handles[BATCH_SLOTS], DWORD Length, REPLAY_GAIN_BATCH_INFO* Result);

__declspec(dllexport)
BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Free)();

#endif