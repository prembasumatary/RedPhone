#include "RtpAudioSender.h"
#include "RtpPacket.h"

#include <android/log.h>
#include <errno.h>

#define TAG "RtpAudoSender"

RtpAudioSender::RtpAudioSender(int socketFd, struct sockaddr_in *sockAddr, int sockAddrLen, char* masterKey) :
  socketFd(socketFd), sequenceNumber(0), sockAddr(sockAddr), sockAddrLen(sockAddrLen)
{
  crypto_policy_set_rtp_default(&policy.rtp);
  crypto_policy_set_rtcp_default(&policy.rtcp);

  policy.ssrc.type  = ssrc_specific;
  policy.ssrc.value = 0;
  policy.key        = (unsigned char*)masterKey;
  policy.next       = NULL;
}

int RtpAudioSender::init() {
  if (srtp_create(&session, &policy) != err_status_ok) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_create failed!");
    return -1;
  }

  return 0;
}

RtpAudioSender::~RtpAudioSender() {
  srtp_dealloc(session);
}

int RtpAudioSender::send(int timestamp, char* encodedData, int encodedDataLen) {
  RtpPacket packet(encodedData, encodedDataLen, sequenceNumber++, timestamp);

  char* serializedPacket    = packet.getSerializedPacket();
  int   serializedPacketLen = packet.getSerializedPacketLen();

  if (srtp_protect(session, serializedPacket, &serializedPacketLen) != err_status_ok) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_protect() failed!");
    return -1;
  }

  if (sendto(socketFd, serializedPacket, serializedPacketLen, 0,
             (struct sockaddr*)sockAddr, sockAddrLen) == -1)
  {
    __android_log_print(ANDROID_LOG_WARN, TAG, "sendto() failed!");
    return -1;
  }

  return 0;
}