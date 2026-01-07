#pragma once
#include "ScriptAPI.hpp"
#include "TextEditor.h"
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>
#include <vector>

class AutocompleteSystem {
private:
  struct CompletionCandidate {
    const ScriptAPI::FuncDesc *func;
    int score;
  };

  std::vector<CompletionCandidate> suggestions;
  int selectedIndex = 0;
  bool isOpen = false;
  std::string currentQuery;

  int startLine = 0;
  int startCol = 0;
  ImVec2 popupPos;
  bool scrollToSelection = false;
  bool wasTyping = false;

  int preCursorLine = 0;
  int preCursorCol = 0;
  int preUndoIndex = 0;
  bool handleNav = false;
  bool handleInsert = false;

  const std::unordered_set<std::string> luaKeywords = {
      "and", "break",    "do",     "else", "elseif", "end",   "false",
      "for", "function", "if",     "in",   "local",  "nil",   "not",
      "or",  "repeat",   "return", "then", "true",   "until", "while"};

public:
  bool Enabled = true;
  bool SuppressOnLuaKeywords = true;

  AutocompleteSystem() = default;

  void PreRender(TextEditor &editor) {
    if (!Enabled) {
      isOpen = false;
      return;
    }
    ImGuiIO &io = ImGui::GetIO();
    wasTyping = false;
    handleNav = false;
    handleInsert = false;

    editor.GetCurrentCursor(preCursorLine, preCursorCol);
    preUndoIndex = editor.GetUndoIndex();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
        ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
      if (isOpen) {
        isOpen = false;
        int line, col;
        editor.GetCurrentCursor(line, col);
        currentQuery = GetWordUnderCursor(editor.GetLineText(line), col);
      }
      return;
    }

    if (isOpen && !suggestions.empty()) {
      if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
        selectedIndex = (selectedIndex - 1 + (int)suggestions.size()) %
                        (int)suggestions.size();
        scrollToSelection = true;
        handleNav = true;
        ConsumeKey(ImGuiKey_UpArrow);
      } else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
        selectedIndex = (selectedIndex + 1) % (int)suggestions.size();
        scrollToSelection = true;
        handleNav = true;
        ConsumeKey(ImGuiKey_DownArrow);
      } else if (ImGui::IsKeyPressed(ImGuiKey_Enter) ||
                 ImGui::IsKeyPressed(ImGuiKey_Tab)) {
        handleInsert = true;
        ConsumeKey(ImGuiKey_Enter);
        ConsumeKey(ImGuiKey_Tab);
      } else if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        isOpen = false;
        ConsumeKey(ImGuiKey_Escape);
      }
    }

    if (!io.InputQueueCharacters.empty() ||
        ImGui::IsKeyPressed(ImGuiKey_Backspace) ||
        ImGui::IsKeyPressed(ImGuiKey_Delete)) {
      wasTyping = true;
    }
  }

  void PostRender(TextEditor &editor) {
    if (!Enabled)
      return;

    if (handleNav) {
      editor.SetCursor(preCursorLine, preCursorCol);
    }

    if (handleInsert) {
      if (editor.GetUndoIndex() > preUndoIndex) {
        editor.Undo();
      }
      ApplyCompletion(editor);
      isOpen = false;
      return;
    }

    int line, col;
    editor.GetCurrentCursor(line, col);
    std::string lineText = editor.GetLineText(line);
    std::string word = GetWordUnderCursor(lineText, col);

    float lineHeight = editor.GetLineHeight();
    float glyphWidth = editor.GetGlyphWidth();
    int firstVisLine = editor.GetFirstVisibleLine();
    int firstVisCol = editor.GetFirstVisibleColumn();

    float localX = (col - firstVisCol) * glyphWidth + 40.0f;
    float localY = (line - firstVisLine + 1) * lineHeight;
    ImVec2 screenPos = ImGui::GetWindowPos();
    popupPos = ImVec2(screenPos.x + localX, screenPos.y + localY);

    if (word.empty()) {
      isOpen = false;
      currentQuery = "";
      return;
    }

    if (SuppressOnLuaKeywords && IsLuaKeyword(word)) {
      isOpen = false;
      currentQuery = word;
      return;
    }

    bool wordChanged = (word != currentQuery);

    if (wordChanged && wasTyping) {
      currentQuery = word;
      BuildSuggestions(currentQuery);

      if (!suggestions.empty()) {
        isOpen = true;
        startLine = line;
        startCol = col - (int)word.length();

        if (word.length() <= 1)
          selectedIndex = 0;
        else if (selectedIndex >= suggestions.size())
          selectedIndex = 0;

        scrollToSelection = true;
      } else {
        isOpen = false;
      }
    } else if (!wasTyping && !isOpen) {
      currentQuery = word;
    }
  }

  void DrawPopup() {
    if (!Enabled || !isOpen || suggestions.empty())
      return;

    ImGui::SetNextWindowPos(popupPos);
    ImGui::SetNextWindowSizeConstraints(ImVec2(350, 0), ImVec2(800, 500));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.12f, 0.12f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

    if (ImGui::Begin("##AutocompletePopup", nullptr, flags)) {

      if (ImGui::BeginChild("##List", ImVec2(0, 120), false)) {
        for (int i = 0; i < suggestions.size(); ++i) {
          const auto &item = suggestions[i];
          bool isSelected = (i == selectedIndex);

          ImGui::TextColored(ImVec4(0.35f, 0.6f, 0.85f, 1.0f), "%s",
                             item.func->ReturnType);
          ImGui::SameLine();

          if (ImGui::Selectable(item.func->Name, isSelected)) {
            selectedIndex = i;
          }

          if (isSelected && scrollToSelection) {
            ImGui::SetScrollHereY();
            scrollToSelection = false;
          }
        }
        ImGui::EndChild();
      }

      ImGui::Separator();
      if (selectedIndex >= 0 && selectedIndex < suggestions.size()) {
        const auto &item = suggestions[selectedIndex];

        ImGui::TextColored(ImVec4(0.35f, 0.6f, 0.85f, 1.0f), "%s",
                           item.func->ReturnType);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s",
                           item.func->Name);

        ImGui::SameLine();
        ImGui::TextDisabled("(");
        ImGui::SameLine();
        if (item.func->Params.empty()) {
          ImGui::TextDisabled(")");
        } else {
          ImGui::TextDisabled("...");
          ImGui::SameLine();
          ImGui::TextDisabled(")");
        }

        ImGui::Spacing();
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 600.0f);
        ImGui::TextWrapped("%s", item.func->Desc);
        ImGui::PopTextWrapPos();
        ImGui::Spacing();

        if (!item.func->Params.empty()) {
          ImGui::Separator();
          ImGui::TextDisabled("Arguments:");

          if (ImGui::BeginTable("##ArgsTable", 2,
                                ImGuiTableFlags_SizingFixedFit |
                                    ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Decl", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Desc", ImGuiTableColumnFlags_WidthStretch);

            for (const auto &p : item.func->Params) {
              ImGui::TableNextRow();

              ImGui::TableSetColumnIndex(0);
              ImGui::TextColored(ImVec4(0.3f, 0.7f, 0.6f, 1.0f), "%s", p.Type);
              ImGui::SameLine();
              ImGui::Text("%s", p.Name);
              if (p.IsOptional) {
                ImGui::SameLine();
                ImGui::TextDisabled("?");
              }

              ImGui::TableSetColumnIndex(1);
              if (strlen(p.Desc) > 0) {
                ImGui::PushStyleColor(ImGuiCol_Text,
                                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::TextWrapped("%s", p.Desc);
                ImGui::PopStyleColor();
              } else {
                ImGui::TextDisabled("-");
              }
            }
            ImGui::EndTable();
          }
        }
      }

      ImGui::End();
    }
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
  }

private:
  bool IsLuaKeyword(const std::string &word) {
    return luaKeywords.find(word) != luaKeywords.end();
  }

  void ConsumeKey(ImGuiKey key) {
    ImGui::GetIO().InputQueueCharacters.clear();
    ImGui::GetIO().AddKeyEvent(key, false);
  }

  std::string GetWordUnderCursor(const std::string &line, int col) {
    if (col > (int)line.length())
      col = (int)line.length();
    int start = col;
    while (start > 0) {
      char c = line[start - 1];
      if (!isalnum(c) && c != '_')
        break;
      start--;
    }
    return line.substr(start, col - start);
  }

  void BuildSuggestions(const std::string &query) {
    suggestions.clear();
    auto &allFuncs = ScriptAPI::Database::GetEngineFunctions();

    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(),
                   ::tolower);

    for (const auto &func : allFuncs) {
      std::string nameLower = func.Name;
      std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(),
                     ::tolower);

      size_t pos = nameLower.find(queryLower);
      if (pos != std::string::npos) {
        int score = (pos == 0 ? 100 : 50) - (int)nameLower.length();
        suggestions.push_back({&func, score});
      }
    }
    std::sort(suggestions.begin(), suggestions.end(),
              [](const CompletionCandidate &a, const CompletionCandidate &b) {
                if (a.score != b.score)
                  return a.score > b.score;
                return std::string(a.func->Name) < std::string(b.func->Name);
              });
  }

  void ApplyCompletion(TextEditor &editor) {
    if (suggestions.empty())
      return;
    const auto *func = suggestions[selectedIndex].func;

    int currentLine, currentCol;
    editor.GetCurrentCursor(currentLine, currentCol);
    editor.SelectRegion(startLine, startCol, currentLine, currentCol);

    std::string insertText = std::string(func->Name) + "(";
    if (func->Params.empty())
      insertText += ")";

    editor.ReplaceTextInCurrentCursor(insertText);
  }
};
