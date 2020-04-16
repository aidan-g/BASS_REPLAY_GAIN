#include <stdio.h>
#include "../bass_replay_gain/bass_replay_gain.h"

int main(int argc, char** argv) {
	DWORD handle;
	REPLAY_GAIN_INFO info;

	if (!BASS_Init(0, 44100, 0, 0, NULL)) {
		printf("Failed to initialize BASS: %d\n", BASS_ErrorGetCode());
		return 1;
	}

	if (!BASS_REPLAY_GAIN_Init()) {
		printf("Failed to initialize GAPLESS.\n");
		return 1;
	}

	handle = BASS_StreamCreateFile(FALSE, "..\\Media\\16 Turn Up the Silence.m4a", 0, 0, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);
	if (handle == 0) {
		printf("Failed to create stream: %d\n", BASS_ErrorGetCode());
		return 1;
	}

	if (!BASS_REPLAY_GAIN_Process(handle, &info)) {
		printf("Failed to process stream: %d\n", BASS_ErrorGetCode());
		return 1;
	}

	printf("Gain: %f\n", info.gain);
	printf("Peak: %f\n", info.peak);

	BASS_REPLAY_GAIN_Free();
	BASS_Free();
}