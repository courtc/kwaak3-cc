/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

struct audio_cb {
	void (* fn)(struct audio_cb *);
	void *opaque;
};

struct audio_mixer {
	SLObjectItf o;
	SLEnvironmentalReverbItf reverb;
};

struct audio_buffer {
	SLObjectItf o;
	SLPlayItf player;
	SLAndroidSimpleBufferQueueItf queue;
	SLEffectSendItf effect;
	int isPlaying;
	short *data;
	int length;

	struct audio_buffer *next;
};

struct audio_engine {
	SLObjectItf o;
	SLEngineItf engine;
	struct audio_mixer mixer;
	struct audio_buffer *buffer;
};

int audio_engine_create(struct audio_engine *e);
void audio_engine_destroy(struct audio_engine *e);

int audio_engine_queue(struct audio_engine *e, const short *data, int length);

int audio_buffer_create(struct audio_engine *e, struct audio_buffer **b);
void audio_buffer_destroy(struct audio_engine *e, struct audio_buffer *b);
int audio_buffer_queue(struct audio_buffer *b, const short *data, int length);

static struct audio_engine simple_audio;
static void *simple_audio_buffer;
static void (*simple_audio_idle)(void);

int simple_audio_init(void *buffer, int size, void (*idle)(void))
{
	int rc;
	simple_audio_buffer = buffer;
	simple_audio_idle = idle;

	rc = audio_engine_create(&simple_audio);

	if (simple_audio_idle) {
		(* simple_audio_idle)();
		(* simple_audio_idle)();
	}
	return rc;
}

void simple_audio_fini(void)
{
	audio_engine_destroy(&simple_audio);
}

int simple_audio_write(int offset, int length)
{
	int rc;
	rc = audio_engine_queue(&simple_audio, (const short *)((const char *)simple_audio_buffer + offset), length);
	return rc;
}

int simple_audio_position(void)
{
	return 0;
}

// aux effect on the output mix, used by the buffer queue player
static const SLEnvironmentalReverbSettings reverbSettings =
		SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

// this callback handler is called every time a buffer finishes playing
static void buffer_finished(SLAndroidSimpleBufferQueueItf bq, void *context)
{
	struct audio_buffer *b = (struct audio_buffer *)context;

	if (simple_audio_idle)
		(* simple_audio_idle)();
	b->isPlaying = 0;
}

int audio_engine_create(struct audio_engine *e)
{
	SLresult result;

	memset(e, 0, sizeof(*e));

	result = slCreateEngine(&e->o, 0, NULL, 0, NULL, NULL);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	result = (*e->o)->Realize(e->o, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	result = (*e->o)->GetInterface(e->o, SL_IID_ENGINE, &e->engine);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// create output mix, with environmental reverb specified as a
	// non-required interface
	const SLInterfaceID ids[1] = { SL_IID_ENVIRONMENTALREVERB };
	const SLboolean req[1] = { SL_BOOLEAN_FALSE };
	result = (*e->engine)->CreateOutputMix(e->engine, &e->mixer.o, 1, ids, req);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	result = (*e->mixer.o)->Realize(e->mixer.o, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// get the environmental reverb interface this could fail if the
	// environmental reverb effect is not available, either because the
	// feature is not present, excessive CPU load, or the required
	// MODIFY_AUDIO_SETTINGS permission was not requested and granted
	result = (*e->mixer.o)->GetInterface(e->mixer.o,
					     SL_IID_ENVIRONMENTALREVERB,
					     &e->mixer.reverb);
	if (SL_RESULT_SUCCESS == result) {
		result = (*e->mixer.reverb)->SetEnvironmentalReverbProperties
				(e->mixer.reverb, &reverbSettings);
	}

	return 0;
}

void audio_engine_destroy(struct audio_engine *e)
{
	struct audio_buffer *b, *lb;

	b = e->buffer;
	while (b != NULL) {
		lb = b;
		b = b->next;
		if (lb->o != NULL)
			(*lb->o)->Destroy(lb->o);
		free(lb);
	}

	if (e->mixer.o != NULL)
		(*e->mixer.o)->Destroy(e->mixer.o);
	if (e->o != NULL)
		(*e->o)->Destroy(e->o);

	memset(e, 0, sizeof(*e));
}

void audio_buffer_destroy(struct audio_engine *e, struct audio_buffer *b)
{
	struct audio_buffer *p, *lb;

	if (b == NULL)
		return;

	lb = NULL;
	p = e->buffer;
	while (p != NULL) {
		if (p == b) {
			if (lb != NULL)
				lb->next = p->next;
			else
				e->buffer = p->next;

			if (p->o != NULL)
				(*p->o)->Destroy(p->o);
			free(p);
			break;
		}
		lb = p;
		p = p->next;
	}
}

int audio_buffer_create(struct audio_engine *e, struct audio_buffer **b)
{
	SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 8 };
	SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, e->mixer.o };
	SLDataSink audioSnk = { &loc_outmix, NULL };
	SLDataFormat_PCM format_pcm = {
		.formatType	= SL_DATAFORMAT_PCM,
		.numChannels	= 2,
		.samplesPerSec	= SL_SAMPLINGRATE_44_1,
		.bitsPerSample	= SL_PCMSAMPLEFORMAT_FIXED_16,
		.containerSize	= SL_PCMSAMPLEFORMAT_FIXED_16,
		.channelMask	= SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
		.endianness	= SL_BYTEORDER_LITTLEENDIAN
	};
	SLDataSource audioSrc = { &loc_bufq, &format_pcm };
	struct audio_buffer *ret;
	SLresult result;

	// configure audio sink

	ret = calloc(1, sizeof(*ret));
	// create audio player
	const SLInterfaceID ids[2] = { SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND };
	const SLboolean req[2] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

	result = (*e->engine)->CreateAudioPlayer(e->engine, &ret->o, &audioSrc, &audioSnk, 2, ids, req);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// realize the player
	result = (*ret->o)->Realize(ret->o, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// get the play interface
	result = (*ret->o)->GetInterface(ret->o, SL_IID_PLAY, &ret->player);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// get the buffer queue interface
	result = (*ret->o)->GetInterface(ret->o, SL_IID_BUFFERQUEUE, &ret->queue);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// register callback on the buffer queue
	result = (*ret->queue)->RegisterCallback(ret->queue, buffer_finished, ret);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// get the effect send interface
	result = (*ret->o)->GetInterface(ret->o, SL_IID_EFFECTSEND, &ret->effect);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	// set the player's state to playing
	result = (*ret->player)->SetPlayState(ret->player, SL_PLAYSTATE_PLAYING);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	if (b != NULL)
		*b = ret;

	ret->next = e->buffer;
	e->buffer = ret;

	return 0;
}

void audio_engine_enable_reverb(struct audio_engine *e, struct audio_buffer *b, int enabled)
{
	SLresult result;

	// we might not have been able to add environmental reverb to the output mix
	if (e->mixer.reverb == NULL)
		return;

	(*b->effect)->EnableEffectSend(b->effect, e->mixer.reverb, (SLboolean)enabled, (SLmillibel)0);
}

int audio_buffer_queue(struct audio_buffer *b, const short *data, int length)
{
	SLresult result;

	if (b == NULL)
		return -1;

	b->isPlaying = 1;
	if (b->length < length) {
		b->data = realloc(b->data, length);
		b->length = length;
	}
	memcpy(b->data, data, length);

	result = (*b->queue)->Enqueue(b->queue, b->data, length);
	if (result != SL_RESULT_SUCCESS)
		return -1;

	return 0;
}

int audio_engine_queue(struct audio_engine *e, const short *data, int length)
{
	struct audio_buffer *b;
	int rc;

	if (e->buffer == NULL) {
		rc = audio_buffer_create(e, NULL);
		if (rc)
			return rc;
	}

	b = e->buffer;
	while (b != NULL) {
		if (!b->isPlaying)
			return audio_buffer_queue(b, data, length);
		b = b->next;
	}

	// None not playing, enqueue in first
	return audio_buffer_queue(e->buffer, data, length);
}


