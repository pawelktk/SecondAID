#pragma once
#include "Events.hpp"
#include "ScriptEditorWindow.hpp"
#include "imgui.h"
#include <algorithm>
#include <string>
#include <vector>

struct ErrorGroup {
  EventLuaError err;
  int count;
  std::string displayMsg;
};

class LuaErrorWindow {
  ScriptEditorWindow *EditorRef = nullptr;
  std::vector<ErrorGroup> ErrorGroups;
  bool AutoScroll = true;

public:
  void Setup(ScriptEditorWindow *editor) { EditorRef = editor; }

  void AddError(const EventLuaError &newErr) {
    for (auto &group : ErrorGroups) {
      if (group.err.script == newErr.script && group.err.line == newErr.line &&
          group.err.errorMsg == newErr.errorMsg) {
        group.count++;
        return;
      }
    }

    ErrorGroup newGroup;
    newGroup.err = newErr;
    newGroup.count = 1;
    newGroup.displayMsg = newErr.errorMsg;
    ErrorGroups.push_back(newGroup);
  }

  void Clear() {
    ErrorGroups.clear();
    if (EditorRef) {
      EditorRef->ClearAllErrors();
    }
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    if (ImGui::Button("Clear")) {
      Clear();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &AutoScroll);

    ImGui::Separator();

    ImVec2 tableSize = ImGui::GetContentRegionAvail();

    if (ImGui::BeginTable("ErrorTable", 5,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_ScrollY,
                          tableSize)) {

      ImGui::TableSetupColumn("Count", ImGuiTableColumnFlags_WidthFixed, 40.0f);
      ImGui::TableSetupColumn("Script", ImGuiTableColumnFlags_WidthStretch,
                              0.3f);
      ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed, 50.0f);
      ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch,
                              0.6f);
      ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed,
                              60.0f);
      ImGui::TableHeadersRow();

      for (int i = 0; i < ErrorGroups.size(); ++i) {
        const auto &group = ErrorGroups[i];
        const auto &err = group.err;

        ImGui::PushID(i);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        if (group.count > 1)
          ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "%dx", group.count);
        else
          ImGui::Text("1");

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", err.script.c_str());
        if (ImGui::IsItemHovered())
          ImGui::SetTooltip("%s", err.script.c_str());

        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%d", err.line);

        ImGui::TableSetColumnIndex(3);
        ImGui::TextWrapped("%s", group.displayMsg.c_str());

        ImGui::TableSetColumnIndex(4);

        if (ImGui::Button("Go")) {
          if (EditorRef) {
            ImGui::SetWindowFocus("Script Editor");
            EditorRef->LoadFile(err.script);
            EditorRef->JumpToLine(err.line - 1);
          }
        }
        ImGui::PopID();
      }

      if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

      ImGui::EndTable();
    }
    ImGui::End();
  }
};
