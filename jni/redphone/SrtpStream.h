#ifndef __STRP_STREAM_H__
#define __STRP_STREAM_H__

#include <openssl/aes.h>
#include <openssl/hmac.h>

#define SRTP_MAC_SIZE  20

#include "RtpPacket.h"

class SrtpStreamParameters {

public:
  uint8_t *cipherKey;
  uint8_t *macKey;
  uint8_t *salt;

  SrtpStreamParameters(uint8_t *cipherKey, uint8_t* macKey, uint8_t *salt) :
    cipherKey(cipherKey), macKey(macKey), salt(salt)
  {}

};

class SrtpStream {

private:
  SrtpStreamParameters &parameters;
  AES_KEY key;

  void setIv(int64_t logicalSequence, uint32_t ssrc, uint8_t *salt, uint8_t *iv);

public:

  SrtpStream(SrtpStreamParameters &parameters);

  int init();
  int decrypt(RtpPacket &packet, int64_t logicalSequence);
  int encrypt(RtpPacket &packet, int64_t logicalSequence);

};

#endif