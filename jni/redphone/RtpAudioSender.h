#ifndef __RTP_AUDIO_SENDER_H__
#define __RTP_AUDIO_SENDER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "srtp.h"

class RtpAudioSender {
private:
  int      socketFd;
  uint32_t sequenceNumber;

  struct sockaddr_in *sockAddr;
  int                sockAddrLen;

  srtp_t        session;
  srtp_policy_t policy;

public:
  RtpAudioSender(int socketFd, struct sockaddr_in *sockAddr, int sockAddrLen, char* masterKey);
  ~RtpAudioSender();

  int init();
  int send(int timestamp, char *encodedData, int encodedDataLen);

};


#endif