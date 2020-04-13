#ifdef _DEBUG
#include <stdio.h>
#endif

#include "bass_replay_gain.h"
#include "processor.h"

BOOL is_initialized = FALSE;

//I have no idea how to prevent linking against this routine in msvcrt.
//It doesn't exist on Windows XP.
//Hopefully it doesn't do anything important.
int _except_handler4_common() {
	return 0;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Init)() {
	is_initialized = TRUE;
#if _DEBUG
	printf("BASS REPLAY_GAIN initialized.\n");
#endif
	return TRUE;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Process)(HSTREAM Handle, REPLAY_GAIN_INFO* Result) {
	BOOL result;
	REPLAY_GAIN_CONTEXT contexts[1] = { 0 };

	if (!is_initialized) {
		return FALSE;
	}

	result = processor_process_channel(contexts, 1, Handle);

	if (result) {
		Result->handle = Handle;
		Result->peak = contexts[0].peak;
		Result->gain = contexts[0].gain;
	}

	return TRUE;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_ProcessBatch)(HSTREAM Handles[BATCH_SLOTS], REPLAY_GAIN_BATCH_INFO* Result) {
	BOOL result;
	DWORD position = 0;
	REPLAY_GAIN_CONTEXT contexts[2] = { 0 };

	if (!is_initialized) {
		return FALSE;
	}

	for (position = 0; position < BATCH_SLOTS; position++) {

		if (!Handles[position]) {
			break;
		}

		memset(&contexts[1], 0, sizeof(REPLAY_GAIN_CONTEXT));
		result = processor_process_channel(contexts, 2, Handles[position]);

		if (result) {
			Result->items[position].handle = Handles[position];
			Result->items[position].peak = contexts[1].peak;
			Result->items[position].gain = contexts[1].gain;
		}
		else {
			return FALSE;
		}
	}

	for (; position < BATCH_SLOTS; position++) {
		memset(&Result->items[position], 0, sizeof(REPLAY_GAIN_INFO));
	}

	if (result) {
		Result->peak = contexts[0].peak;
		Result->gain = contexts[0].gain;
	}

	return TRUE;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Free)() {
	BOOL success = TRUE;
	if (!is_initialized) {
		success = FALSE;
	}
	else {
		//Nothing to do.
	}
	if (success) {
		is_initialized = FALSE;
#if _DEBUG
		printf("BASS REPLAY_GAIN released.\n");
#endif
	}
	else {
#if _DEBUG
		printf("Failed to release BASS REPLAY_GAIN.\n");
#endif
	}
	return success;
}