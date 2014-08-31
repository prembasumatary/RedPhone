
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

CallAudioManager::CallAudioManager(int androidSdkVersion, int socketFd, struct sockaddr_in *sockAddr,
                                   SrtpStreamParameters &senderParameters, SrtpStreamParameters &receiverParameters)
  : engineObject(NULL), engineEngine(NULL), audioCodec(),
    audioSender(socketFd, sockAddr, sizeof(struct sockaddr_in), senderParameters),
    audioReceiver(socketFd, sockAddr, sizeof(struct sockaddr_in), receiverParameters),
    webRtcJitterBuffer(audioCodec), microphoneReader(androidSdkVersion, audioCodec, audioSender),
    audioPlayer(webRtcJitterBuffer, audioCodec)
{
}

CallAudioManager::~CallAudioManager() {
//  if (srtp_initialized) {
//    srtp_shutdown();
//  }

  microphoneReader.stop();
  audioPlayer.stop();
  webRtcJitterBuffer.stop();

  if (engineObject != NULL) {
    (*engineObject)->Destroy(engineObject);
  }
}

int CallAudioManager::run() {
  running = 1;

  if (slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL) != SL_RESULT_SUCCESS) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to create engineObject!");
    return -1;
  }

  if ((*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to realize engineObject!");
    return -1;
  }

  if ((*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine) != SL_RESULT_SUCCESS) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to get engine interface!");
    return -1;
  }

//  if (srtp_init() != err_status_ok) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_init failed!");
//    return -1;
//  }
//
//  srtp_initialized = 1;

  if (audioCodec.init() != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize codec!");
    return -1;
  }

  if (audioSender.init() != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize RTP sender!");
    return -1;
  }

  if (audioReceiver.init() != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize RTP receiver!");
    return -1;
  }

  if (webRtcJitterBuffer.init() != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize jitter buffer!");
    return -1;
  }

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

  while(running) {
    RtpPacket *packet = audioReceiver.receive(buffer, sizeof(buffer));

    if (packet != NULL) {
      webRtcJitterBuffer.addAudio(packet);
      delete packet;
    }
  }

  return 0;
}

//static int startAudio(int androidSdkVersion, int sampleRate, int bufferFrames,
//                      int socketFd, const char* serverIp, int serverPort,
//                      char* masterKey)
//{
//  __android_log_print(ANDROID_LOG_WARN, TAG, "CallAudioManager::startAudio(%d, %d, %s, %d)",
//                      androidSdkVersion, socketFd, serverIp, serverPort);
//  running = 1;
//
//  struct sockaddr_in sockAddr;
//  SLObjectItf        engineObject = NULL;
//  SLEngineItf        engineEngine = NULL;
//  int                result       = -1;
//
//  memset((void*)&sockAddr, 0, sizeof(sockAddr));
//  sockAddr.sin_family = AF_INET;
//  sockAddr.sin_port   = htons(serverPort);
//
//  if (inet_aton(serverIp, &sockAddr.sin_addr) == 0) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Invalid address: %s", serverIp);
//    goto failure;
//  }
//
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Destination addr: %d", sockAddr.sin_addr.s_addr);
//
//  if (slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL) != SL_RESULT_SUCCESS) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to create engineObject!");
//    goto failure;
//  }
//
//  if ((*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to realize engineObject!");
//    goto failure;
//  }
//
//  if ((*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine) != SL_RESULT_SUCCESS) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to get engine interface!");
//    goto failure;
//  }
//
//  if (srtp_init() != err_status_ok) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_init failed!");
//    goto failure;
//  }
//
//  AudioCodec audioCodec;
//
//  if (audioCodec.init() != 0) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize codec!");
//    goto failure;
//  }
//
//  RtpAudioSender audioSender(socketFd, &sockAddr, sizeof(sockAddr), masterKey);
//
//  if (audioSender.init() != 0) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize RTP sender!");
//    goto failure;
//  }
//
//  RtpAudioReceiver audioReceiver(socketFd, &sockAddr, sizeof(sockAddr), masterKey);
//
//  if (audioReceiver.init() != 0) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize RTP receiver!");
//    goto failure;
//  }
//
//  WebRtcJitterBuffer webRtcJitterBuffer(audioCodec);
//
//  if (webRtcJitterBuffer.init() != 0) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Failed to initialize jitter buffer!");
//    goto failure;
//  }
//
//  SequenceCounter  sequenceCounter;
//  MicrophoneReader microphoneReader(androidSdkVersion, audioCodec, audioSender);
//  AudioPlayer      audioPlayer(sampleRate, bufferFrames, webRtcJitterBuffer, audioCodec);
//
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Starting MicrophoneReader...");
//
//  if (microphoneReader.start(&engineEngine) == -1) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "ERROR -- MicrophoneReader::start() returned -1!");
//    goto failure;
//  }
//
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Starting AudioPlayer...");
//
//  if (audioPlayer.start(&engineEngine) == -1) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "AudioPlayer::start() returned -1!");
//    goto failure;
//  }
//
//  char buffer[4096];
//
//  while(running) {
//    RtpPacket *packet = audioReceiver.receive(buffer, sizeof(buffer));
//
//    if (packet != NULL) {
//      webRtcJitterBuffer.addAudio(packet);
//      delete packet;
//    }
//  }
//
//  result = 0;
//  goto cleanup;
//
//  failure:
//    result = -1;
//
//  cleanup:
//    microphoneReader.stop();
//    audiPlayer.stop();
//    webRtcJitterBuffer.stop();
//
//    if (engineObject != NULL) {
//      (*engineObject)->Destroy(engineObject);
//    }
//
//  return result;
//}

JNIEXPORT void JNICALL Java_org_thoughtcrime_redphone_audio_CallAudioManager2_start
  (JNIEnv *env, jobject obj, jint androidSdkVersion, jint sampleRate, jint bufferFrames,
   jint socketFd, jstring serverIpString, jint serverPort,
   jbyteArray senderCipherKey, jbyteArray senderMacKey, jbyteArray senderSalt,
   jbyteArray receiverCipherKey, jbyteArray receiverMacKey, jbyteArray receiverSalt)
{
  struct sockaddr_in sockAddr;
  int result;

  SrtpStreamParameters senderParameters((uint8_t*)env->GetByteArrayElements(senderCipherKey, 0),
                                        (uint8_t*)env->GetByteArrayElements(senderMacKey, 0),
                                        (uint8_t*)env->GetByteArrayElements(senderSalt, 0));

  SrtpStreamParameters receiverParameters((uint8_t*)env->GetByteArrayElements(receiverCipherKey, 0),
                                          (uint8_t*)env->GetByteArrayElements(receiverMacKey, 0),
                                          (uint8_t*)env->GetByteArrayElements(receiverSalt, 0));


  const char* serverIp = env->GetStringUTFChars(serverIpString, 0);

  memset((void*)&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_port   = htons(serverPort);

  if (inet_aton(serverIp, &sockAddr.sin_addr) == 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Invalid address: %s", serverIp);
    result = -1;
  } else {
    CallAudioManager callAudioManager(androidSdkVersion, socketFd, &sockAddr,
                                      senderParameters, receiverParameters);
    result = callAudioManager.run();
  }


//
//  int result = startAudio(androidSdkVersion, sampleRate, bufferFrames,
//                          socketFd, serverIp, serverPort, masterKeyBytes);
//
//  cleanup:

//  env->ReleaseByteArrayElements(masterKey, (jbyte*)masterKeyBytes, 0);

  env->ReleaseByteArrayElements(senderCipherKey, (jbyte*)senderParameters.cipherKey, 0);
  env->ReleaseByteArrayElements(senderMacKey, (jbyte*)senderParameters.macKey, 0);
  env->ReleaseByteArrayElements(senderSalt, (jbyte*)senderParameters.salt, 0);

  env->ReleaseByteArrayElements(receiverCipherKey, (jbyte*)receiverParameters.cipherKey, 0);
  env->ReleaseByteArrayElements(receiverMacKey, (jbyte*)receiverParameters.macKey, 0);
  env->ReleaseByteArrayElements(receiverSalt, (jbyte*)receiverParameters.salt, 0);

  env->ReleaseStringUTFChars(serverIpString, serverIp);

  if (result == -1) {
   env->ThrowNew(env->FindClass("org/thoughtcrime/redphone/audio/NativeAudioException"),
                                "Failed to initialize native audio");
  }
}

JNIEXPORT void JNICALL Java_org_thoughtcrime_redphone_audio_CallAudioManager2_stop
(JNIEnv *env, jobject obj)
{
  running = 0;
  __android_log_print(ANDROID_LOG_WARN, TAG, "Set running to 0!");
}

