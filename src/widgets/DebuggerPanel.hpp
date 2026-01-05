#pragma once
#include "Events.hpp"
#include "imgui.h"
#include <functional>
#include <string>

enum class DebuggerState { Running, Paused };

class DebuggerPanel {
  DebuggerState currentState = DebuggerState::Running;

  EventGotScriptContext CurrentContext;
  bool HasContext = false;
  int StepAmount = 1;

  std::function<void()> OnResume;
  std::function<void(int)> OnStep;
  std::function<void()> OnJumpToExecution;

public:
  DebuggerPanel() : CurrentContext() {}

  void SetState(DebuggerState state) { currentState = state; }

  void UpdateContext(const EventGotScriptContext &ctx) {
    CurrentContext = ctx;
    HasContext = true;
  }

  void ClearContext() {
    HasContext = false;
    CurrentContext = EventGotScriptContext();
  }

  void SetCallbacks(std::function<void()> onResume,
                    std::function<void(int)> onStep,
                    std::function<void()> onJump) {
    OnResume = onResume;
    OnStep = onStep;
    OnJumpToExecution = onJump;
  }

  void DrawCopyableText(const char *label, const std::string &text) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::PushID(label);
    ImGui::SetNextItemWidth(-FLT_MIN);
    char buff[256];
    strncpy(buff, text.c_str(), sizeof(buff));
    ImGui::InputText("##val", buff, sizeof(buff), ImGuiInputTextFlags_ReadOnly);
    ImGui::PopID();
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    if (currentState == DebuggerState::Paused) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "STATUS: PAUSED");
    } else {
      ImGui::TextColored(ImVec4(0, 1, 0, 1), "STATUS: RUNNING");
    }
    ImGui::Separator();

    if (currentState == DebuggerState::Paused) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.6f, 0, 1));
      if (ImGui::Button("RESUME (F5)", ImVec2(100, 0))) {
        if (OnResume)
          OnResume();
      }
      ImGui::PopStyleColor();

      ImGui::SameLine();

      if (ImGui::Button("STEP (F10)", ImVec2(80, 0))) {
        if (OnStep)
          OnStep(1);
      }

      ImGui::SameLine();
      if (ImGui::Button("Show Exec")) {
        if (OnJumpToExecution)
          OnJumpToExecution();
      }

      ImGui::Spacing();

      ImGui::PushItemWidth(50);
      ImGui::InputInt("##steps", &StepAmount, 0, 0);
      ImGui::PopItemWidth();
      ImGui::SameLine();
      if (ImGui::Button("Step N")) {
        if (OnStep)
          OnStep(StepAmount > 0 ? StepAmount : 1);
      }
    } else {
      ImGui::BeginDisabled();
      ImGui::Button("RESUME (F5)", ImVec2(100, 0));
      ImGui::SameLine();
      ImGui::Button("STEP (F10)", ImVec2(80, 0));
      ImGui::EndDisabled();
    }

    ImGui::Separator();

    if (HasContext) {
      ImGui::TextDisabled("Context Information:");
      if (ImGui::BeginTable("ContextTable", 2,
                            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_Resizable)) {

        ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                                80.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

        DrawCopyableText("Object Name", CurrentContext.thisObjectName);
        DrawCopyableText("Object ID",
                         std::to_string(CurrentContext.thisObjectID));
        DrawCopyableText("Function", CurrentContext.function);
        DrawCopyableText("Root Script", CurrentContext.rootScript);

        ImGui::EndTable();
      }
    } else {
      ImGui::TextDisabled("No context available.");
    }
    ImGui::End();
  }
};
