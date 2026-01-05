#pragma once
#include "ScriptEditorWindow.hpp"
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

class StaticAnalysisOverview {
  ScriptEditorWindow *EditorRef = nullptr;
  ImGuiTextFilter Filter;
  bool ShowErrorsOnly = false;

  std::vector<int> FilteredIndices;
  bool FilterDirty = true;
  size_t LastResultsSize = 0;

  std::string GetShortPath(const std::string &fullPath) {
    std::string lower = fullPath;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    std::replace(lower.begin(), lower.end(), '\\', '/');

    size_t pos = lower.find("scripts/");
    if (pos != std::string::npos) {
      return fullPath.substr(pos + 8);
    }
    return fullPath;
  }

  void UpdateFilteredList() {
    if (!EditorRef)
      return;
    FilteredIndices.clear();
    const auto &results = EditorRef->GlobalLintResults;

    for (int i = 0; i < results.size(); ++i) {
      const auto &item = results[i];
      if (ShowErrorsOnly && item.type != "E")
        continue;
      if (!Filter.PassFilter(item.file.c_str()) &&
          !Filter.PassFilter(item.message.c_str()))
        continue;
      FilteredIndices.push_back(i);
    }
    FilterDirty = false;
    LastResultsSize = results.size();
  }

public:
  void Setup(ScriptEditorWindow *editor) { EditorRef = editor; }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    if (!EditorRef) {
      ImGui::End();
      return;
    }
    if (EditorRef->GlobalLintResults.size() != LastResultsSize) {
      FilterDirty = true;
    }

    if (ImGui::Button("Run Analysis (All Scripts)")) {
      EditorRef->RunGlobalAnalysis();
    }

    ImGui::SameLine();
    if (EditorRef->IsGlobalAnalysisInProgress) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Scanning... Please wait.");
    } else {
      ImGui::TextDisabled("%zu issues found.",
                          EditorRef->GlobalLintResults.size());
    }

    ImGui::Separator();

    if (ImGui::Checkbox("Errors Only", &ShowErrorsOnly))
      FilterDirty = true;
    ImGui::SameLine();
    if (Filter.Draw("Filter Path/Msg", 200))
      FilterDirty = true;

    if (FilterDirty)
      UpdateFilteredList();

    ImGui::Separator();

    ImVec2 tableSize = ImGui::GetContentRegionAvail();
    if (ImGui::BeginTable("GlobalLintTable", 5,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_ScrollY |
                              ImGuiTableFlags_Sortable,
                          tableSize)) {

      ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthStretch, 0.3f);
      ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed, 50.0f);
      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 40.0f);
      ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch,
                              0.6f);
      ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed,
                              50.0f);
      ImGui::TableHeadersRow();

      ImGuiListClipper clipper;
      clipper.Begin(FilteredIndices.size());

      while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          int realIdx = FilteredIndices[i];
          const auto &item = EditorRef->GlobalLintResults[realIdx];

          ImGui::PushID(realIdx);
          ImGui::TableNextRow();

          ImGui::TableSetColumnIndex(0);
          std::string shortPath = GetShortPath(item.file);
          ImGui::Text("%s", shortPath.c_str());
          if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", item.file.c_str());

          // Line
          ImGui::TableSetColumnIndex(1);
          ImGui::Text("%d", item.line);

          // Type
          ImGui::TableSetColumnIndex(2);
          if (item.type == "E")
            ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "ERR");
          else
            ImGui::TextColored(ImVec4(1, 0.9f, 0.2f, 1), "WARN");

          // Message
          ImGui::TableSetColumnIndex(3);
          ImGui::TextWrapped("%s", item.message.c_str());

          ImGui::TableSetColumnIndex(4);
          if (ImGui::Button("Go")) {
            EditorRef->LoadFile(shortPath);
            EditorRef->JumpToLine(item.line - 1);
            ImGui::SetWindowFocus("Script Editor");
          }

          ImGui::PopID();
        }
      }
      ImGui::EndTable();
    }

    ImGui::End();
  }
};
