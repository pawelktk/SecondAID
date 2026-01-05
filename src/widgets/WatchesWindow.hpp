#pragma once
#include "imgui.h"
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

struct WatchHistoryEntry {
  std::string Value;
  std::string ContextType;
  std::string File;
  int Line;
};

struct WatchItem {
  std::string Expression;
  std::string CurrentValue;
  std::vector<WatchHistoryEntry> History;
  bool ShowHistoryPopup = false;
};

class WatchesWindow {
  std::vector<WatchItem> Items;
  char InputBuf[256] = "";

  std::function<void(std::string)> OnAddRequest;
  std::function<void(int)> OnRemoveRequest;
  std::function<void()> OnClearAllRequest;

public:
  void Setup(std::function<void(std::string)> onAdd,
             std::function<void(int)> onRemove, std::function<void()> onClear) {
    OnAddRequest = onAdd;
    OnRemoveRequest = onRemove;
    OnClearAllRequest = onClear;
  }

  void UpdateWatchValue(const std::string &expression, const std::string &value,
                        const std::string &ctxType, const std::string &file,
                        int line) {

    for (auto &item : Items) {
      if (item.Expression == expression) {
        item.CurrentValue = value;
        bool isDuplicate = false;
        if (!item.History.empty()) {
          if (item.History.back().Value == value)
            isDuplicate = true;
        }

        if (!isDuplicate) {
          WatchHistoryEntry entry;
          entry.Value = value;
          entry.ContextType = ctxType;
          entry.File = file;
          entry.Line = line;
          item.History.push_back(entry);
        }
        return;
      }
    }
    WatchItem newItem;
    newItem.Expression = expression;
    newItem.CurrentValue = value;
    newItem.History.push_back({value, ctxType, file, line});
    Items.push_back(newItem);
  }

  void SyncList(const std::vector<std::string> &apiList) {
    auto it = std::remove_if(
        Items.begin(), Items.end(), [&](const WatchItem &localItem) {
          return std::find(apiList.begin(), apiList.end(),
                           localItem.Expression) == apiList.end();
        });
    Items.erase(it, Items.end());

    for (const auto &exp : apiList) {
      bool exists = false;
      for (const auto &local : Items) {
        if (local.Expression == exp) {
          exists = true;
          break;
        }
      }
      if (!exists) {
        WatchItem newItem;
        newItem.Expression = exp;
        newItem.CurrentValue = "???";
        Items.push_back(newItem);
      }
    }
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    // --- INPUT ---
    if (ImGui::BeginChild("ControlsRegion", ImVec2(0, 35), false,
                          ImGuiWindowFlags_NoScrollbar)) {
      if (ImGui::Button("Add")) {
        if (OnAddRequest && strlen(InputBuf) > 0) {
          OnAddRequest(std::string(InputBuf));
          memset(InputBuf, 0, sizeof(InputBuf));
        }
      }
      ImGui::SameLine();
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 70);
      ImGui::InputTextWithHint("##newwatch", "Expression...", InputBuf,
                               sizeof(InputBuf),
                               ImGuiInputTextFlags_EnterReturnsTrue);
      if (ImGui::IsItemDeactivatedAfterEdit() &&
          ImGui::IsKeyPressed(ImGuiKey_Enter) && strlen(InputBuf) > 0) {
        if (OnAddRequest) {
          OnAddRequest(std::string(InputBuf));
          memset(InputBuf, 0, sizeof(InputBuf));
          ImGui::SetKeyboardFocusHere(-1);
        }
      }

      ImGui::SameLine();
      if (ImGui::Button("Del All")) {
        if (OnClearAllRequest)
          OnClearAllRequest();
        Items.clear();
      }
    }
    ImGui::EndChild();

    // ImGui::Separator();

    // --- TABLE ---
    if (ImGui::BeginTable("WatchesTable", 4,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_ScrollY)) {
      ImGui::TableSetupColumn("Expression", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Hist", ImGuiTableColumnFlags_WidthFixed, 40.0f);
      ImGui::TableSetupColumn("Del", ImGuiTableColumnFlags_WidthFixed, 30.0f);
      ImGui::TableHeadersRow();

      for (int i = 0; i < Items.size(); ++i) {
        auto &item = Items[i];
        ImGui::PushID(i);

        ImGui::TableNextRow();

        // Col 1: Expression
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", item.Expression.c_str());

        // Col 2: Value
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 1.0f, 1.0f), "%s",
                           item.CurrentValue.c_str());

        // Col 3: History Button
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("H")) {
          ImGui::OpenPopup("HistoryPopup");
        }

        // --- HISTORY POPUP ---
        if (ImGui::BeginPopup("HistoryPopup")) {
          ImGui::Text("History for: %s", item.Expression.c_str());
          ImGui::Separator();
          if (item.History.empty()) {
            ImGui::TextDisabled("No history yet.");
          } else {
            if (ImGui::BeginTable("HistTable", 3, ImGuiTableFlags_Borders)) {
              ImGui::TableSetupColumn("Value");
              ImGui::TableSetupColumn("Reason");
              ImGui::TableSetupColumn("Loc");
              ImGui::TableHeadersRow();

              for (auto it = item.History.rbegin(); it != item.History.rend();
                   ++it) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", it->Value.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::TextDisabled("%s", it->ContextType.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::TextDisabled("%s:%d", it->File.c_str(), it->Line);
              }
              ImGui::EndTable();
            }
          }
          ImGui::EndPopup();
        }

        // Col 4: Delete
        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("X")) {
          if (OnRemoveRequest)
            OnRemoveRequest(i);
          Items.erase(Items.begin() + i);
          ImGui::PopID();
          continue;
        }

        ImGui::PopID();
      }
      ImGui::EndTable();
    }
    ImGui::End();
  }
};
