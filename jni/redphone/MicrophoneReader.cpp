#include "MicrophoneReader.h"
#include "SampleRateUtil.h"

#include <jni.h>

#include <android/log.h>

#define TAG "MicrophoneReader"

#ifndef SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION
#define SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION ((SLuint32) 0x00000004)
#endif

MicrophoneReader::MicrophoneReader(int androidSdkVersion, AudioCodec &audioCodec, RtpAudioSender &rtpAudioSender) :
  androidSdkVersion(androidSdkVersion), audioCodec(audioCodec), rtpAudioSender(rtpAudioSender)
{
//  int inputBufferFrames = (sampleRate / bufferFrames / 4) * bufferFrames;
//
//  inputBuffer = (short *) calloc(inputBufferFrames, sizeof(short));
}

MicrophoneReader::~MicrophoneReader() {
//  free(inputBuffer);
}

void MicrophoneReader::recorderCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
  MicrophoneReader* microphoneReader = static_cast<MicrophoneReader*>(context);
  microphoneReader->recorderCallback(bufferQueue);
}

void MicrophoneReader::recorderCallback(SLAndroidSimpleBufferQueueItf bufferQueue)
{
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Got recorder callback!");
  int encodedAudioLen = audioCodec.encode(inputBuffer, encodedAudio, sizeof(encodedAudio));
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Successfully encoded %d bytes of audio", encodedAudioLen);

  rtpAudioSender.send(encodedAudio, encodedAudioLen);
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Successfully sent via RTP...");

  (*bufferQueue)->Enqueue(bufferQueue, inputBuffer, FRAME_SIZE * sizeof(short));
}

int MicrophoneReader::start(SLEngineItf *engineEnginePtr) {
  SLEngineItf engineEngine = *engineEnginePtr;

  SLDataLocator_AndroidSimpleBufferQueue loc_bq     = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1};
  SLDataFormat_PCM                       format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_8,
                                                       SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                                       SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};

  SLDataLocator_IODevice loc_dev  = {SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT, SL_DEFAULTDEVICEID_AUDIOINPUT, NULL};

  SLDataSource audioSrc = {&loc_dev, NULL};
  SLDataSink   audioSnk = {&loc_bq, &format_pcm};

  const SLInterfaceID id[2]  = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};
  const SLboolean     req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

  if ((*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject, &audioSrc,
                                           &audioSnk, 2, id, req) != SL_RESULT_SUCCESS)
  {
    return -1;
  }

  if ((*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDCONFIGURATION, &androidConfig) == SL_RESULT_SUCCESS) {
    SLint32 recordingPreset = SL_ANDROID_RECORDING_PRESET_GENERIC;

    if (androidSdkVersion >= 14) {
      recordingPreset = SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION;
    }

    (*androidConfig)->SetConfiguration(androidConfig, SL_ANDROID_KEY_RECORDING_PRESET,
                                       &recordingPreset, sizeof(SLint32));
  }

  if ((*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recorderRecord) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &recorderBufferQueue) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*recorderBufferQueue)->RegisterCallback(recorderBufferQueue, &MicrophoneReader::recorderCallback, this) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*recorderBufferQueue)->Enqueue(recorderBufferQueue, inputBuffer, FRAME_SIZE * sizeof(short)) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_RECORDING) != SL_RESULT_SUCCESS) {
    return -1;
  }

  return 0;
}