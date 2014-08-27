
#include "AudioCodec.h"
#include "MicrophoneReader.h"
#include "SequenceCounter.h"
#include "JitterBuffer.h"
#include "RtpAudioReceiver.h"
#include "RtpAudioSender.h"
#include "AudioPlayer.h"

#include "CallAudioManager.h"

#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <jni.h>
#include <android/log.h>

#define TAG "CallAudioManager"

static volatile int running = 0;

static int startAudio(int androidSdkVersion, int sampleRate, int bufferFrames,
                      int socketFd, const char* serverIp, int serverPort,
                      char* masterKey)
{
  __android_log_print(ANDROID_LOG_WARN, TAG, "CallAudioManager::startAudio()");

  struct sockaddr_in sockAddr;
  SLObjectItf        engineObject;
  SLEngineItf        engineEngine;

  memset((void*)&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_port   = htons(serverPort);

  if (inet_aton(serverIp, &sockAddr.sin_addr) == 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Invalid address: %s", serverIp);
    return -1;
  }

  __android_log_print(ANDROID_LOG_WARN, TAG, "Destination addr: %d", sockAddr.sin_addr.s_addr);

  __android_log_print(ANDROID_LOG_WARN, TAG, "Creating engineObject");

  if (slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
    return -1;
  }

  if ((*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine) != SL_RESULT_SUCCESS) {
    return -1;
  }

  __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_init()");
  srtp_init();

  AudioCodec       audioCodec;
  RtpAudioSender   audioSender(socketFd, &sockAddr, sizeof(sockAddr), masterKey);
  MicrophoneReader microphoneReader(androidSdkVersion, audioCodec, audioSender);

  SequenceCounter  sequenceCounter;
  RtpAudioReceiver audioReceiver(socketFd, &sockAddr, sizeof(sockAddr), masterKey);
  WebRtcJitterBuffer webRtcJitterBuffer(audioCodec);
//  JitterBuffer     jitterBuffer;
  AudioPlayer      audioPlayer(sampleRate, bufferFrames, webRtcJitterBuffer, audioCodec);

  __android_log_print(ANDROID_LOG_WARN, TAG, "Starting MicrophoneReader...");

  if (microphoneReader.start(&engineEngine) == -1) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "ERROR -- MicrophoneReader::start() returned -1!");
    return -1;
  }

  __android_log_print(ANDROID_LOG_WARN, TAG, "Starting AudioPlayer...");
  if (audioPlayer.start(&engineEngine) == -1) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "AudioPlayer::start() returned -1!");
    return -1;
  }

  char buffer[4096];

  running = 1;

  while(running) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Waiting for RTP packet...");
    RtpPacket *packet = audioReceiver.receive(buffer, sizeof(buffer));
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Got RTP packet....");
    int16_t  sequenceNumber  = packet->getSequenceNumber();
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Got sequence number: %d", sequenceNumber);
    int64_t  logicalSequence = sequenceCounter.getNextLogicalSequence(sequenceNumber);
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Calculated logical sequence: %lld", logicalSequence);

    webRtcJitterBuffer.addAudio(packet);
//    jitterBuffer.addAudio(logicalSequence, packet->getPayload(), packet->getPayloadLen());

    delete packet;
  }

  return 0;
}
JNIEXPORT jint JNICALL Java_org_thoughtcrime_redphone_audio_CallAudioManager2_stop
(JNIEnv *env, jobject obj)
{
  running = 0;
  return 0;
}

JNIEXPORT void JNICALL Java_org_thoughtcrime_redphone_audio_CallAudioManager2_start
  (JNIEnv *env, jobject obj, jint androidSdkVersion, jint sampleRate, jint bufferFrames,
   jint socketFd, jstring serverIpString, jint serverPort, jbyteArray masterKey)
{

  char* masterKeyBytes = (char*)env->GetByteArrayElements(masterKey, 0);
  const char* serverIp = env->GetStringUTFChars(serverIpString, 0);

  startAudio(androidSdkVersion, sampleRate, bufferFrames,
             socketFd, serverIp, serverPort, masterKeyBytes);

  env->ReleaseByteArrayElements(masterKey, (jbyte*)masterKeyBytes, 0);
  env->ReleaseStringUTFChars(serverIpString, serverIp);
}

