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

  srtp_create(&session, &policy);
}

RtpAudioSender::~RtpAudioSender() {
  srtp_dealloc(session);
}

int RtpAudioSender::send(char* encodedData, int encodedDataLen) {
  RtpPacket packet(encodedData, encodedDataLen, sequenceNumber++);

  char* serializedPacket    = packet.getSerializedPacket();
  int   serializedPacketLen = packet.getSerializedPacketLen();

  int protect_result = srtp_protect(session, serializedPacket, &serializedPacketLen);

//  __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_protect() result: %d", protect_result);
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Total packet length: %d", serializedPacketLen);
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Socket FD: %d", socketFd);
//  __android_log_print(ANDROID_LOG_WARN, TAG, "SockAddrLen: %d", sockAddrLen);
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Destination port: %d", ntohs(sockAddr->sin_port));
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Destination addr: %d", sockAddr->sin_addr.s_addr);


  int sendto_result = sendto(socketFd, serializedPacket, serializedPacketLen, 0,
                             (struct sockaddr*)sockAddr, sockAddrLen);

//  __android_log_print(ANDROID_LOG_WARN, TAG, "sendto_result result: %d, errno: %d", sendto_result, errno);

  return sendto_result;
}