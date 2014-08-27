#ifndef __AUDIO_CODEC_H__
#define __AUDIO_CODEC_H__

#include <sys/types.h>
#include <speex/speex.h>

class AudioCodec {

private:
  void *enc; //speex encoder
  void *dec; //speex decoder

  SpeexBits enc_bits, dec_bits;

  int enc_frame_size, dec_frame_size;

public:
  AudioCodec();
  ~AudioCodec();
  int encode(short *rawData, char* encodedData, int encodedDataLen);
  int decode(char* encodedData, int encodedDataLen, short* rawData);

};

#endif