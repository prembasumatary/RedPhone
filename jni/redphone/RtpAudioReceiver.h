#ifndef __RTP_AUDIO_RECEIVER_H__
#define __RTP_AUDIO_RECEIVER_H__

#include "RtpPacket.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//#include <srtp.h>

#include "SequenceCounter.h"
#include "SrtpStream.h"

class RtpAudioReceiver {

private:
  int socketFd;
  struct sockaddr_in *sockAddr;
  int sockAddrLen;

  SequenceCounter sequenceCounter;
  SrtpStream      srtpStream;


//  int initialized;
//
//  srtp_t session;
//  srtp_policy_t policy;

public:
  RtpAudioReceiver(int socketFd, struct sockaddr_in *sockAddr, int sockAddrLen, SrtpStreamParameters *parameters);
//  ~RtpAudioReceiver();
//
  int init();
  RtpPacket* receive(char* encodedData, int encodedDataLen);

};


#endif