#include "AudioCodec.h"

#include <speex/speex.h>
//#include <speex/speex_preprocess.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <android/log.h>

#define TAG "AudioCodec"



AudioCodec::AudioCodec() {
  enc        = speex_encoder_init( speex_lib_get_mode( SPEEX_MODEID_NB ) );
  dec        = speex_decoder_init( speex_lib_get_mode( SPEEX_MODEID_NB ) );

  WebRtcAecm_Create(&aecm);
  WebRtcAecm_Init(aecm, SPEEX_SAMPLE_RATE);

  if (enc == NULL) {
    throw -1;
  }

  if (dec == NULL) {
    throw -1;
  }

  spx_int32_t tmp;
  tmp=1;
  speex_decoder_ctl(dec, SPEEX_SET_ENH, &tmp);
  tmp=0;
  speex_encoder_ctl(enc, SPEEX_SET_VBR, &tmp);
  tmp=3;
  speex_encoder_ctl(enc, SPEEX_SET_QUALITY, &tmp);
  tmp=1;
  speex_encoder_ctl(enc, SPEEX_SET_COMPLEXITY, &tmp);

  speex_encoder_ctl(enc, SPEEX_GET_FRAME_SIZE, &enc_frame_size );
  speex_decoder_ctl(dec, SPEEX_GET_FRAME_SIZE, &dec_frame_size );

  __android_log_print(ANDROID_LOG_WARN, TAG, "Encoding frame size: %d", enc_frame_size);
  __android_log_print(ANDROID_LOG_WARN, TAG, "Decoding frame size: %d", dec_frame_size);

  speex_bits_init(&enc_bits);
  speex_bits_init(&dec_bits);
}

AudioCodec::~AudioCodec() {
  speex_bits_destroy( &enc_bits );
  speex_bits_destroy( &dec_bits );

  speex_encoder_destroy( enc );
  speex_decoder_destroy( dec );
}

int AudioCodec::encode(short *rawData, char* encodedData, int maxEncodedDataLen) {
  short cleanData[160];

//  speex_echo_capture(echo_state, (spx_int16_t *)rawData, (spx_int16_t *)canceledEcho);
  WebRtcAecm_Process(aecm, rawData, NULL, cleanData, 160, 75);
//  WebRtcAec_Process(aec, rawData, NULL, cleanData, NULL, 160, 110, 0);

  speex_bits_reset(&enc_bits);
  speex_encode_int(enc, (spx_int16_t *)cleanData, &enc_bits);

  return speex_bits_write(&enc_bits, encodedData, maxEncodedDataLen);
}

int AudioCodec::decode(char* encodedData, int encodedDataLen, short *rawData) {
  int rawDataOffset = 0;

  speex_bits_read_from(&dec_bits, encodedData, encodedDataLen);

  // TODO buffer overflow!
  while (speex_decode_int(dec, &dec_bits, rawData + rawDataOffset) == 0) {
//    speex_echo_playback(echo_state, (spx_int16_t *)(rawData + rawDataOffset));
    WebRtcAecm_BufferFarend(aecm, rawData + rawDataOffset, dec_frame_size);
//    WebRtcAec_BufferFarend(aec, rawData + rawDataOffset, dec_frame_size);
    rawDataOffset += dec_frame_size;
  }

  return rawDataOffset;
}
