#pragma once
#include "imgui.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

struct LogEntry {
  std::string msg;
  std::string category;
  int32_t colorCode;
};

class LogWindow {
  std::vector<LogEntry> Items;
  std::vector<int> DisplayIndices;
  std::set<std::string> Categories;

  std::set<int> SelectedItemsIndices;
  int FocusedDisplayIdx = -1;
  int AnchorDisplayIdx = -1;
  bool RequestScrollToFocus = false;

  ImGuiTextFilter Filter;

  bool AutoScroll = true;
  bool ScrollToBottom = false;

  std::string SelectedCategory = "ALL";
  bool NeedsFilterUpdate = false;

  ImVec4 GetColorForCode(int32_t col) const {
    if (col == 0)
      return ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    int r = (col >> 16) & 0xFF;
    int g = (col >> 8) & 0xFF;
    int b = col & 0xFF;
    return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
  }

  void RebuildFilteredList() {
    DisplayIndices.clear();
    for (int i = 0; i < Items.size(); i++) {
      const auto &item = Items[i];
      if (SelectedCategory != "ALL" && item.category != SelectedCategory)
        continue;
      if (!Filter.PassFilter(item.msg.c_str()) &&
          !Filter.PassFilter(item.category.c_str()))
        continue;
      DisplayIndices.push_back(i);
    }
    FocusedDisplayIdx = -1;
    AnchorDisplayIdx = -1;
    if (AutoScroll)
      ScrollToBottom = true;
  }

  void CopySelectedToClipboard() {
    if (SelectedItemsIndices.empty())
      return;
    std::string clipboardText;
    for (int idx : SelectedItemsIndices) {
      if (idx >= 0 && idx < Items.size()) {
        const auto &item = Items[idx];
        if (!item.category.empty())
          clipboardText += "[" + item.category + "] ";
        clipboardText += item.msg + "\n";
      }
    }
    if (!clipboardText.empty())
      ImGui::SetClipboardText(clipboardText.c_str());
  }

  void SelectRange(int startDisplayIdx, int endDisplayIdx) {
    int first = std::min(startDisplayIdx, endDisplayIdx);
    int last = std::max(startDisplayIdx, endDisplayIdx);
    if (first < 0)
      first = 0;
    if (last >= DisplayIndices.size())
      last = (int)DisplayIndices.size() - 1;

    for (int i = first; i <= last; i++) {
      SelectedItemsIndices.insert(DisplayIndices[i]);
    }
  }

  void HandleDragAutoscroll() {
    if (!ImGui::IsMouseDragging(0))
      return;
    if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
      return;

    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();

    float scrollThreshold = 30.0f;
    float scrollSpeed = 15.0f;

    if (mousePos.y < winPos.y + scrollThreshold) {
      ImGui::SetScrollY(ImGui::GetScrollY() - scrollSpeed);
    } else if (mousePos.y > winPos.y + winSize.y - scrollThreshold) {
      ImGui::SetScrollY(ImGui::GetScrollY() + scrollSpeed);
    }
  }

  void HandleKeyboardInputs() {
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
      return;

    ImGuiIO &io = ImGui::GetIO();
    bool shift = io.KeyShift;
    bool ctrl = io.KeyCtrl;

    if (ImGui::IsKeyPressed(ImGuiKey_C) && ctrl)
      CopySelectedToClipboard();

    if (ImGui::IsKeyPressed(ImGuiKey_A) && ctrl) {
      SelectedItemsIndices.clear();
      for (int idx : DisplayIndices)
        SelectedItemsIndices.insert(idx);
    }

    int moveDir = 0;
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
      moveDir = -1;
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
      moveDir = 1;

    if (moveDir != 0 && !DisplayIndices.empty()) {
      if (FocusedDisplayIdx == -1)
        FocusedDisplayIdx = (moveDir > 0) ? -1 : (int)DisplayIndices.size();

      int newIdx = FocusedDisplayIdx + moveDir;
      if (newIdx < 0)
        newIdx = 0;
      if (newIdx >= DisplayIndices.size())
        newIdx = (int)DisplayIndices.size() - 1;

      if (newIdx != FocusedDisplayIdx) {
        FocusedDisplayIdx = newIdx;
        RequestScrollToFocus = true;
        AutoScroll = false;

        if (shift) {
          if (AnchorDisplayIdx == -1)
            AnchorDisplayIdx = FocusedDisplayIdx;
          if (!ctrl)
            SelectedItemsIndices.clear();
          SelectRange(AnchorDisplayIdx, FocusedDisplayIdx);
        } else {
          SelectedItemsIndices.clear();
          SelectedItemsIndices.insert(DisplayIndices[FocusedDisplayIdx]);
          AnchorDisplayIdx = FocusedDisplayIdx;
        }
      }
    }
  }

public:
  LogWindow() { Categories.insert("ALL"); }

  void Clear() {
    Items.clear();
    DisplayIndices.clear();
    SelectedItemsIndices.clear();
    Categories.clear();
    Categories.insert("ALL");
    SelectedCategory = "ALL";
    FocusedDisplayIdx = -1;
    AnchorDisplayIdx = -1;
  }

  void AddLog(const std::string &msg, const std::string &category,
              int32_t colorCode) {
    Items.push_back({msg, category, colorCode});

    if (!category.empty() && Categories.find(category) == Categories.end()) {
      Categories.insert(category);
    }

    bool catMatch = (SelectedCategory == "ALL" || category == SelectedCategory);
    bool filterMatch =
        Filter.PassFilter(msg.c_str()) || Filter.PassFilter(category.c_str());

    if (catMatch && filterMatch) {
      DisplayIndices.push_back((int)Items.size() - 1);
      if (AutoScroll)
        ScrollToBottom = true;
    }
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    if (ImGui::Button("Clear"))
      Clear();
    ImGui::SameLine();
    // if (ImGui::Button("Copy")) CopySelectedToClipboard();
    // ImGui::SameLine();

    if (ImGui::Checkbox("Auto-scroll", &AutoScroll)) {
      if (AutoScroll)
        ScrollToBottom = true;
    }
    ImGui::SameLine();

    ImGui::SetNextItemWidth(150);
    if (ImGui::BeginCombo("##cat", SelectedCategory.c_str())) {
      for (const auto &cat : Categories) {
        if (ImGui::Selectable(cat.c_str(), SelectedCategory == cat)) {
          SelectedCategory = cat;
          NeedsFilterUpdate = true;
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    if (Filter.Draw("Search", -50.0f))
      NeedsFilterUpdate = true;

    if (NeedsFilterUpdate) {
      RebuildFilteredList();
      NeedsFilterUpdate = false;
    }
    ImGui::Separator();

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    HandleKeyboardInputs();

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
      HandleDragAutoscroll();

    ImVec4 headerCol = ImGui::GetStyle().Colors[ImGuiCol_Header];
    ImVec4 headerActiveCol = ImGui::GetStyle().Colors[ImGuiCol_HeaderActive];
    ImVec4 headerHover = ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered];
    headerCol.w *= 0.4f;
    headerActiveCol.w *= 0.4f;
    headerHover.w *= 0.4f;

    ImGui::PushStyleColor(ImGuiCol_Header, headerCol);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerActiveCol);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerHover);

    ImGuiListClipper clipper;
    clipper.Begin(DisplayIndices.size());

    while (clipper.Step()) {
      for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        int realIdx = DisplayIndices[i];
        const auto &item = Items[realIdx];

        ImVec4 col = GetColorForCode(item.colorCode);

        ImGui::PushID(realIdx);
        bool is_selected = SelectedItemsIndices.count(realIdx);

        ImGui::PushStyleColor(ImGuiCol_Text, col);
        std::string label = item.category.empty()
                                ? item.msg
                                : ("[" + item.category + "] " + item.msg);

        ImGui::Selectable(label.c_str(), is_selected,
                          ImGuiSelectableFlags_SpanAllColumns);

        ImVec2 itemMin = ImGui::GetItemRectMin();
        ImVec2 itemMax = ImGui::GetItemRectMax();

        if (ImGui::IsItemHovered()) {
          if (ImGui::IsMouseClicked(0)) {
            FocusedDisplayIdx = i;
            AutoScroll = false;

            if (ImGui::GetIO().KeyShift) {
              if (AnchorDisplayIdx == -1)
                AnchorDisplayIdx = i;
              if (!ImGui::GetIO().KeyCtrl)
                SelectedItemsIndices.clear();
              SelectRange(AnchorDisplayIdx, i);
            } else if (ImGui::GetIO().KeyCtrl) {
              AnchorDisplayIdx = i;
              if (is_selected)
                SelectedItemsIndices.erase(realIdx);
              else
                SelectedItemsIndices.insert(realIdx);
            } else {
              AnchorDisplayIdx = i;
              SelectedItemsIndices.clear();
              SelectedItemsIndices.insert(realIdx);
            }
          }
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsMouseDragging(0)) {
          if (ImGui::IsMouseHoveringRect(itemMin, itemMax)) {
            if (FocusedDisplayIdx != i) {
              FocusedDisplayIdx = i;
              AutoScroll = false;

              if (AnchorDisplayIdx != -1) {
                if (!ImGui::GetIO().KeyCtrl)
                  SelectedItemsIndices.clear();
                SelectRange(AnchorDisplayIdx, i);
              }
            }
          }
        }

        ImGui::PopStyleColor();

        if (RequestScrollToFocus && i == FocusedDisplayIdx) {
          ImGui::SetScrollHereY();
          RequestScrollToFocus = false;
        }

        ImGui::PopID();
      }
    }
    clipper.End();
    ImGui::PopStyleColor(3);

    if (ScrollToBottom && AutoScroll)
      ImGui::SetScrollHereY(1.0f);

    ScrollToBottom = false;

    ImGui::EndChild();
    ImGui::End();
  }
};
