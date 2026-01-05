#pragma once
#include "Events.hpp"
#include <functional>
#include <string>

struct AidCallbacks {
  std::function<void(EventGameLog event)> OnGameLogReceived = [](auto) {};
  std::function<void(AIDPacket packet)> OnUnimplementedPacketReceived =
      [](auto) {};
  std::function<void(std::string file, std::string source)> OnSourceReceived =
      [](auto, auto) {};
  std::function<void(EventLuaError)> OnLuaError = [](EventLuaError) {};
  std::function<void(std::string exp, std::string result)> OnWatchRecieved =
      [](auto, auto) {};
  std::function<void(EventGotScriptContext)> OnContextInfoRecieved =
      [](EventGotScriptContext) {};
  std::function<void(ConnectionState)> OnConnectionStateChanged = [](auto) {};
  std::function<void()> OnScriptFinished = []() {};
  std::function<void(PauseReason)> OnScriptPaused = [](auto) {};
  std::function<void()> OnScriptResumed = []() {};
  std::function<void(std::string file, int line, std::string function)>
      OnDbgLocationUpdate = [](auto, auto, auto) {};
};
