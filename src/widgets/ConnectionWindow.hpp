#pragma once
#include "Events.hpp"
#include "imgui.h"
#include <cstring>
#include <functional>
#include <string>
#include <vector>

#ifdef _WIN32
#include <iphlpapi.h>
#include <winsock2.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/types.h>
#endif

class ConnectionWindow {
  char TargetIPBuf[64] = "127.0.0.1";
  std::vector<std::string> LocalIPs;

  bool AutoConnect = true;
  bool IsConnected = false;
  bool IsConnecting = false;
  bool IsGameReady = false;

  std::function<void(std::string)> OnConnectRequest;
  std::function<void()> OnDisconnectRequest;

  void DetectLocalIPs() {
    LocalIPs.clear();
#ifdef _WIN32
    ULONG outBufLen = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses =
        (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses,
                             &outBufLen) == ERROR_BUFFER_OVERFLOW) {
      free(pAddresses);
      pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
    }

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses,
                             &outBufLen) == NO_ERROR) {
      for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
           pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next) {
        if (pCurrAddresses->OperStatus != IfOperStatusUp)
          continue;
        if (pCurrAddresses->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
          continue;

        for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast =
                 pCurrAddresses->FirstUnicastAddress;
             pUnicast != NULL; pUnicast = pUnicast->Next) {
          struct sockaddr_in *sa_in =
              (struct sockaddr_in *)pUnicast->Address.lpSockaddr;
          char buf[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &(sa_in->sin_addr), buf, INET_ADDRSTRLEN);
          std::string ip = buf;
          if (ip != "127.0.0.1" && ip.find("169.254") == std::string::npos) {
            LocalIPs.push_back(ip);
          }
        }
      }
    }
    free(pAddresses);
#else
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
      return;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr == NULL)
        continue;
      if (ifa->ifa_addr->sa_family == AF_INET) {
        struct sockaddr_in *pAddr = (struct sockaddr_in *)ifa->ifa_addr;
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &pAddr->sin_addr, buf, INET_ADDRSTRLEN);
        std::string ip = buf;
        if (ip != "127.0.0.1")
          LocalIPs.push_back(ip);
      }
    }
    freeifaddrs(ifaddr);
#endif
    if (LocalIPs.empty())
      LocalIPs.push_back("127.0.0.1");
  }

public:
  ConnectionWindow() {
    DetectLocalIPs();
    if (!LocalIPs.empty()) {
      strncpy(TargetIPBuf, LocalIPs[0].c_str(), sizeof(TargetIPBuf));
    }
  }

  void Setup(std::function<void(std::string)> onConnect,
             std::function<void()> onDisconnect) {
    OnConnectRequest = onConnect;
    OnDisconnectRequest = onDisconnect;
  }

  void SetState(ConnectionState state) {
    switch (state) {
    case ConnectionState::CONNECTED:
      IsConnected = true;
      IsGameReady = false;
      IsConnecting = false;
      break;
    case ConnectionState::GAME_READY:
      IsConnected = true;
      IsGameReady = true;
      IsConnecting = false;
      break;
    case ConnectionState::GAME_DISCONNECTED:
    case ConnectionState::CONNECTION_LOST:
    case ConnectionState::FAILED_BIND_PORT:
      IsConnected = false;
      IsConnecting = false;
      IsGameReady = false;
      break;
    }
  }

  bool ShouldAutoConnect() const { return AutoConnect; }
  std::string GetTargetIP() const { return std::string(TargetIPBuf); }

  void DoAutoconnect() {
    std::string ip = GetTargetIP();
    IsConnecting = true;
    OnConnectRequest(TargetIPBuf);
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(350, 220), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    // --- STATUS ---
    ImGui::Text("Status: ");
    ImGui::SameLine();
    if (IsConnecting) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "CONNECTING...");
    } else if (IsGameReady) {
      ImGui::TextColored(ImVec4(0, 1, 0, 1), "CONNECTED & READY");
    } else if (IsConnected) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "CONNECTED (Waiting for Game...)");
    } else {
      ImGui::TextColored(ImVec4(1, 0, 0, 1), "DISCONNECTED");
    }

    ImGui::Separator();

    // --- LOCAL INFO ---
    ImGui::TextDisabled("Your Local IP(s):");
    for (const auto &ip : LocalIPs) {
      ImGui::BulletText("%s", ip.c_str());
      if (ImGui::IsItemClicked()) {
        strncpy(TargetIPBuf, ip.c_str(), sizeof(TargetIPBuf));
      }
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Click to set as Target IP");
    }

    ImGui::Separator();

    // --- CONTROLS ---
    ImGui::Text("Target IP:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##targetip", TargetIPBuf, sizeof(TargetIPBuf));

    ImGui::Checkbox("Auto-Connect at Startup", &AutoConnect);

    ImGui::Spacing();

    float width = ImGui::GetContentRegionAvail().x;
    if (IsConnecting) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
      if (ImGui::Button("STOP", ImVec2(width, 30))) {
        if (OnDisconnectRequest) {
          IsConnecting = false;
          OnDisconnectRequest();
        }
      }
      ImGui::PopStyleColor();
    } else if (!IsConnected) {
      if (ImGui::Button("CONNECT", ImVec2(width, 30))) {
        if (OnConnectRequest) {
          IsConnecting = true;
          OnConnectRequest(TargetIPBuf);
        }
      }
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
      if (ImGui::Button("DISCONNECT", ImVec2(width, 30))) {
        if (OnDisconnectRequest)
          OnDisconnectRequest();
      }
      ImGui::PopStyleColor();
    }

    ImGui::End();
  }
};
