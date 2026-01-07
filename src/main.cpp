#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>

#include "DefaultLayout.hpp"
#include "Events.hpp"
#include "SecondAidHLAPI.hpp"
#include "widgets/BreakpointsWindow.hpp"
#include "widgets/ConnectionWindow.hpp"
#include "widgets/ConsoleWindow.hpp"
#include "widgets/DebuggerPanel.hpp"
#include "widgets/LogWindow.hpp"
#include "widgets/LuaErrorWindow.hpp"
#include "widgets/ScriptEditorWindow.hpp"
#include "widgets/ScriptExplorer.hpp"
#include "widgets/StaticAnalysisOverview.hpp"
#include "widgets/StaticAnalysisWindow.hpp"
#include "widgets/WatchesWindow.hpp"

struct AppState {
  SecondAidHLAPI aid;
  LogWindow gameLogWindow;
  LogWindow appStatusWindow;
  ConsoleWindow consoleWindow;
  ScriptExplorer scriptExplorer;
  ScriptEditorWindow scriptEditor;
  BreakpointsWindow breakpointsWindow;
  LuaErrorWindow luaErrorWindow;
  DebuggerPanel debugPanel;
  WatchesWindow watchesWindow;
  ConnectionWindow connectionWindow;
  StaticAnalysisWindow analysisWindow;
  StaticAnalysisOverview analysisOverviewWindow;

  std::vector<EventGameLog> pendingLogs;
  std::mutex queueMutex;

  std::vector<EventLuaError> pendingLuaErrors;

  std::vector<std::pair<std::string, uint32_t>> pendingNetworkLogs;

  std::string CurrentContextFile = "Unknown";
  int CurrentContextLine = 0;
  std::string CurrentPauseReason = "Manual"; // "Step", "Breakpoint", "Manual"

  void AddSystemLog(const std::string &msg) {
    appStatusWindow.AddLog(msg, "System", 0);
  }

  void EnqueueLuaError(EventLuaError err) {
    std::lock_guard<std::mutex> lock(queueMutex);
    pendingLuaErrors.push_back(err);
  }

  void EnqueueLog(EventGameLog log) {
    std::lock_guard<std::mutex> lock(queueMutex);
    pendingLogs.push_back(log);
  }

  void EnqueueNetworkLog(const std::string &msg, uint32_t color) {
    std::lock_guard<std::mutex> lock(queueMutex);
    pendingNetworkLogs.push_back({msg, color});
  }

  void ProcessEvents() {
    std::lock_guard<std::mutex> lock(queueMutex);

    for (const auto &log : pendingLogs) {
      gameLogWindow.AddLog(log.msg, log.channel, log.colour);
      if (log.channel == "Console") {
        consoleWindow.AddLog(log.msg, log.colour);
      }
    }
    pendingLogs.clear();

    for (const auto &err : pendingLuaErrors) {
      luaErrorWindow.AddError(err);
      appStatusWindow.AddLog("Lua Error in " + err.script + ":" +
                                 std::to_string(err.line),
                             "Error", 0xFF0000FF);
    }
    pendingLuaErrors.clear();

    for (const auto &netLog : pendingNetworkLogs) {
      appStatusWindow.AddLog(netLog.first, "Network", netLog.second);
    }
    pendingNetworkLogs.clear();
  }
};

void SetupImGuiStyle() {
  ImGui::StyleColorsDark();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void SetupAidCallbacks(AppState &app) {
  app.aid.Callbacks().OnWatchRecieved = [&app](std::string exp,
                                               std::string val) {
    app.watchesWindow.UpdateWatchValue(exp, val, app.CurrentPauseReason,
                                       app.CurrentContextFile,
                                       app.CurrentContextLine);
  };

  app.aid.Callbacks().OnGameLogReceived = [&app](EventGameLog event) {
    app.EnqueueLog(event);
  };

  app.aid.Callbacks().OnNetworkLogReceived = [&app](std::string msg,
                                                    uint32_t color) {
    app.EnqueueNetworkLog(msg, color);
  };

  app.aid.Callbacks().OnLuaError = [&app](EventLuaError err) {
    app.EnqueueLuaError(err);
    app.scriptEditor.MarkErrorLine(err.script, err.line - 1);
  };

  app.aid.Callbacks().OnConnectionStateChanged = [&app](ConnectionState state) {
    app.connectionWindow.SetState(state);
    std::string msg;
    switch (state) {
    case ConnectionState::CONNECTED:
      msg = "Connected to server (Handshake OK). Waiting for game payload...";
      break;
    case ConnectionState::GAME_READY:
      msg = "Game Ready! Debugger attached and active.";
      break;
    case ConnectionState::GAME_DISCONNECTED:
      msg = "Game Disconnected normally. Waiting for new connection...";
      break;
    case ConnectionState::CONNECTION_LOST:
      msg = "Connection Lost unexpectedly! \n"
            "   - Check if the game crashed.\n"
            "   - Check if Firewall/Antivirus is blocking port 56000.\n"
            "   - Ensure both devices are on the same network.";
      break;
    case ConnectionState::FAILED_BIND_PORT:
      msg = "CRITICAL: Failed to bind port 56000!\n"
            "   - Is another instance of SecondAID running?\n"
            "   - Is the port used by another application?\n"
            "   - Try restarting SecondAID.";
      break;
    }
    app.AddSystemLog(msg);
  };

  app.aid.Callbacks().OnSourceReceived = [&app](std::string file,
                                                std::string source) {
    app.AddSystemLog("Received source for: " + file);
    app.scriptEditor.OpenDiffView(source);
  };

  app.aid.Callbacks().OnContextInfoRecieved =
      [&app](EventGotScriptContext ctx) { app.debugPanel.UpdateContext(ctx); };

  app.aid.Callbacks().OnScriptResumed = [&app]() {
    app.debugPanel.SetState(DebuggerState::Running);
    app.debugPanel.ClearContext();
    app.scriptEditor.ClearPausedState();
    app.AddSystemLog("Script Resumed.");
  };

  app.aid.Callbacks().OnScriptPaused = [&app](PauseReason reason) {
    if (reason == PauseReason::STEP)
      app.CurrentPauseReason = "Step";
    else if (reason == PauseReason::BREAKPOINT)
      app.CurrentPauseReason = "Breakpoint";
    else
      app.CurrentPauseReason = "Pause";
  };

  app.aid.Callbacks().OnDbgLocationUpdate = [&app](std::string file, int line,
                                                   std::string function) {
    app.debugPanel.SetState(DebuggerState::Paused);
    app.CurrentContextFile = file;
    app.CurrentContextLine = line;
    app.scriptEditor.SetPausedState(file, line - 1);
    app.scriptEditor.LoadFile(file);
    app.AddSystemLog("Paused at: " + file + ":" + std::to_string(line));
  };
}

int main(int, char **) {
  if (!glfwInit())
    return 1;
  const char *glsl_version = "#version 130";
  GLFWwindow *window =
      glfwCreateWindow(1728, 972, "SecondAID by pawelktk", NULL, NULL);
  if (!window)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  SetupImGuiStyle();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  AppState app;
  SetupAidCallbacks(app);

  app.connectionWindow.Setup(
      [&app](std::string ip) {
        app.AddSystemLog("Connecting to " + ip + " (Port 56000)...");
        app.aid.Start(ip);
      },
      [&app]() {
        app.AddSystemLog("Disconnecting...");
        app.aid.Stop();
        app.connectionWindow.SetState(ConnectionState::GAME_DISCONNECTED);
        app.debugPanel.SetState(DebuggerState::Running);
        app.scriptEditor.ClearPausedState();
      });

  app.watchesWindow.Setup(
      [&app](std::string exp) {
        app.AddSystemLog("Adding watch: " + exp);
        app.aid.WatchAdd(exp);
        app.watchesWindow.SyncList(app.aid.WatchGetList());
      },
      [&app](int idx) { app.aid.WatchRemove(idx); },
      [&app]() { app.aid.WatchClearAll(); });

  app.scriptEditor.SetAddWatchCallback([&app](std::string exp) {
    app.AddSystemLog("Adding watch from editor: " + exp);
    app.aid.WatchAdd(exp);
    app.watchesWindow.SyncList(app.aid.WatchGetList());
  });

  app.consoleWindow.SetSendCommandCallback(
      [&app](std::string cmd) { app.aid.ConsoleSendCommand(cmd); });

  app.connectionWindow.DoAutoconnect();

  app.scriptExplorer.SetSelectionCallback(
      [&app](std::string file) { app.scriptEditor.LoadFile(file); });
  app.scriptEditor.SetDiffRequestCallback([&app](std::string pathLower) {
    app.AddSystemLog("Requesting cached script for: " + pathLower);
    app.aid.ScriptGetCachedSource(pathLower);
  });

  app.scriptEditor.SetReloadRequestCallback([&app](std::string scriptPath) {
    app.AddSystemLog("Reloading: " + scriptPath);
    app.aid.ScriptReload(scriptPath);
  });

  app.scriptEditor.SetOnScriptListChangedCallback(
      [&app]() { app.scriptExplorer.Refresh(); });

  app.debugPanel.SetCallbacks(
      [&app]() {
        app.AddSystemLog("Resuming execution...");
        app.aid.ExecutionResume();
      },
      [&app](int n) {
        app.AddSystemLog("Stepping " + std::to_string(n) + " times...");
        app.aid.ExecutionStep(n);
      },
      [&app]() {
        app.scriptEditor.LoadFile(app.aid.GetCurrentDebuggedFile());
      });

  app.scriptEditor.SetBreakpointChangeCallback(
      [&app](std::string file, int line, bool add) {
        if (add) {
          app.aid.BreakpointAdd(file, line + 1);
        } else {
          auto list = app.aid.BreakpointGetList();
          for (size_t i = 0; i < list.size(); i++) {
            if (list[i].File == file && list[i].Line == (line + 1)) {
              app.aid.BreakpointRemove((int)i);
              break;
            }
          }
        }
      });

  app.breakpointsWindow.Setup(
      &app.scriptEditor,
      [&app](int index) {
        app.AddSystemLog("Removing BP index: " + std::to_string(index));
        app.aid.BreakpointRemove(index);
      },
      [&app]() {
        app.AddSystemLog("Clearing all breakpoints.");
        app.aid.BreakpointClearAll();
      });

  app.luaErrorWindow.Setup(&app.scriptEditor);
  app.analysisWindow.Setup(&app.scriptEditor);
  app.analysisOverviewWindow.Setup(&app.scriptEditor);

  std::ifstream iniFile("imgui.ini");
  if (!iniFile.good()) {
    ImGui::LoadIniSettingsFromMemory(defaultLayout);
  }
  bool firstFrame = true;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // --- breakpoints ---
    std::vector<BreakpointInfo> currentBPs = app.aid.BreakpointGetList();
    app.breakpointsWindow.UpdateList(currentBPs);
    auto &globalPoints = app.scriptEditor.GetGlobalBreakpoints();
    globalPoints.clear();
    for (const auto &bp : currentBPs) {
      std::string f = bp.File;
      globalPoints[f].insert(bp.Line - 1);
    }

    app.ProcessEvents();

    if (app.aid.IsConnected() && !app.aid.IsRunning()) {
      if (ImGui::IsKeyPressed(ImGuiKey_F5, false)) {
        app.aid.ExecutionResume();
      }
      if (ImGui::IsKeyPressed(ImGuiKey_F10, false)) {
        app.aid.ExecutionStep(1);
      }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    // --- draw ---
    app.appStatusWindow.Draw("SecondAID Status");
    app.gameLogWindow.Draw("Game Logs");
    app.luaErrorWindow.Draw("Lua Errors");
    app.consoleWindow.Draw("Console");
    app.scriptExplorer.Draw("Script Browser");
    app.scriptEditor.Draw("Script Editor");
    app.breakpointsWindow.Draw("Breakpoints");
    app.watchesWindow.Draw("Watches");
    app.debugPanel.Draw("Controls");
    app.connectionWindow.Draw("Connection");
    app.analysisWindow.Draw("Active File Analysis");
    app.analysisOverviewWindow.Draw("Static Analysis Overview");

    if (firstFrame) {
      ImGui::SetWindowFocus("Script Editor");
      ImGui::SetWindowFocus("Connection");
      firstFrame = false;
    }
    // Render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    // std::cout<<ImGui::GetIO().Framerate<<std::endl;
  }

  // Cleanup
  // app.aid.Stop();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
