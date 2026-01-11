#pragma once
#include "AIDPacket.hpp"
#include "AnsiColours.hpp"
#include "BinaryStream.hpp"
#include "Callbacks.hpp"
#include "Events.hpp"
#include "MultiplatformNet.hpp"
#include <atomic>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
// Needed for error codes
#include <winsock2.h>
#endif

uint32_t GetTimeMs() {
  using namespace std::chrono;
  return (uint32_t)duration_cast<milliseconds>(
             steady_clock::now().time_since_epoch())
      .count();
}

struct BreakpointInfo {
  std::string File;
  int Line;
};
enum AutoMode { IDLE, AUTO_RUNTO };

class SecondAid {
public:
  int AID_PORT = 11381;
  int GAME_PORT = 11637;
  std::string useIP;

  SOCKET g_Socket = INVALID_SOCKET;
  sockaddr_in g_TargetAddr;
  std::atomic<bool> g_Running = false;
  std::atomic<bool> g_IsConnected = false;
  std::atomic<bool> g_RecievedReadyPacket = false;
  std::atomic<uint32_t> g_LastRecvTime = 0;

  std::vector<std::string> g_Watches;
  std::vector<BreakpointInfo> g_Breakpoints;

  std::atomic<AutoMode> g_AutoMode = IDLE;
  int g_TargetLine = -1;
  std::string g_TargetFile = "";
  std::string g_CurrentFile = "";

  std::thread m_ReceiverThread;
  std::thread m_ManagerThread;

  AidCallbacks callbacks;

  bool m_IsReassembling = false;
  AIDPacketHeader m_PendingHeader;
  std::vector<uint8_t> m_ReassemblyBuffer;
  void ProcessCompletePacket(AIDPacketHeader header,
                             std::vector<uint8_t> &payloadData);

  void SendPacket(AIDPacket const &packet);
  void SendDisconnectPacket();
  AIDPacketHeader GetHeaderTemplate();

  // LuaDbg
  AIDPacketHeader GetLuaDbgHeaderTemplate();
  template <typename... Args>
  void SendLuaDbgPacket(LuaCmdId id, Args &&...args) {
    BinaryStream fullPayload(id);
    fullPayload << BinaryStream(std::forward<Args>(args)...);
    SendPacket(AIDPacket(GetLuaDbgHeaderTemplate(), fullPayload));
  }
  void SendLuaBreakpoint(const std::string &filepath, int line, bool add);
  void SendLuaStep();
  void SendLuaWatchRequest(const std::string &varName);
  void SendLuaAttach();
  void SendLuaDetach();
  void SendLuaDrop();
  void SendLuaReload(const std::string &filepath);
  void SendLuaGetSource(const std::string &filepath);
  void RefreshWatches();

  // Console
  void SendConsoleCommand(const std::string &cmd);

  // Property tree
  void SendTreeRequest();

  // Auto-stepper for LuaDebugger
  void ProcessAutoStep(const std::string &file, int line);

  // Thread funcs
  void ConnectionManagerThreadFunc();
  void ReceiverThreadFunc();

  SecondAid();
  SecondAid(std::string ip);
  ~SecondAid();
  void Start(std::string ip);
  void Stop();
};

SecondAid::SecondAid() {
#ifdef _WIN32
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}
SecondAid::SecondAid(std::string ip) : SecondAid() { Start(ip); }
SecondAid::~SecondAid() {
  Stop();
#ifdef _WIN32
  WSACleanup();
#endif
}

void SecondAid::Stop() {
  g_Running = false;
  SendLuaDetach();
  SendDisconnectPacket();
#ifdef _WIN32
  shutdown(g_Socket, SD_BOTH);
  closesocket(g_Socket);
#else
  shutdown(g_Socket, SHUT_RDWR);
  close(g_Socket);
#endif
  if (m_ManagerThread.joinable())
    m_ManagerThread.join();
  // std::cout << "ManagerThread stopped" << std::endl;
  if (m_ReceiverThread.joinable())
    m_ReceiverThread.join();
  // std::cout << "ReceiverThread stopped" << std::endl;

  if (g_Socket != INVALID_SOCKET) {
    CLOSE_SOCKET(g_Socket);
    g_Socket = INVALID_SOCKET;
  }
}

void SecondAid::Start(std::string ip) {
  if (g_Running)
    return;
  useIP = ip;
  g_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  int rcvBufSize = 1024 * 1024; // 1MB
  setsockopt(g_Socket, SOL_SOCKET, SO_RCVBUF, (char *)&rcvBufSize,
             sizeof(rcvBufSize));

  sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(AID_PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(g_Socket, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
    callbacks.OnConnectionStateChanged(ConnectionState::FAILED_BIND_PORT);
    return;
  }

  g_TargetAddr.sin_family = AF_INET;
  g_TargetAddr.sin_port = htons(GAME_PORT);
  g_TargetAddr.sin_addr.s_addr = inet_addr(useIP.c_str());

  g_Running = true;
  m_ReceiverThread = std::thread(&SecondAid::ReceiverThreadFunc, this);
  m_ManagerThread = std::thread(&SecondAid::ConnectionManagerThreadFunc, this);
}

void SecondAid::SendPacket(AIDPacket const &packet) {
  int res = packet.Send(g_Socket, g_TargetAddr);

  if (res == SOCKET_ERROR) {
#ifdef _WIN32
    int err = WSAGetLastError();
    if (err == WSAEACCES) {
      static uint32_t lastLogTime = 0;
      uint32_t now = GetTimeMs();
      if (now - lastLogTime > 1000) {
        callbacks.OnNetworkLogReceived(
            "CRITICAL: Send blocked! Permission Denied (Check Firewall).",
            0xFF0000FF);
        lastLogTime = now;
      }
    }
#endif
  }
}

AIDPacketHeader SecondAid::GetHeaderTemplate() {
  AIDPacketHeader result = {};
  result.SenderIP = inet_addr(useIP.c_str());
  result.SenderPort = htons(AID_PORT);
  return result;
}

//
// LuaDebugger
//

AIDPacketHeader SecondAid::GetLuaDbgHeaderTemplate() {
  AIDPacketHeader result = GetHeaderTemplate();
  result.PacketID = AIDPacketID::Handshake;
  result.Data.Raw.Data1 = 0;
  result.Data.Raw.Data2 = 0;
  strncpy(result.Category, "LuaDebugger", 31);
  strncpy(result.Message, "LuaDebugger", 31);
  return result;
}

void SecondAid::SendDisconnectPacket() {
  auto pktH = GetHeaderTemplate();
  pktH.PacketID = AIDPacketID::Disconnect;
  SendPacket(AIDPacket(pktH));
}

void SecondAid::SendLuaBreakpoint(const std::string &filepath, int line,
                                  bool add) {
  LuaCmdId cmdId = add ? LuaCmdId::AddBreakpoint : LuaCmdId::RemoveBreakpoint;
  int32_t extra = -1;
  SendLuaDbgPacket(cmdId, filepath, (int32_t)line, extra);

  /*
  if (g_AutoMode == IDLE) {
    std::cout << ANSI_CYAN << "[LUA] BP " << (add ? "Added" : "Removed") << ": "
  << filepath << ":"
              << line << ANSI_RESET << std::endl;
  }
  */
}

void SecondAid::SendLuaStep() { SendLuaDbgPacket(LuaCmdId::Step); }

void SecondAid::SendLuaWatchRequest(const std::string &varName) {
  SendLuaDbgPacket(LuaCmdId::Watch, varName);
}
void SecondAid::SendLuaAttach() {
  // Reset
  SendLuaDetach();
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  // Init
  AIDPacketHeader initPkt = GetLuaDbgHeaderTemplate();
  initPkt.PacketID = AIDPacketID::Ping;
  SendPacket(initPkt);

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  // Upload Breakpoints
  if (!g_Breakpoints.empty()) {
    int uploaded = 0;
    for (const auto &bp : g_Breakpoints) {
      SendLuaBreakpoint(bp.File, bp.Line, true);
      uploaded++;
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << ANSI_GREEN << "[LUA] Attached & Resumed (" << uploaded
              << " BPs active)" << ANSI_RESET << std::endl;
  } else {
    std::cout << ANSI_GREEN << "[LUA] Attached & Resumed (No BPs)" << ANSI_RESET
              << std::endl;
  }
}

void SecondAid::SendLuaDetach() {
  SendLuaDbgPacket(LuaCmdId::Detach);

  // Flush
  AIDPacketHeader flushPkt = GetLuaDbgHeaderTemplate();
  flushPkt.PacketID = AIDPacketID::Log;
  SendPacket(AIDPacket(flushPkt));

  g_AutoMode = IDLE;
  std::cout << ANSI_GREEN << "[LUA] Detached / Resumed natively." << ANSI_RESET
            << std::endl;
}

// It's more "continue", idk why tf the button in AID is named "Drop object"
void SecondAid::SendLuaDrop() {
  SendLuaDbgPacket(LuaCmdId::DropObject);
  g_AutoMode = IDLE;
}

void SecondAid::SendLuaReload(const std::string &filepath) {
  SendLuaDbgPacket(LuaCmdId::ReloadScript, filepath);

  std::cout << "[LUA] Reload requested: " << filepath << std::endl;
}

void SecondAid::SendLuaGetSource(const std::string &filepath) {
  SendLuaDbgPacket(LuaCmdId::GetSource, filepath);
}

void SecondAid::RefreshWatches() {
  if (g_Watches.empty())
    return;
  for (const auto &var : g_Watches) {
    SendLuaWatchRequest(var);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}

//
// Console
//

void SecondAid::SendConsoleCommand(const std::string &cmd) {
  AIDPacketHeader pkt = GetHeaderTemplate();
  pkt.PacketID = AIDPacketID::Log;
  strncpy(pkt.Category, "console", 31);
  strncpy(pkt.Message, "console", 31);
  BinaryStream bs;
  bs << cmd;
  SendPacket(AIDPacket(pkt, bs));
}

//
// Prop tree
//

void SecondAid::SendTreeRequest() {
  BinaryStream bs;
  AIDPacketHeader pkt = GetHeaderTemplate();
  pkt.PacketID = AIDPacketID::Log;
  pkt.Data.Raw.Data1 = -1;
  strncpy(pkt.Category, "AIDTree", 31);
  strncpy(pkt.Message, "PTree", 31);
  SendPacket(pkt);
}

//
// Auto-stepper for LuaDebugger
//

void SecondAid::ProcessAutoStep(const std::string &file, int line) {
  bool shouldStop = false;

  if (g_AutoMode == AUTO_RUNTO) {
    if (line == g_TargetLine && file == g_TargetFile) {
      shouldStop = true;
      std::cout << ANSI_BLUE << "[RUNTO] Reached target: " << file << ":"
                << line << ANSI_RESET << std::endl;
    }
  } /*else if (g_AutoMode == AUTO_CONTINUE) {
    for (const auto &bp : g_Breakpoints) {
      if (bp.File == file && bp.Line == line) {
        shouldStop = true;
        std::cout << ANSI_YELLOW << "[CONTINUE] Hit Breakpoint: " << file << ":"
  << line
                  << ANSI_RESET << std::endl;
        break;
      }
    }
  }*/

  if (shouldStop) {
    g_AutoMode = IDLE;
    RefreshWatches();
  } else {
    SendLuaStep();
  }
}

//
// Thread funcs
//

void SecondAid::ConnectionManagerThreadFunc() {
  int loopCounter = 0;
  int handshakeAttempts = 0;

  while (g_Running) {
    uint32_t now = GetTimeMs();
    if (g_IsConnected && (now - g_LastRecvTime > 3000)) {
      g_IsConnected = false;
      g_RecievedReadyPacket = false;
      callbacks.OnConnectionStateChanged(ConnectionState::CONNECTION_LOST);
      handshakeAttempts = 0;
    }

    AIDPacketHeader handshake = GetHeaderTemplate();
    handshake.PacketID = AIDPacketID::Handshake;
    handshake.Data.Handshake.LogicPort = htons(AID_PORT);
    handshake.Data.Handshake.LogicIP = inet_addr(useIP.c_str());

    AIDPacketHeader ping = GetHeaderTemplate();
    ping.PacketID = AIDPacketID::SyncPing;
    ping.Data.Sync.Timestamp = now;

    if (!g_IsConnected) {
      SendPacket(handshake);
      handshakeAttempts++;

      if (handshakeAttempts % 5 == 0) {
        callbacks.OnNetworkLogReceived("Waiting for handshake... (Attempt " +
                                           std::to_string(handshakeAttempts) +
                                           "). Check Firewall/IP.",
                                       0xFFAAAA00);
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(250));
    } else {
      handshakeAttempts = 0;
      SendPacket(ping);
      if (loopCounter % 4 == 0)
        SendPacket(handshake);
      loopCounter++;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void SecondAid::ProcessCompletePacket(AIDPacketHeader header,
                                      std::vector<uint8_t> &payloadData) {
  BinaryStream payloadStream;
  payloadStream.write(payloadData);
  AIDPacket pktFull(header, payloadStream);

  switch (header.PacketID) {
  case AIDPacketID::Log: {
    if (strncmp(header.Category, "LOGGER", 6) == 0) {
      char *msgPtr = (char *)payloadData.data();
      std::string channel(msgPtr);
      std::string msg(msgPtr + channel.size() + 1);
      callbacks.OnGameLogReceived(
          EventGameLog(channel, msg, header.Data.Log.ColorRGB));
    } else {
      callbacks.OnUnimplementedPacketReceived(pktFull);
    }
    break;
  }
  case AIDPacketID::Handshake: {
    if (strncmp(header.Category, "LuaDebugger", 11) == 0 &&
        payloadData.size() >= 4) {
      int32_t cmdID = 0;
      std::memcpy(&cmdID, payloadData.data(), 4);
      char *data = (char *)payloadData.data() + 4;
      int dLen = (int)payloadData.size() - 4;

      if (cmdID == 6 || cmdID == 4) {
        if (g_AutoMode == IDLE) {
          callbacks.OnScriptPaused((cmdID == 6) ? PauseReason::BREAKPOINT
                                                : PauseReason::STEP);
          RefreshWatches();
        }
      } else if (cmdID == 5) {
        callbacks.OnScriptResumed();
        g_AutoMode = IDLE;
      } else if (cmdID == 36) {
        if (dLen == 2)
          callbacks.OnScriptFinished();
        else
          callbacks.OnContextInfoRecieved(
              EventGotScriptContext(std::string(data)));
      } else if (cmdID == 3) {
        std::string f = data;
        data += f.length() + 1;
        std::string func = data;
        data += func.length() + 1;
        int32_t ln = 0;
        std::memcpy(&ln, data, 4);

        g_CurrentFile = f;
        if (g_AutoMode != IDLE) {
          ProcessAutoStep(f, ln);
        } else {
          callbacks.OnDbgLocationUpdate(f, ln, func);
        }
      } else if (cmdID == 41) {
        callbacks.OnLuaError(EventLuaError(std::string(data, dLen)));
        g_AutoMode = IDLE;
      } else if (cmdID == 10) {
        std::string n = data;
        data += n.length() + 1;
        callbacks.OnWatchRecieved(n, data);
      } else if (cmdID == 12) {
        std::string n = data;
        data += n.length() + 1;

        char *end = (char *)payloadData.data() + payloadData.size();
        int len = (int)(end - data);
        len = (len < 0) ? 0 : len;

        callbacks.OnSourceReceived(n, std::string(data, len));
      } else {
        callbacks.OnUnimplementedPacketReceived(pktFull);
      }
    } else {
      callbacks.OnUnimplementedPacketReceived(pktFull);
    }
    break;
  }
  case AIDPacketID::DataResponse: {
    char *payloadPtr = (char *)payloadData.data();
    if (strncmp(header.Category, "PTree", 5) == 0) {
      std::cout << "[TREE] Received (" << header.PayloadSize << " bytes)"
                << std::endl;
    } else if (!*header.Category && header.PayloadSize == 8 &&
               (strncmp(payloadPtr, "GuildII", 8) == 0)) {
      if (!g_RecievedReadyPacket) {
        SendLuaAttach();
        callbacks.OnConnectionStateChanged(ConnectionState::GAME_READY);
        g_RecievedReadyPacket = true;
      }
    } else {
      callbacks.OnUnimplementedPacketReceived(pktFull);
    }
    break;
  }
  case AIDPacketID::Disconnect:
    callbacks.OnConnectionStateChanged(ConnectionState::GAME_DISCONNECTED);
    g_IsConnected = false;
    g_RecievedReadyPacket = false;
    break;

  case AIDPacketID::Dropped:
    std::cout << ANSI_RED << "[SYSTEM] Packet dropped." << ANSI_RESET
              << std::endl;
    break;

  case AIDPacketID::SyncPing:
  case AIDPacketID::SyncPong:
  case AIDPacketID::Ping:
    break;

  default:
    callbacks.OnUnimplementedPacketReceived(pktFull);
  }
}

void SecondAid::ReceiverThreadFunc() {
  char buffer[65535];
  sockaddr_in sender;
  socklen_t senderLen = sizeof(sender);

  while (g_Running) {
    int bytes = recvfrom(g_Socket, buffer, sizeof(buffer), 0,
                         (sockaddr *)&sender, &senderLen);

    if (bytes == SOCKET_ERROR) {
#ifdef _WIN32
      int err = WSAGetLastError();
      if (err == WSAECONNRESET) {
        callbacks.OnNetworkLogReceived(
            "Error: Remote Port Unreachable! (Game closed?)", 0xFF5555FF);
      } else if (err != WSAEWOULDBLOCK && err != WSAETIMEDOUT) {
        callbacks.OnNetworkLogReceived("Socket Error: " + std::to_string(err),
                                       0xFF5555FF);
      }
#endif
      continue;
    }

    if (bytes <= 0)
      continue;

    if (m_IsReassembling) {
      if (bytes >= (int)sizeof(AIDPacketHeader)) {
        AIDPacketHeader *potentialHeader = (AIDPacketHeader *)buffer;
        if (potentialHeader->Magic == AID_MAGIC) {
          m_IsReassembling = false;
          m_ReassemblyBuffer.clear();
          callbacks.OnNetworkLogReceived(
              "WARN: Dropped fragmented packet (New header arrived)",
              0xFFAA00FF);
        }
      }
    }

    if (m_IsReassembling) {
      m_ReassemblyBuffer.insert(m_ReassemblyBuffer.end(), buffer,
                                buffer + bytes);

      if (m_ReassemblyBuffer.size() >= m_PendingHeader.PayloadSize) {
        ProcessCompletePacket(m_PendingHeader, m_ReassemblyBuffer);
        m_IsReassembling = false;
        m_ReassemblyBuffer.clear();
      }
      continue;
    }

    if (bytes < (int)sizeof(AIDPacketHeader))
      continue;

    AIDPacketHeader *pkt = (AIDPacketHeader *)buffer;

    if (pkt->Magic != AID_MAGIC) {
      std::string senderIP = inet_ntoa(sender.sin_addr);
      callbacks.OnNetworkLogReceived("WARN: Invalid Magic from " + senderIP,
                                     0xFFAA00FF);
      continue;
    }

    if (sender.sin_addr.s_addr != g_TargetAddr.sin_addr.s_addr) {
      std::string senderIP = inet_ntoa(sender.sin_addr);
      callbacks.OnNetworkLogReceived("Packet from unexpected IP: " + senderIP,
                                     0xFF8888FF);
    }

    g_LastRecvTime = GetTimeMs();
    if (!g_IsConnected) {
      g_IsConnected = true;
      SendLuaAttach();
      callbacks.OnConnectionStateChanged(ConnectionState::CONNECTED);
      callbacks.OnNetworkLogReceived("Handshake successful! Connected.",
                                     0xFF00FF00);
    }

    int payloadBytesReceived = bytes - sizeof(AIDPacketHeader);
    char *payloadStart = buffer + sizeof(AIDPacketHeader);

    if (payloadBytesReceived >= (int)pkt->PayloadSize) {
      std::vector<uint8_t> fullData(payloadStart,
                                    payloadStart + pkt->PayloadSize);
      ProcessCompletePacket(*pkt, fullData);
    } else {
      m_IsReassembling = true;
      m_PendingHeader = *pkt;
      m_ReassemblyBuffer.clear();
      if (payloadBytesReceived > 0) {
        m_ReassemblyBuffer.insert(m_ReassemblyBuffer.end(), payloadStart,
                                  payloadStart + payloadBytesReceived);
      }
    }
  }
}
