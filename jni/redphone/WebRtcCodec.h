#ifndef __WEB_RTC_CODEC_H__
#define __WEB_RTC_CODEC_H__

#include "AudioCodec.h"
#include <sys/types.h>

#include <modules/audio_coding/neteq/interface/audio_decoder.h>

class WebRtcCodec : public AudioDecoder {

private:
  AudioCodec &codec;

public:
  WebRtcCodec(AudioCodec &codec) :
    AudioDecoder(kDecoderArbitrary), codec(codec)
  {}

  int Decode(const uint8_t* encoded, size_t encoded_len,
             int16_t* decoded, SpeechType* speech_type)
  {
    *speech_type = kSpeech;
    return codec.decode(encoded, encoded_len, decoded);
  }

  int DecodeRedundant(const uint8_t* encoded, size_t encoded_len,
                      int16_t* decoded, SpeechType* speech_type)
  {
    return Decode(encoded, encoded_len, decoded, speech_type);
  }

  bool HasDecodePlc() const {
    return TRUE;
  }

  int DecodePlc(int num_frames, int16_t* decoded) {
    return codec.decode(NULL, SPEEX_ENCODED_FRAME_SIZE * num_frames, decoded);
  }

  void Init() {}

  int PacketDuration(const uint8_t* encoded, size_t encoded_len) {
    return (encoded_len / SPEEX_ENCODED_FRAME_SIZE) * SPEEX_FRAME_SIZE;
  }

  int PacketDurationRedundant(const uint8_t* encoded, size_t encoded_len) const {
    return PacketDuration(encoded, encoded_len);
  }

  bool PacketHasFec(const uint8_t* encoded, size_t encoded_len) const {
    return FALSE;
  }

  static bool CodecSupported(NetEqDecoder codec_type) {
    return codec_type == kDecoderArbitrary;
  }

  static int CodecSampleRateHz(NetEqDecoder codec_type) {
    return 8000;
  }

  static AudioDecoder* CreateAudioDecoder(NetEqDecoder codec_type) {
    return NULL;
  }
}



#endif