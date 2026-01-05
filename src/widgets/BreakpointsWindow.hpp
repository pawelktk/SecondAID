#pragma once
#include "ScriptEditorWindow.hpp"
#include "imgui.h"
#include <functional>
#include <string>
#include <vector>

struct BreakpointUI {
  std::string File;
  int Line;
  int ID;
};

class BreakpointsWindow {
  ScriptEditorWindow *EditorRef = nullptr;
  std::vector<BreakpointUI> CachedList;
  std::function<void(int)> OnRemoveRequest;
  std::function<void()> OnClearAllRequest;

public:
  void Setup(ScriptEditorWindow *editor, std::function<void(int)> onRemove,
             std::function<void()> onClear) {
    EditorRef = editor;
    OnRemoveRequest = onRemove;
    OnClearAllRequest = onClear;
  }

  void UpdateList(const std::vector<BreakpointInfo> &apiList) {
    CachedList.clear();
    for (size_t i = 0; i < apiList.size(); i++) {
      CachedList.push_back({apiList[i].File, apiList[i].Line, (int)i});
    }
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    if (ImGui::Button("Delete All")) {
      if (OnClearAllRequest)
        OnClearAllRequest();
    }

    ImGui::Separator();

    if (ImGui::BeginTable("BPTable", 3,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_ScrollY)) {
      ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed, 50.0f);
      ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed,
                              100.0f);
      ImGui::TableHeadersRow();

      for (int i = 0; i < CachedList.size(); ++i) {
        const auto &bp = CachedList[i];

        ImGui::PushID(i);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", bp.File.c_str());
        if (ImGui::IsItemHovered())
          ImGui::SetTooltip("%s", bp.File.c_str());

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d", bp.Line);

        ImGui::TableSetColumnIndex(2);

        if (ImGui::Button("Go")) {
          if (EditorRef) {
            EditorRef->LoadFile(bp.File);
            EditorRef->JumpToLine(bp.Line - 1);
          }
        }
        ImGui::SameLine();
        if (ImGui::Button("X")) {
          if (OnRemoveRequest)
            OnRemoveRequest(bp.ID);
        }

        ImGui::PopID();
      }
      ImGui::EndTable();
    }
    ImGui::End();
  }
};
