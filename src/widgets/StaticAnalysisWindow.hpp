#pragma once
#include "ScriptEditorWindow.hpp"
#include "imgui.h"
#include <string>
#include <vector>

class StaticAnalysisWindow {
  ScriptEditorWindow *EditorRef = nullptr;

public:
  void Setup(ScriptEditorWindow *editor) { EditorRef = editor; }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }
    if (!EditorRef) {
      ImGui::Text("No editor linked.");
      ImGui::End();
      return;
    }
    auto doc = EditorRef->GetActiveDocument();

    if (!doc) {
      ImGui::TextDisabled("No script open.");
      ImGui::End();
      return;
    }

    ImGui::Text("Analysis for: %s", doc->FileName.c_str());
    ImGui::SameLine();
    if (ImGui::SmallButton("Re-check Now")) {
      EditorRef->ForceLintActive();
    }

    ImGui::Separator();

    if (doc->LintWarnings.empty()) {
      ImGui::TextColored(ImVec4(0, 1, 0, 1), "No issues found.");
    } else {
      ImVec2 tableSize = ImGui::GetContentRegionAvail();
      if (ImGui::BeginTable("LintTable", 4,
                            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_Resizable |
                                ImGuiTableFlags_ScrollY,
                            tableSize)) {

        ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed,
                                40.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed,
                                40.0f);
        ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed,
                                50.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < doc->LintWarnings.size(); ++i) {
          const auto &warn = doc->LintWarnings[i];
          ImGui::PushID(i);
          ImGui::TableNextRow();

          ImGui::TableSetColumnIndex(0);
          ImGui::Text("%d", warn.line);

          ImGui::TableSetColumnIndex(1);
          if (warn.type == "E")
            ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "ERR");
          else
            ImGui::TextColored(ImVec4(1, 0.9f, 0.2f, 1), "WARN");

          ImGui::TableSetColumnIndex(2);
          ImGui::TextWrapped("%s", warn.message.c_str());

          ImGui::TableSetColumnIndex(3);
          if (ImGui::Button("Go")) {
            EditorRef->JumpToLine(warn.line - 1);
            ImGui::SetWindowFocus("Script Editor");
          }
          ImGui::PopID();
        }
        ImGui::EndTable();
      }
    }
    ImGui::End();
  }
};
