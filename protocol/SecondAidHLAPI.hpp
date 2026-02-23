// This is a high-level API for SecondAID
// I made it so that it can be easily hooked to GUI/CLI without any bloat and to
// keep the stepping state machine thingy abstracted away
#pragma once

#include "AIDPacket.hpp"
#include "SecondAid.hpp"
#include <string>
#include <vector>
class SecondAidHLAPI {
  SecondAid aid;

public:
  // Breakpoints
  void BreakpointAdd(std::string scriptPath, int line);
  void BreakpointRemove(int idx);
  void BreakpointClearAll();
  // BreakpointInfo BreakpointGet(int idx);
  bool BreakpointExists(int idx);
  std::vector<BreakpointInfo> BreakpointGetList();
  // Debugging execution
  void ExecutionResume();
  void ExecutionStep(int n);
  // void ExecutionNextBreakpoint();
  void ExecutionRunToLine(int line);
  // void ExecutionDropObject();
  //  LuaDebugger attach/detach
  void DebuggerAttach();
  void DebuggerDetach();
  // Script
  void ScriptReload(std::string scriptPath);
  void ScriptGetCachedSource(std::string scriptPath);
  // Watches
  void WatchAdd(std::string exp);
  void WatchRemove(int idx);
  void WatchClearAll();
  bool WatchExists(int idx);
  std::vector<std::string> WatchGetList();
  // Console
  void ConsoleSendCommand(std::string line);

  // Start/Stop/System
  void Start(std::string ip);
  void Stop();
  bool IsRunning();
  bool IsConnected();
  std::string GetCurrentDebuggedFile();

  // Callbacks
  AidCallbacks &Callbacks() { return aid.callbacks; }
};

// Breakpoints
void SecondAidHLAPI::BreakpointAdd(std::string scriptPath, int line) {
  // TODO check if the scriptPath exists
  // TODO check if the breakpoint has been already added
  aid.g_Breakpoints.push_back({scriptPath, line});
  aid.SendLuaBreakpoint(scriptPath, line, true);
}
void SecondAidHLAPI::BreakpointRemove(int idx) {
  if (BreakpointExists(idx)) {
    aid.SendLuaBreakpoint(aid.g_Breakpoints[idx].File, aid.g_Breakpoints[idx].Line, false);
    aid.g_Breakpoints.erase(aid.g_Breakpoints.begin() + idx);
  }
}
void SecondAidHLAPI::BreakpointClearAll() {
  for (auto v : aid.g_Breakpoints)
    aid.SendLuaBreakpoint(v.File, v.Line, false);
  aid.g_Breakpoints.clear();
  // aid.SendLuaAttach();
}
bool SecondAidHLAPI::BreakpointExists(int idx) {
  return (idx >= 0 && idx < aid.g_Breakpoints.size());
}
/*BreakpointInfo SecondAidHLAPI::BreakpointGet(int idx) {
  // TODO implement
}*/
std::vector<BreakpointInfo> SecondAidHLAPI::BreakpointGetList() { return aid.g_Breakpoints; }
// Debugging execution
void SecondAidHLAPI::ExecutionResume() { aid.SendLuaDrop(); }
void SecondAidHLAPI::ExecutionStep(int n) {
  for (int i = 0; i < n; i++) {
    aid.SendLuaStep();
    // TODO check if we reaaaly need it
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
/*void SecondAidHLAPI::ExecutionNextBreakpoint() {
  if (aid.g_IsConnected) {
    aid.g_AutoMode = AUTO_CONTINUE;
    aid.SendLuaStep();
  }
}*/
// void SecondAidHLAPI::ExecutionDropObject() { aid.SendLuaDrop(); }
void SecondAidHLAPI::ExecutionRunToLine(int line) {
  if (aid.g_CurrentFile.empty()) {
    // TODO make it sanely throw error there
    // std::cout << "Context unknown. Break/Step first." << std::endl;
  } else {
    aid.g_TargetLine = line;
    aid.g_TargetFile = aid.g_CurrentFile;
    aid.g_AutoMode = AUTO_RUNTO;
    // std::cout << ANSI_GREEN << "[AUTO] Running to " << aid.g_TargetFile <<
    // ":"
    //           << aid.g_TargetLine << "..." << ANSI_RESET << std::endl;
    aid.SendLuaStep();
  }
}
// LuaDebugger attach/detach
void SecondAidHLAPI::DebuggerAttach() { aid.SendLuaAttach(); }
void SecondAidHLAPI::DebuggerDetach() { aid.SendLuaDetach(); }
// Script
void SecondAidHLAPI::ScriptReload(std::string scriptPath) { aid.SendLuaReload(scriptPath); }
void SecondAidHLAPI::ScriptGetCachedSource(std::string scriptPath) {
  aid.SendLuaGetSource(scriptPath);
}
// Watches
void SecondAidHLAPI::WatchAdd(std::string exp) {
  aid.g_Watches.push_back(exp);
  // TODO make it keep sending the requests
  if (aid.g_IsConnected)
    aid.SendLuaWatchRequest(exp);
}
void SecondAidHLAPI::WatchRemove(int idx) {
  if (WatchExists(idx)) {
    aid.g_Watches.erase(aid.g_Watches.begin() + idx);
  }
}
bool SecondAidHLAPI::WatchExists(int idx) { return (idx >= 0 && idx < aid.g_Watches.size()); }
void SecondAidHLAPI::WatchClearAll() { aid.g_Watches.clear(); }
std::vector<std::string> SecondAidHLAPI::WatchGetList() { return aid.g_Watches; }

// Console
void SecondAidHLAPI::ConsoleSendCommand(std::string line) { aid.SendConsoleCommand(line); }

// Start/Stop/System
void SecondAidHLAPI::Start(std::string ip) { aid.Start(ip); }
void SecondAidHLAPI::Stop() { aid.Stop(); }
bool SecondAidHLAPI::IsRunning() { return aid.g_Running; }
bool SecondAidHLAPI::IsConnected() { return aid.g_IsConnected; }
std::string SecondAidHLAPI::GetCurrentDebuggedFile() { return aid.g_CurrentFile; }
