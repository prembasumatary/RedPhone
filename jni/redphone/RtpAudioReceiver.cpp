#include "RtpAudioReceiver.h"

#include <android/log.h>

#define TAG "RtpAudioReceiver"

RtpAudioReceiver::RtpAudioReceiver(int socketFd, struct sockaddr_in *sockAddr, int sockAddrLen, char* masterKey) :
  socketFd(socketFd), sockAddr(sockAddr), sockAddrLen(sockAddrLen)
{
  crypto_policy_set_rtp_default(&policy.rtp);
  crypto_policy_set_rtcp_default(&policy.rtcp);

  policy.ssrc.type  = ssrc_specific;
  policy.ssrc.value = 0;
  policy.key        = (unsigned char*)masterKey;
  policy.next       = NULL;

  srtp_create(&session, &policy);
}

RtpPacket* RtpAudioReceiver::receive(char* encodedData, int encodedDataLen) {
//  __android_log_print(ANDROID_LOG_WARN, TAG, "Calling recvfrom()");

  int received = recv(socketFd, encodedData, encodedDataLen, 0);

//  __android_log_print(ANDROID_LOG_WARN, TAG, "Received data %d", received);

  int unprotect_result = srtp_unprotect(session, encodedData, &received);

//   __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_unprotect() result %d vs %d", unprotect_result, err_status_ok);
//  __android_log_print(ANDROID_LOG_WARN, TAG, "New data length %d", received);

  return new RtpPacket(encodedData, received);
}