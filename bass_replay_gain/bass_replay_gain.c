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
	//We don't actually have anything to do but most plugins have an Init.
	if (is_initialized) {
#if _DEBUG
		printf("Already intialized.\n");
#endif
	}
	is_initialized = TRUE;
#if _DEBUG
	printf("BASS REPLAY_GAIN initialized.\n");
#endif
	return TRUE;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Process)(HSTREAM Handle, REPLAY_GAIN_INFO* Result) {
	REPLAY_GAIN_CONTEXT contexts[1] = { 0 };

	//Make sure no garbage is returned to the caller.
	Result->handle = 0;
	Result->peak = 0;
	Result->gain = 0;

	//Check we're initialized.
	if (!is_initialized) {
#if _DEBUG
		printf("Not intialized.\n");
#endif
		return FALSE;
	}

	//Attempt to process the channel.
	if (!processor_process_channel(contexts, 1, Handle)) {
#if _DEBUG
		printf("Failed to process channel %d.\n", Handle);
#endif
		return FALSE;
	}

	//Success.
	Result->handle = Handle;
	Result->peak = contexts[0].peak;
	Result->gain = contexts[0].gain;

	return TRUE;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_ProcessBatch)(HSTREAM Handles[BATCH_SLOTS], REPLAY_GAIN_BATCH_INFO* Result) {
	DWORD position = 0;
	//Create two contexts, the first is the batch context and the second is the channel context.
	REPLAY_GAIN_CONTEXT contexts[2] = { 0 };

	//Make sure no garbage is returned to the caller.
	for (position = 0; position < BATCH_SLOTS; position++) {
		memset(&Result->items[position], 0, sizeof(REPLAY_GAIN_INFO));
	}
	Result->peak = 0;
	Result->gain = 0;

	//Check we're initialized.
	if (!is_initialized) {
#if _DEBUG
		printf("Not intialized.\n");
#endif
		return FALSE;
	}

	//For each channel handle in the batch...
	for (position = 0; position < BATCH_SLOTS; position++) {

		if (!Handles[position]) {
			//No more channel handles, all done.
			break;
		}

		//Zero the channel context, it will be re-initialized.
		memset(&contexts[1], 0, sizeof(REPLAY_GAIN_CONTEXT));

		//Attempt to process the channel.
		if (!processor_process_channel(contexts, 2, Handles[position])) {
#if _DEBUG
			printf("Failed to process channel %d.\n", Handles[position]);
#endif
			return FALSE;
		}

		//Success.
		Result->items[position].handle = Handles[position];
		Result->items[position].peak = contexts[1].peak;
		Result->items[position].gain = contexts[1].gain;
	}

	//All channels were successful, these are the batch values for peak and gain.
	Result->peak = contexts[0].peak;
	Result->gain = contexts[0].gain;

	return TRUE;
}

BOOL BASSREPLAYGAINDEF(BASS_REPLAY_GAIN_Free)() {
	//We don't actually have anything to do but most plugins have a Free.
	BOOL success = TRUE;
	if (!is_initialized) {
#if _DEBUG
		printf("Not intialized.\n");
#endif
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