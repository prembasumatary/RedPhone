#ifndef __WEBRTC_JITTER_BUFFER_H__
#define __WEBRTC_JITTER_BUFFER_H__

#include "AudioCodec.h"
#include "WebRtcCodec.h"
#include "RtpPacket.h"

#include <modules/audio_coding/neteq/interface/neteq.h>
#include <modules/interface/module_common_types.h>

class WebRtcJitterBuffer {

private:
  NetEq *neteq;
  WebRtcCodec webRtcCodec;

public:
  WebRtcJitterBuffer(AudioCodec &codec) : webRtcCodec(codec) {
    NetEq::Config config;
    config.sample_rate_hz = 8000;

    neteq = NetEq::Create(config);

    if (neteq->RegisterExternalDecoder(&webRtcCodec, kDecoderPCMu, 0) != kOK) {
      throw -1; // TODO
    }
  }

  void addAudio(RtpPacket *packet) {
    WebRtcRTPHeader header;
    header.header.payloadType    = packet->getPayloadType();
    header.header.sequenceNumber = packet->getSequenceNumber();
    header.header.timestamp      = packet->getTimestamp();
    header.header.ssrc           = packet->getSsrc();

    uint8_t *payload = (uint8_t*)malloc(packet->getPayloadLen());
    memcpy(payload, packet->getPayload(), packet->getPayloadLen());

    neteq->InsertPacket(header, payload, packet->getPayloadLen(), 0); // TODO check return
  }

  int getAudio(short *rawData, int maxRawData) {
    int samplesPerChannel;
    int numChannels;

    neteq->GetAudio(maxRawData, rawData, &samplesPerChannel, &numChannels); // TODO check return

    return samplesPerChannel;
  }


}



#endif