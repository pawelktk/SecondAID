#pragma once

#include "BinaryStream.hpp"
#include "MultiplatformNet.hpp"
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <ios>
#include <string>
#include <vector>

const uint32_t AID_MAGIC = 0x08AFFE15;

enum class LuaCmdId {
  // To game
  Watch = 10,            // {string varName}
  GetSource = 12,        // {string varPath}
  Step = 20,             // {}
  AddBreakpoint = 31,    // {string filepath, int32_t lineNum, int32_t extra}
  Detach = 32,           // {}
  RemoveBreakpoint = 33, // {string filepath, int32_t lineNum, int32_t extra}
  ReloadScript = 34,     // {string filepath}
  DropObject = 35        // {}
  // From game
  // TODO
};

enum class AIDPacketID : uint16_t {
  Log = 0,
  Ping = 1,
  Handshake = 2,
  Disconnect = 3,
  DataResponse = 4,
  Dropped = 5,
  SyncPing = 6,
  SyncPong = 7
};
#pragma pack(push, 1)
struct AIDPacketHeader {
  uint32_t Magic = AID_MAGIC;
  uint16_t SenderPort;
  uint32_t SenderIP;
  AIDPacketID PacketID;
  uint32_t PayloadSize = 0;
  union {
    struct {
      int32_t ColorRGB;
      uint32_t Reserved;
    } Log;
    struct {
      uint32_t Timestamp;
      uint32_t Echo;
    } Sync;
    struct {
      int32_t LogicPort;
      uint32_t LogicIP;
    } Handshake;
    struct {
      int32_t State;
      float Init;
    } GameState;
    struct {
      int32_t Data1;
      uint32_t Data2;
    } Raw;
  } Data;
  char Category[32];
  char Message[32];
};
#pragma pack(pop)

class AIDPacket {
public:
  AIDPacketHeader header;
  BinaryStream payload;

  // IMPORTANT after those constructors we need to also set payload and Data
  AIDPacket(AIDPacketHeader hTemplate, AIDPacketID PacketID,
            std::string Category, std::string Message)
      : header(hTemplate) {
    header.PacketID = PacketID;
    strncpy(header.Category, Category.c_str(), 31);
    strncpy(header.Message, Message.c_str(), 31);
  }
  AIDPacket(AIDPacketHeader hTemplate, AIDPacketID PacketID)
      : header(hTemplate) {
    strncpy(header.Category, hTemplate.Category, 31);
    strncpy(header.Message, hTemplate.Message, 31);
    header.PacketID = PacketID;
  }
  AIDPacket(AIDPacketHeader hTemplate) : header(hTemplate) {
    strncpy(header.Category, hTemplate.Category, 31);
    strncpy(header.Message, hTemplate.Message, 31);
  }
  AIDPacket(AIDPacketHeader hTemplate, BinaryStream payload)
      : header(hTemplate) {
    strncpy(header.Category, hTemplate.Category, 31);
    strncpy(header.Message, hTemplate.Message, 31);
    header.PayloadSize = payload.size();
    this->payload = payload;
  }
  void SetPayload(BinaryStream raw) { payload = raw; }

  int Send(SOCKET fd, sockaddr_in targetAddr) const {
    std::vector<uint8_t> buffer(sizeof(AIDPacketHeader) + header.PayloadSize);
    std::memcpy(buffer.data(), &header, sizeof(AIDPacketHeader));
    if (payload.size() && header.PayloadSize > 0)
      std::memcpy(buffer.data() + sizeof(AIDPacketHeader), payload.data(),
                  header.PayloadSize);

    return sendto(fd, (const char *)buffer.data(), (int)buffer.size(), 0,
                  (sockaddr *)&targetAddr, sizeof(targetAddr));
  }

  std::string toString() const {
    std::stringstream ss;

    ss << "[AIDPacket] ";
    switch (header.PacketID) {
    case AIDPacketID::Log:
      ss << "Type: Log";
      break;
    case AIDPacketID::Ping:
      ss << "Type: Ping";
      break;
    case AIDPacketID::Handshake:
      ss << "Type: Handshake";
      break;
    case AIDPacketID::Disconnect:
      ss << "Type: Disconnect";
      break;
    case AIDPacketID::DataResponse:
      ss << "Type: DataResponse";
      break;
    case AIDPacketID::Dropped:
      ss << "Type: Dropped";
      break;
    case AIDPacketID::SyncPing:
      ss << "Type: SyncPing";
      break;
    case AIDPacketID::SyncPong:
      ss << "Type: SyncPong";
      break;
    default:
      ss << "Type: Unknown (" << (int)header.PacketID << ")";
      break;
    }

    std::string cat(header.Category, strnlen(header.Category, 32));
    std::string msg(header.Message, strnlen(header.Message, 32));

    if (!cat.empty())
      ss << " | Cat: " << cat;
    if (!msg.empty())
      ss << " | Msg: " << msg;

    ss << " | Data: { ";
    switch (header.PacketID) {
    case AIDPacketID::Log:
      ss << "RGB: #" << std::hex << std::setw(6) << std::setfill('0')
         << header.Data.Log.ColorRGB << std::dec;
      break;

    case AIDPacketID::SyncPing:
    case AIDPacketID::SyncPong:
      ss << "TS: " << header.Data.Sync.Timestamp
         << ", Echo: " << header.Data.Sync.Echo;
      break;

    case AIDPacketID::Handshake: {
      uint8_t *logicIp = (uint8_t *)&header.Data.Handshake.LogicIP;
      ss << "LogicPort: " << header.Data.Handshake.LogicPort
         << ", LogicIP: " << (int)logicIp[0] << "." << (int)logicIp[1] << "."
         << (int)logicIp[2] << "." << (int)logicIp[3];
      break;
    }

    case AIDPacketID::DataResponse:
      ss << "State: " << header.Data.GameState.State
         << ", Init: " << header.Data.GameState.Init;
      break;

    default:
      ss << "Raw1: " << header.Data.Raw.Data1
         << ", Raw2: " << header.Data.Raw.Data2;
      break;
    }
    ss << " }";

    if (header.PayloadSize > 0) {
      ss << " | Payload: " << header.PayloadSize << " bytes";
    }

    return ss.str();
  }
};
