#ifndef __AUDIO_PLAYER_H__
#define __AUDIO_PLAYER_H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#include "AudioCodec.h"
#include "JitterBuffer.h"

#define SAMPLE_RATE 8000
#define FRAME_RATE  50
#define FRAME_SIZE  SAMPLE_RATE / FRAME_RATE

class AudioPlayer {

private:
 JitterBuffer &jitterBuffer;
 AudioCodec &audioCodec;

 int sampleRate;
 int bufferFrames;

 SLObjectItf bqPlayerObject;
 SLPlayItf   bqPlayerPlay;

 SLObjectItf outputMixObject;

 SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
 short outputBuffer[FRAME_SIZE];

public:
  AudioPlayer(int sampleRate, int bufferFrames, JitterBuffer &jitterBuffer, AudioCodec &audioCodec);
  int start(SLEngineItf *engineEngine);
  static void playerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context);
  void playerCallback(SLAndroidSimpleBufferQueueItf bufferQueue);
};

#endif