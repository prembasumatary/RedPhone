#include "RtpAudioReceiver.h"

#include <android/log.h>

#define TAG "RtpAudioReceiver"

RtpAudioReceiver::RtpAudioReceiver(int socketFd, struct sockaddr_in *sockAddr, int sockAddrLen, SrtpStreamParameters &parameters) :
  socketFd(socketFd), sockAddr(sockAddr), sockAddrLen(sockAddrLen), sequenceCounter(), srtpStream(parameters)
{
//  crypto_policy_set_rtp_default(&policy.rtp);
//  crypto_policy_set_rtcp_default(&policy.rtcp);
//
//  policy.ssrc.type  = ssrc_specific;
//  policy.ssrc.value = 0;
//  policy.key        = (unsigned char*)masterKey;
//  policy.next       = NULL;
}

//RtpAudioReceiver::~RtpAudioReceiver() {
//  if (initialized && session != NULL) {
//    srtp_dealloc(session);
//  }
//}

int RtpAudioReceiver::init() {
  if (srtpStream.init() != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP stream failed to initialize!");
    return -1;
  }
//  if (srtp_create(&session, &policy) != err_status_ok) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_create failed!");
//    return -1;
//  }

  return 0;
}

RtpPacket* RtpAudioReceiver::receive(char* encodedData, int encodedDataLen) {
  int received = recv(socketFd, encodedData, encodedDataLen, 0);

  if (received == -1) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "recv() failed!");
    return NULL;
  }

  RtpPacket *packet = new RtpPacket(encodedData, received);

  if (srtpStream.decrypt(*packet, sequenceCounter.convertNext(packet->getSequenceNumber())) != 0) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP decrypt failed!");
    delete packet;
    return NULL;
  }


//  uint16_t sequenceNumber = packet->getSequenceNumber();
//  char*    payload        = packet->getPayload();
//  int      payloadLen     = packet->getPayloadLen();
//
//    __android_log_print(ANDROID_LOG_WARN, TAG, "Sequence number: %d", sequenceNumber);
//
//  if (srtpStream.decrypt(sequenceCounter.convertNext(sequenceNumber),
//                         packet->getSsrc(), (uint8_t*)payload, (uint32_t*)&payloadLen) != 0)
//  {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "SRTP decrypt failed!");
//    delete packet;
//    return NULL;
//  }

//  packet->setPayloadLen(payloadLen);

  return packet;

//  if (srtp_unprotect(session, encodedData, &received) != err_status_ok) {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "srtp_unprotect() failed!");
//    return NULL;
//  }
//
//  return new RtpPacket(encodedData, received);
}