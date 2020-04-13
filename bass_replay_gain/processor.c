#include "processor.h"

BOOL processor_process_channel(REPLAY_GAIN_CONTEXT* contexts, DWORD context_count, HSTREAM handle) {
	DWORD length;
	DWORD count;
	PFLOAT samples_input;
	PFLOAT samples_output;
	DWORD sample_count;
	BASS_CHANNELINFO info;

	if (!BASS_ChannelGetInfo(handle, &info)) {
		return FALSE;
	}

	if (!processor_init_contexts(contexts, context_count, info.chans, info.freq)) {
		//Failed to initialize contexts, the sample rate is probably not supported.
		return FALSE;
	}

	length = info.freq / 10;
	samples_input = calloc(length, sizeof(FLOAT));
	if (!samples_input) {
		//Out of memory.
		return FALSE;
	}
	samples_output = calloc(length, sizeof(FLOAT));
	if (!samples_output) {
		//Out of memory.
		return FALSE;
	}

	length = (length * sizeof(FLOAT)) | BASS_DATA_FLOAT;

	do {
		count = BASS_ChannelGetData(handle, samples_input, length);

		if (count > 0) {

			if (count == BASS_STREAMPROC_END) {
				break;
			}
			if (count == BASS_ERROR_UNKNOWN) {
				break;
			}

			sample_count = count / sizeof(FLOAT);
			if (!processor_process_samples(contexts, context_count, samples_input, samples_output, sample_count)) {
				//Processor failed.
				count = -1;
				break;
			}
		}
	} while (count > 0);

	free(samples_input);
	free(samples_output);

	if (count < 0) {
		return FALSE;
	}

	if (!processor_calc_replaygain(contexts, context_count)) {
		return FALSE;
	}

	return TRUE;
}

BOOL processor_init_contexts(REPLAY_GAIN_CONTEXT* contexts, DWORD context_count, DWORD channel_count, DWORD sample_rate) {
	DWORD position;
	BOOL result = TRUE;
	for (position = 0; position < context_count; position++) {
		if (contexts[position].is_initialized) {
			if (contexts[position].channel_count != channel_count || contexts[position].sample_rate != sample_rate) {
				//Format of context differs. 
				return FALSE;
			}
			continue;
		}
		result = result && scanner_init_context(&contexts[position], channel_count, sample_rate);
	}
	return result;
}

BOOL processor_process_samples(REPLAY_GAIN_CONTEXT* contexts, DWORD context_count, PFLOAT samples_input, PFLOAT samples_output, DWORD sample_count) {
	DWORD position;
	BOOL result = TRUE;
	for (position = 0; position < context_count; position++) {
		contexts[position].samples_input = samples_input;
		contexts[position].samples_output = samples_output;
		result = result && scanner_process_samples(&contexts[position], sample_count);
		contexts[position].samples_input = NULL;
		contexts[position].samples_output = NULL;
	}
	return result;
}

BOOL processor_calc_replaygain(REPLAY_GAIN_CONTEXT* contexts, DWORD context_count) {
	DWORD position;
	BOOL result = TRUE;
	for (position = 0; position < context_count; position++) {
		result = result && scanner_calc_replaygain(&contexts[position]);
	}
	return result;
}