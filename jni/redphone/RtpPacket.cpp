#include "RtpPacket.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include <srtp.h>

RtpPacket::RtpPacket(char* packetBuf, int packetLen) {
  packet     = (char*)malloc(packetLen);
  payloadLen = packetLen - sizeof(RtpHeader);
  memcpy(packet, packetBuf, packetLen);
}

RtpPacket::RtpPacket(char* payload, int payloadBufLen, int sequenceNumber) {
  packet     = (char*)malloc(sizeof(RtpHeader) + payloadBufLen + SRTP_MAX_TRAILER_LEN);
  payloadLen = payloadBufLen;

  memset(packet, 0, sizeof(RtpHeader) + payloadLen + SRTP_MAX_TRAILER_LEN);

  RtpHeader header;
  header.flags          = htons(32768);
  header.sequenceNumber = htons(sequenceNumber);
  header.ssrc           = 0;
  header.timestamp      = 0; // TODO

  memcpy(packet, (void*)&header, sizeof(RtpHeader));
  memcpy(packet + sizeof(RtpHeader), payload, payloadLen);
}

RtpPacket::~RtpPacket() {
  free(packet);
}

uint16_t RtpPacket::getSequenceNumber() {
  RtpHeader *header = (RtpHeader*)packet;
  return ntohs(header->sequenceNumber);
}

char* RtpPacket::getPayload() {
  return packet + sizeof(RtpHeader);
}

uint32_t RtpPacket::getPayloadLen() {
  return payloadLen;
}

char* RtpPacket::getSerializedPacket() {
  return packet;
}

int RtpPacket::getSerializedPacketLen() {
  return sizeof(RtpHeader) + payloadLen;
}