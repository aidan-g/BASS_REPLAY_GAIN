# BASS_REPLAY_GAIN

A replay gain calculator for BASS which uses IIR stereo filters similar to libavfilter.

bass.dll is required for native projects.
ManagedBass is required for .NET projects.

A simple example;

```c
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
		printf("Failed to initialize REPLAY GAIN.\n");
		return 1;
	}

	handle = BASS_StreamCreateFile(FALSE, "test.mp3", 0, 0, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);
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
```

Using `BASS_SAMPLE_FLOAT` is recommended but not required.

`BASS_REPLAY_GAIN_Process` is quite fast, less than a second for a 30MB track. It could be used for playback on the fly (just reset the stream position).

If you wish to apply a volume effect with `BASS_FX` the calculation is `pow(10, gain / 20)`.

A batch mode is also available to calculating album gain: `BASS_REPLAY_GAIN_ProcessBatch`. It accepts set of channel handles (up to 256) and returns a `REPLAY_GAIN_BATCH_INFO` containing information for both individual tracks and the entire batch.

`BASS_REPLAY_GAIN_Process` and `BASS_REPLAY_GAIN_ProcessBatch` are thread safe.


