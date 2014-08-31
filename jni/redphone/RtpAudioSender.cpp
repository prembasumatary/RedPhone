#include "RtpAudioSender.h"
#include "RtpPacket.h"

#include <android/log.h>
#include <errno.h>

#define TAG "RtpAudioSender"

RtpAudioSender::RtpAudioSender(int socketFd, struct sockaddr_in *sockAddr, int sockAddrLen,
                               SrtpStreamParameters &parameters) :
  socketFd(socketFd), sequenceNumber(0), sockAddr(sockAddr), sockAddrLen(sockAddrLen),
  srtpStream(parameters)
{
//  crypto_policy_set_rtp_default(&policy.rtp);
//  crypto_policy_set_rtcp_default(&policy.rtcp);
//
//  policy.ssrc.type  = ssrc_specific;
//  policy.ssrc.value = 0;
//  policy.key        = (unsigned char*)masterKey;
//  policy.next       = NULL;
}

int RtpAudioSender::init() {
  if (srtpStream.init() != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP stream failed!");
    return -1;
  }
//  int result;
//
//  if ((result = srtp_create(&session, &policy)) != err_status_ok) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_create failed! %d", result);
//    return -1;
//  }
//
//  initialized = 1;
//
  return 0;
}

//RtpAudioSender::~RtpAudioSender() {
//  if (initialized && session != NULL) {
//    srtp_dealloc(session);
//  }
//}

int RtpAudioSender::send(int timestamp, char* encodedData, int encodedDataLen) {
  RtpPacket packet(encodedData, encodedDataLen, sequenceNumber, timestamp);

  if (srtpStream.encrypt(packet, sequenceNumber++) != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP encrypt() failed!");
    return -1;
  }


//  if (srtpStream.encrypt(sequenceNumber++, 0, (uint8_t*)encodedData, (uint32_t*)&encodedDataLen) != 0) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP encrypt() failed!");
//    return -1;
//  }



  char* serializedPacket    = packet.getSerializedPacket();
  int   serializedPacketLen = packet.getSerializedPacketLen();

//  if (srtpStream.encrypt(sequenceCounter.getNextLogicalSequence(sequenceNumber),
//                         packet.getSsrc(), serializedPacket, &serializedPacketLen) != 0)
//  {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP encrypt() failed!");
//    return -1;
//  }

//  if (srtp_protect(session, serializedPacket, &serializedPacketLen) != err_status_ok) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_protect() failed!");
//    return -1;
//  }

  if (sendto(socketFd, serializedPacket, serializedPacketLen, 0,
             (struct sockaddr*)sockAddr, sockAddrLen) == -1)
  {
    __android_log_print(ANDROID_LOG_WARN, TAG, "sendto() failed!");
    return -1;
  }

  return 0;
}