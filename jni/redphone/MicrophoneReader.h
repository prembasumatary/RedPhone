#ifndef __MICROPHONE_READER_H__
#define __MICROPHONE_READER_H__

#include "AudioCodec.h"
#include "RtpAudioSender.h"

#include <jni.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#define SAMPLE_RATE 8000
#define FRAME_RATE  50
#define FRAME_SIZE  SAMPLE_RATE / FRAME_RATE

class MicrophoneReader {
  private:
    int androidSdkVersion;
    AudioCodec &audioCodec;
    RtpAudioSender &rtpAudioSender;

//    int sampleRate;
//    int bufferFrames;
    short inputBuffer[FRAME_SIZE];
    char encodedAudio[1024];

    SLObjectItf recorderObject;
    SLRecordItf recorderRecord;

    SLAndroidConfigurationItf androidConfig;

    SLAndroidSimpleBufferQueueItf recorderBufferQueue;

    //SLuint32 convertSampleRate(SLuint32 rate);

  public:
    MicrophoneReader(int androidSdkVersion, AudioCodec &audioCodec, RtpAudioSender &rtpAudioSender);
    ~MicrophoneReader();

    int start(SLEngineItf *engineEngine);
    void recorderCallback(SLAndroidSimpleBufferQueueItf bufferQueue);
    static void recorderCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void* context);

};

#endif