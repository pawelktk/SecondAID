#pragma once
#include "../tools/AsyncLuaLinter.hpp"
#include "../tools/GlobalsManager.hpp"
#include "../tools/LuaLinter.hpp"
#include "ImGuiFileDialog.h"
#include "TextDiff.h"
#include "TextEditor.h"
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class ScriptDocument {
  std::string GetWordAt(int line, int visualCol) {
    if (line >= editor.GetLineCount())
      return "";
    std::string text = editor.GetLineText(line);
    if (text.empty())
      return "";

    int index = 0;
    int currentVisual = 0;
    int tabSize = editor.GetTabSize();

    bool found = false;
    for (size_t i = 0; i < text.size(); ++i) {
      int charWidth = 1;
      if (text[i] == '\t') {
        charWidth = tabSize - (currentVisual % tabSize);
      }
      if (visualCol < currentVisual + charWidth) {
        index = i;
        found = true;
        break;
      }
      currentVisual += charWidth;
    }

    if (!found)
      return "";
    // -----------------------------------------------------------

    if (!isalnum(text[index]) && text[index] != '_')
      return "";

    int start = index;
    int end = index;

    while (start > 0 && (isalnum(text[start - 1]) || text[start - 1] == '_')) {
      start--;
    }
    while (end < text.size() && (isalnum(text[end]) || text[end] == '_')) {
      end++;
    }

    return text.substr(start, end - start);
  }

  std::string GetSelectedTextManual() {
    if (!editor.AnyCursorHasSelection())
      return "";

    size_t count = editor.GetNumberOfCursors();
    for (size_t i = 0; i < count; ++i) {
      int startLine, startCol, endLine, endCol;
      editor.GetCursor(startLine, startCol, endLine, endCol, i);

      if (startLine != endLine || startCol != endCol) {
        return editor.GetCursorText(i);
      }
    }
    return "";
  }

public:
  TextEditor editor;
  TextDiff diff;

  std::string FullPath;
  std::string RelativePath;
  std::string RelativePathLower;
  std::string FileName;

  bool IsCachedView = false;
  bool IsDiffOpen = false;
  bool BringToFront = false;

  std::vector<LintResult> LintWarnings;

  int LastLintedUndoIndex = -1;
  int KnownUndoIndex = 0;
  std::chrono::steady_clock::time_point LastEditTime;
  bool NeedsLinting = false;

  size_t SavedUndoIndex = 0;

  std::string LastCachedContent;
  bool IgnoreWhitespacesInDiff = false;
  int ExecutionLine = -1;

  bool IsGotoLineOpen = false;
  int GotoLineNumber = 1;
  bool RequestSmartIndent = false;

  std::function<void(std::string)> OnDiffRequested;
  std::function<void(std::string, int, bool)> OnBreakpointChanged;
  std::function<void(std::string)> OnAddWatchRequested;
  std::function<void(std::string)> OnGoToDefRequested;
  std::set<int> CachedBreakpoints;

  ScriptDocument(const std::string &fullPath, const std::string &relPath,
                 const TextEditor::Language *lang)
      : FullPath(fullPath), RelativePath(relPath) {
    RelativePathLower = relPath;
    std::transform(RelativePathLower.begin(), RelativePathLower.end(),
                   RelativePathLower.begin(), ::tolower);
    std::replace(RelativePathLower.begin(), RelativePathLower.end(), '\\', '/');
    if (RelativePathLower.find("scripts/") != 0)
      RelativePathLower = "scripts/" + RelativePathLower;

    FileName = fs::path(fullPath).filename().string();

    editor.SetLanguage(lang);
    editor.SetPalette(TextEditor::GetDarkPalette());
    editor.SetShowWhitespacesEnabled(false);
    editor.SetTabSize(4);
    editor.SetShowLineNumbersEnabled(true);
    editor.SetShowMatchingBrackets(true);
    editor.SetShowScrollbarMiniMapEnabled(true);
    editor.SetReadOnlyEnabled(false);

    diff.SetLanguage(lang);

    SetupLineDecorator();

    SetupContextMenus();
    LoadFromDisk();
  }

  ScriptDocument(const std::string &name, const std::string &content,
                 const TextEditor::Language *lang, bool isCached)
      : FullPath(""), RelativePath(name), FileName(name),
        IsCachedView(isCached) {
    RelativePathLower = name;
    editor.SetLanguage(lang);
    editor.SetPalette(TextEditor::GetDarkPalette());
    editor.SetText(content);
    editor.SetReadOnlyEnabled(true);
    diff.SetLanguage(lang);

    SetupLineDecorator();

    SetupContextMenus();
  }

  ScriptDocument(const std::string &name, const TextEditor::Language *lang)
      : FullPath(""), RelativePath(name), FileName(name) {
    RelativePathLower = name;

    editor.SetLanguage(lang);
    editor.SetPalette(TextEditor::GetDarkPalette());
    editor.SetShowWhitespacesEnabled(false);
    editor.SetTabSize(4);
    editor.SetShowLineNumbersEnabled(true);
    editor.SetShowMatchingBrackets(true);
    editor.SetReadOnlyEnabled(false);
    SavedUndoIndex = 0;

    diff.SetLanguage(lang);

    SetupLineDecorator();
    SetupContextMenus();
  }

  void SetupLineDecorator() {
    editor.SetLineDecorator(20.0f, [this](TextEditor::Decorator &d) {
      int line = d.line;

      bool isExecution = (ExecutionLine >= 0 && line == (ExecutionLine));
      bool isBreakpoint = (CachedBreakpoints.count(line) > 0);

      float size = d.height * 0.7f;
      auto drawList = ImGui::GetWindowDrawList();
      auto pos = ImGui::GetCursorScreenPos();

      ImVec2 center = ImVec2(pos.x + d.width * 0.5f, pos.y + d.height * 0.5f);

      // execution pointer - green triangle
      // TODO make that triangle less ugly
      if (isExecution) {
        // drawList->AddCircleFilled(center, size * 0.5f, IM_COL32(50, 255, 50,
        // 255));

        drawList->AddTriangleFilled(
            ImVec2(pos.x + 2, pos.y + 2),
            ImVec2(pos.x + 2, pos.y + d.height - 2),
            ImVec2(pos.x + d.width - 2, pos.y + d.height * 0.5f),
            IM_COL32(50, 255, 50, 255));
      }
      // breakpoints - red dot
      else if (isBreakpoint) {
        drawList->AddCircleFilled(center, size * 0.45f,
                                  IM_COL32(255, 50, 50, 255));
      }

      ImGui::SetCursorScreenPos(pos);
      std::string btnId = "##dec_" + std::to_string(line);

      if (ImGui::InvisibleButton(btnId.c_str(), ImVec2(d.width, d.height))) {
        if (OnBreakpointChanged) {
          bool exists = isBreakpoint;
          OnBreakpointChanged(RelativePathLower, line, !exists);
        }
      }

      // Tooltip
      if (ImGui::IsItemHovered()) {
        if (isExecution)
          ImGui::SetTooltip("Execution Paused Here");
        else if (isBreakpoint)
          ImGui::SetTooltip("Breakpoint (Click to remove)");
        else
          ImGui::SetTooltip("Click to add Breakpoint");
      }
    });
  }

  void RefreshMarkers(const std::set<int> &currentBreakpoints,
                      const std::set<int> &errorLines) {
    editor.ClearMarkers();
    // 1. breakpoints - we use decorators instead of markers
    CachedBreakpoints = currentBreakpoints;

    // 2. Runtime Errors - purple
    for (int line : errorLines) {
      editor.AddMarker(line, IM_COL32(220, 50, 220, 255),
                       IM_COL32(80, 0, 80, 255), "Lua Error",
                       "Error occured here");
    }

    // 3. Lint Warnings - yelow
    for (const auto &w : LintWarnings) {
      editor.AddMarker(
          w.line - 1, IM_COL32(255, 200, 0, 255), IM_COL32(100, 80, 0, 255),
          (w.type == "E" ? "Error" : "Warning"), w.message.c_str());
    }
  }

  void RunLinter() {
    if (FullPath.empty())
      return;
    LintWarnings = LuaLinter::Run(FullPath);
  }

  void ClearLinter() { LintWarnings.clear(); }

  bool IsDirty() const {
    if (IsCachedView)
      return false;
    return editor.GetUndoIndex() != SavedUndoIndex;
  }

  void LoadFromDisk() {
    std::ifstream t(FullPath);
    if (t.good()) {
      std::stringstream buffer;
      buffer << t.rdbuf();
      editor.SetText(buffer.str());
      SavedUndoIndex = editor.GetUndoIndex();
    } else {
      editor.SetText("-- Error: Could not open file:\n-- " + FullPath);
      editor.SetReadOnlyEnabled(true);
    }
  }

  bool Save() {
    if (IsCachedView || FullPath.empty())
      return false;
    std::ofstream t(FullPath);
    if (t.good()) {
      t << editor.GetText();
      SavedUndoIndex = editor.GetUndoIndex();
      return true;
    }
    return false;
  }

  void SetPathAfterSaveAs(const std::string &fullPath,
                          const std::string &relPath) {
    FullPath = fullPath;
    RelativePath = relPath;

    RelativePathLower = relPath;
    std::transform(RelativePathLower.begin(), RelativePathLower.end(),
                   RelativePathLower.begin(), ::tolower);
    std::replace(RelativePathLower.begin(), RelativePathLower.end(), '\\', '/');
    if (RelativePathLower.find("scripts/") != 0)
      RelativePathLower = "scripts/" + RelativePathLower;

    FileName = fs::path(fullPath).filename().string();

    SavedUndoIndex = editor.GetUndoIndex();
  }

  void SetExecutionLine(int line) {
    ExecutionLine = line;
    if (line >= 0) {
      editor.SetCursor(line, 0);
      editor.ScrollToLine(line, TextEditor::Scroll::alignMiddle);
      editor.SetFocus();
    }
  }

  void ClearExecutionLine() { ExecutionLine = -1; }

  void JumpToLine(int line) {
    if (line >= 0 && line < editor.GetLineCount()) {
      editor.SetCursor(line, 0);
      editor.ScrollToLine(line, TextEditor::Scroll::alignMiddle);
      editor.SetFocus();
    }
  }

  std::string TrimLines(const std::string &input) {
    std::stringstream ss(input);
    std::stringstream out;
    std::string line;
    while (std::getline(ss, line)) {
      line.erase(line.begin(),
                 std::find_if(line.begin(), line.end(), [](unsigned char ch) {
                   return !std::isspace(ch);
                 }));
      line.erase(
          std::find_if(line.rbegin(), line.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          line.end());
      out << line << "\n";
    }
    return out.str();
  }

  void RefreshDiff() {
    std::string local = editor.GetText();
    std::string game = LastCachedContent;
    if (!RelativePath.empty()) {
      try {
        fs::path p(RelativePath);
        std::string stem = p.stem().string();
        std::transform(stem.begin(), stem.end(), stem.begin(), ::tolower);
        std::regex re("function\\s+" + stem + "_");
        game = std::regex_replace(game, re, "function ");
      } catch (...) {
      }
    }
    if (IgnoreWhitespacesInDiff) {
      local = TrimLines(local);
      game = TrimLines(game);
    }
    diff.SetText(local, game);
  }

  void OpenDiffView(const std::string &cachedContent) {
    LastCachedContent = cachedContent;
    RefreshDiff();
    IsDiffOpen = true;
  }

  void RequestDiff() {
    if (OnDiffRequested)
      OnDiffRequested(RelativePathLower);
  }

  bool ShouldIndent(const std::string &lineContent) {
    std::string s = lineContent;
    size_t commentPos = s.find("--");
    if (commentPos != std::string::npos)
      s = s.substr(0, commentPos);
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());

    if (s.empty())
      return false;

    if (s.back() == '{' || s.back() == '(')
      return true;

    static const std::vector<std::string> indentTriggers = {"then", "do",
                                                            "repeat", "else"};
    for (const auto &trigger : indentTriggers) {
      if (s.size() >= trigger.size()) {
        if (s.compare(s.size() - trigger.size(), trigger.size(), trigger) ==
            0) {
          if (s.size() == trigger.size() ||
              !std::isalnum(s[s.size() - trigger.size() - 1])) {
            return true;
          }
        }
      }
    }

    if (s.find("function") != std::string::npos &&
        s.find("end") == std::string::npos)
      return true;
    return false;
  }

  void MarkDirtyForLint() {
    NeedsLinting = true;
    LastEditTime = std::chrono::steady_clock::now();
  }

  void HandleInputs() {
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
      return;
    ImGuiIO &io = ImGui::GetIO();

    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
      int line, col;
      editor.GetCurrentCursor(line, col);
      std::string currentLineText = editor.GetLineText(line);
      if (ShouldIndent(currentLineText)) {
        RequestSmartIndent = true;
      }
    }

    int currentUndo = editor.GetUndoIndex();
    if (currentUndo != LastLintedUndoIndex) {
      // TODO
    }

    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))
      Save();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_G)) {
      IsGotoLineOpen = true;
      GotoLineNumber = 1;
    }
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_F))
      editor.OpenFindReplaceWindow();

    if (ImGui::IsKeyPressed(ImGuiKey_F2)) {
      int line, col;
      editor.GetCurrentCursor(line, col);
      if (OnBreakpointChanged)
        OnBreakpointChanged(RelativePathLower, line, true);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_F12)) {
      int line, col;
      editor.GetCurrentCursor(line, col);
      std::string word = GetWordAt(line, col);
      std::string sel = GetSelectedTextManual();
      if (!sel.empty())
        word = sel;

      if (!word.empty() && OnGoToDefRequested) {
        OnGoToDefRequested(word);
      }
    }
  }

  void SetupContextMenus() {
    editor.SetTextContextMenuCallback([this](int line, int col) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, editor.CanUndo()))
        editor.Undo();
      if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, editor.CanRedo()))
        editor.Redo();
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr,
                          !editor.IsReadOnlyEnabled() &&
                              editor.AnyCursorHasSelection()))
        editor.Cut();
      if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr,
                          editor.AnyCursorHasSelection()))
        editor.Copy();
      if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr,
                          !editor.IsReadOnlyEnabled()))
        editor.Paste();
      ImGui::Separator();
      if (ImGui::MenuItem("Select All", "Ctrl+A"))
        editor.SelectAll();
      if (ImGui::MenuItem("Toggle Comments", "Ctrl+/", nullptr,
                          !editor.IsReadOnlyEnabled()))
        editor.ToggleComments();
      ImGui::Separator();
      if (ImGui::MenuItem("Find/Replace...", "Ctrl+F"))
        editor.OpenFindReplaceWindow();
      if (ImGui::MenuItem("Compare with Game Cache", nullptr, false,
                          !IsCachedView))
        RequestDiff();
      ImGui::Separator();
      // --- GO TO DEFINITION ---
      std::string selection = GetSelectedTextManual();
      std::string word = GetWordAt(line, col);
      std::string target = selection.empty() ? word : selection;

      if (!target.empty()) {
        if (ImGui::MenuItem(("Go to Definition: " + target).c_str(), "F12")) {
          if (OnGoToDefRequested)
            OnGoToDefRequested(target);
        }
        ImGui::Separator();
      }

      if (ImGui::MenuItem("Toggle Breakpoint", "F2")) {
        if (OnBreakpointChanged)
          OnBreakpointChanged(RelativePathLower, line, true);
      }
      ImGui::Separator();

      if (!selection.empty()) {
        std::string displayLabel = selection;
        if (displayLabel.size() > 20)
          displayLabel = displayLabel.substr(0, 17) + "...";

        if (ImGui::MenuItem(("Add Watch: " + displayLabel).c_str())) {
          if (OnAddWatchRequested)
            OnAddWatchRequested(selection);
        }
      } else if (!word.empty()) {
        if (ImGui::MenuItem(("Add Watch: " + word).c_str())) {
          if (OnAddWatchRequested)
            OnAddWatchRequested(word);
        }
      } else {
        if (ImGui::MenuItem("Add New Watch...")) {
          if (OnAddWatchRequested)
            OnAddWatchRequested("");
        }
      }
    });

    editor.SetLineNumberContextMenuCallback([this](int line) {
      if (ImGui::MenuItem("Toggle Breakpoint", "F2")) {
        if (OnBreakpointChanged)
          OnBreakpointChanged(RelativePathLower, line, true);
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Copy Line")) {
        editor.SelectLine(line);
        editor.Copy();
        editor.ClearCursors();
      }
    });
  }

  void DrawContent(const std::set<int> &activeBreakpoints,
                   const std::set<int> &errorLines) {
    if (IsDiffOpen) {
      if (ImGui::Button("<- Back to Editor"))
        IsDiffOpen = false;
      ImGui::SameLine();
      ImGui::Text("Diff View: %s", FileName.c_str());
      ImGui::SameLine(ImGui::GetWindowWidth() - 300);
      bool sideBySide = diff.GetSideBySideMode();
      if (ImGui::Checkbox("Side-by-Side", &sideBySide))
        diff.SetSideBySideMode(sideBySide);
      ImGui::SameLine();
      // it doesn't really work correctly...
      // if (ImGui::Checkbox("Ignore Whitespaces", &IgnoreWhitespacesInDiff))
      //   RefreshDiff();
      ImGui::Separator();
      diff.Render("DiffView", ImGui::GetContentRegionAvail(), true);
    } else {
      RefreshMarkers(activeBreakpoints, errorLines);
      HandleInputs();

      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, editor.CanUndo()))
            editor.Undo();
          if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, editor.CanRedo()))
            editor.Redo();
          ImGui::Separator();
          if (ImGui::MenuItem("Find/Replace", "Ctrl+F"))
            editor.OpenFindReplaceWindow();
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
          bool val = editor.IsShowLineNumbersEnabled();
          if (ImGui::MenuItem("Show Line Numbers", nullptr, &val))
            editor.SetShowLineNumbersEnabled(val);
          val = editor.IsShowScrollbarMiniMapEnabled();
          if (ImGui::MenuItem("Show MiniMap", nullptr, &val))
            editor.SetShowScrollbarMiniMapEnabled(val);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      if (IsGotoLineOpen) {
        ImGui::OpenPopup("Go To Line");
        IsGotoLineOpen = false;
      }
      if (ImGui::BeginPopupModal("Go To Line", NULL,
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputInt("Line", &GotoLineNumber);
        if (ImGui::Button("Go") || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
          JumpToLine(GotoLineNumber - 1);
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
          ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
      }

      float statusBarHeight = ImGui::GetFrameHeight();
      ImGui::BeginChild("EditorRegion", ImVec2(0, -statusBarHeight));
      editor.Render("TextEditor");
      ImGui::EndChild();

      int line, col;
      editor.GetCurrentCursor(line, col);
      ImGui::TextDisabled(
          "Ln %d, Col %d | %s", line + 1, col + 1,
          IsCachedView ? "RO" : (editor.IsOverwriteEnabled() ? "OVR" : "INS"));

      if (RequestSmartIndent) {
        editor.ReplaceTextInCurrentCursor("\t");
        RequestSmartIndent = false;
      }
    }
  }
};

class ScriptEditorWindow {
  std::vector<std::shared_ptr<ScriptDocument>> Documents;
  std::shared_ptr<ScriptDocument> ActiveDocument = nullptr;
  TextEditor::Language customLuaLang;

  bool AutoLintOnSave = false;

  std::map<std::string, std::set<int>> GlobalBreakpoints;
  std::map<std::string, std::set<int>> GlobalErrors;
  std::shared_ptr<ScriptDocument> PendingCloseDoc = nullptr;

  bool AutoReloadOnSave = true;

  std::string PausedScriptPath = "";
  int PausedLine = -1;

  std::function<void(std::string)> OnDiffRequested;
  std::function<void(std::string, int, bool)> OnBreakpointChanged;
  std::function<void(std::string)> OnReloadRequested;
  std::function<void(std::string)> OnAddWatchRequested;
  std::function<void()> OnScriptListChanged;

  AsyncLuaLinter linter;
  GlobalsManager globalsManager;
  bool RealTimeLinting = true;

  int NewFileCounter = 1;

  bool OpenAboutModal = false;

  void HandleGlobalInputs() {
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
      return;

    ImGuiIO &io = ImGui::GetIO();

    // Ctrl + N : New File
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N)) {
      NewFile();
    }

    // Ctrl + Shift + S : Save As
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_S)) {
      TriggerSaveAs();
    }
  }

  // IMPORTANT i had to blacklist console.lua, because it causes the game to
  // crash on reload (possibly because of line lengths?)
  std::vector<std::string> ScriptBlacklist = {"scripts/hud/console.lua"};

  fs::path ResolveCaseInsensitivePath(const std::string &inputPath) {
    fs::path finalPath = fs::current_path();
    fs::path input(inputPath);
    for (const auto &part : input) {
      if (part == "." || part == "/")
        continue;
      bool found = false;
      if (fs::exists(finalPath)) {
        for (const auto &entry : fs::directory_iterator(finalPath)) {
          std::string entryName = entry.path().filename().string();
          std::string partName = part.string();
          if (entryName.size() == partName.size()) {
            bool match = true;
            for (size_t i = 0; i < entryName.size(); ++i) {
              if (std::tolower(entryName[i]) != std::tolower(partName[i])) {
                match = false;
                break;
              }
            }
            if (match) {
              finalPath /= entryName;
              found = true;
              break;
            }
          }
        }
      }
      if (!found)
        finalPath /= part;
    }
    return finalPath;
  }

public:
  std::vector<LintResult> GlobalLintResults;
  bool IsGlobalAnalysisInProgress = false;
  ScriptEditorWindow() {
    customLuaLang = *TextEditor::Language::Lua();
    customLuaLang.name = "Lua (SecondAID)";
    static const std::vector<std::string> keywords = {
        // TODO do we need any custom keywords apart from default lua ones?
    };
    for (const auto &k : keywords)
      customLuaLang.keywords.insert(k);

    fs::path root = fs::current_path() / "Scripts";
    if (!fs::exists(root))
      root = fs::current_path() / "scripts";
    globalsManager.SetRoot(root);

    // TODO this could be done asynchronically
    globalsManager.ScanAll();
    globalsManager.GenerateConfig();

    fs::path configPath = root / ".luacheckrc";
    linter.SetConfigPath(configPath.string());
  }

  void SetDiffRequestCallback(std::function<void(std::string)> cb) {
    OnDiffRequested = cb;
  }
  void
  SetBreakpointChangeCallback(std::function<void(std::string, int, bool)> cb) {
    OnBreakpointChanged = cb;
  }
  void SetReloadRequestCallback(std::function<void(std::string)> cb) {
    OnReloadRequested = cb;
  }
  void SetAddWatchCallback(std::function<void(std::string)> cb) {
    OnAddWatchRequested = cb;
    for (auto &doc : Documents) {
      if (doc) {
        doc->OnAddWatchRequested = cb;
      }
    }
  }
  void SetOnScriptListChangedCallback(std::function<void()> cb) {
    OnScriptListChanged = cb;
  }
  void RunGlobalAnalysis() {
    fs::path root = fs::current_path() / "Scripts";
    if (!fs::exists(root))
      root = fs::current_path() / "scripts";

    if (fs::exists(root)) {
      IsGlobalAnalysisInProgress = true;
      linter.RequestFolderScan(root.string());
    }
  }

  void UpdateLinting() {
    std::string reqID;
    std::vector<LintResult> resData;
    bool isFolder = false;

    if (linter.GetResult(reqID, resData, isFolder)) {

      if (isFolder) {
        GlobalLintResults = resData;
        IsGlobalAnalysisInProgress = false;
      } else {
        for (auto &doc : Documents) {
          if (doc->FullPath == reqID) {
            doc->LintWarnings = resData;
            const auto &bps = GlobalBreakpoints[doc->RelativePathLower];
            const auto &errs = GlobalErrors[doc->RelativePathLower];
            doc->RefreshMarkers(bps, errs);
            break;
          }
        }
      }
    }
    IsGlobalAnalysisInProgress = linter.IsScanning() && isFolder;

    if (RealTimeLinting && ActiveDocument) {
      int currentUndo = ActiveDocument->editor.GetUndoIndex();
      if (currentUndo != ActiveDocument->LastLintedUndoIndex) {
        if (!ActiveDocument->NeedsLinting) {
          ActiveDocument->MarkDirtyForLint();
        }
      }
    }

    for (auto &doc : Documents) {
      int currentUndo = doc->editor.GetUndoIndex();

      if (currentUndo != doc->KnownUndoIndex) {
        doc->KnownUndoIndex = currentUndo;
        doc->LastEditTime = std::chrono::steady_clock::now();
        doc->NeedsLinting = true;
      }

      if (doc->NeedsLinting) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now - doc->LastEditTime)
                           .count();

        if (elapsed > 500) {
          linter.RequestLint(doc->FullPath, doc->editor.GetText());

          doc->LastLintedUndoIndex = currentUndo;
          doc->NeedsLinting = false;
        }
      }
    }
  }

  std::shared_ptr<ScriptDocument> GetActiveDocument() { return ActiveDocument; }
  void ForceLintActive() {
    if (ActiveDocument)
      ActiveDocument->MarkDirtyForLint();
  }

  std::map<std::string, std::set<int>> &GetGlobalBreakpoints() {
    return GlobalBreakpoints;
  }

  void MarkErrorLine(const std::string &scriptPath, int line) {
    std::string lowerPath = scriptPath;
    std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(),
                   ::tolower);
    std::replace(lowerPath.begin(), lowerPath.end(), '\\', '/');
    if (lowerPath.find("scripts/") != 0 &&
        lowerPath.find("scripts") != std::string::npos) {
      // TODO Ensure proper prefix logic if needed
    }

    GlobalErrors[lowerPath].insert(line);
  }

  void ClearAllErrors() { GlobalErrors.clear(); }

  void ReloadAllScriptsFromDisk() {
    if (!OnReloadRequested)
      return;

    fs::path root = fs::current_path() / "Scripts";
    if (!fs::exists(root))
      root = fs::current_path() / "scripts";

    if (!fs::exists(root))
      return;

    try {
      for (const auto &entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_regular_file() && entry.path().extension() == ".lua") {
          std::string relPath =
              fs::relative(entry.path(), fs::current_path()).string();
          std::transform(relPath.begin(), relPath.end(), relPath.begin(),
                         ::tolower);
          std::replace(relPath.begin(), relPath.end(), '\\', '/');
          if (relPath.find("scripts/") != 0)
            relPath = "scripts/" + relPath;
          bool blacklisted = false;
          for (const auto &blocked : ScriptBlacklist) {
            if (relPath == blocked) {
              blacklisted = true;
              break;
            }
          }
          if (blacklisted) {
            continue;
          }
          OnReloadRequested(relPath);
        }
      }
    } catch (...) {
    }
  }

  void SetPausedState(const std::string &scriptPath, int line) {
    PausedScriptPath = scriptPath;
    PausedLine = line;

    for (auto &doc : Documents) {
      if (doc->RelativePathLower == PausedScriptPath) {
        doc->SetExecutionLine(PausedLine);
        ActiveDocument = doc;
        doc->BringToFront = true;
      } else {
        doc->ClearExecutionLine();
      }
    }
  }

  void ClearPausedState() {
    PausedScriptPath = "";
    PausedLine = -1;
    ClearExecutionLine();
  }

  void LoadFile(const std::string &relPathInput) {
    std::string lowerInput = relPathInput;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                   ::tolower);
    std::replace(lowerInput.begin(), lowerInput.end(), '\\', '/');

    std::string tabKey = lowerInput;
    if (tabKey.find("scripts/") != 0)
      tabKey = "scripts/" + tabKey;

    for (auto &doc : Documents) {
      if (doc->RelativePathLower == tabKey) {
        ActiveDocument = doc;
        doc->BringToFront = true;
        if (doc->RelativePathLower == PausedScriptPath)
          doc->SetExecutionLine(PausedLine);
        return;
      }
    }

    std::string rawPath = relPathInput;
    if (lowerInput.find("scripts/") == 0)
      rawPath = relPathInput.substr(8);

    fs::path resolvedPath;
    bool found = false;
    resolvedPath = ResolveCaseInsensitivePath("Scripts/" + rawPath);
    if (fs::exists(resolvedPath))
      found = true;
    if (!found) {
      resolvedPath = ResolveCaseInsensitivePath("scripts/" + rawPath);
      if (fs::exists(resolvedPath))
        found = true;
    }
    if (!found) {
      resolvedPath = ResolveCaseInsensitivePath(rawPath);
      if (fs::exists(resolvedPath))
        found = true;
    }
    if (!found)
      resolvedPath = ResolveCaseInsensitivePath(relPathInput);

    auto newDoc = std::make_shared<ScriptDocument>(
        resolvedPath.string(), relPathInput, &customLuaLang);
    newDoc->OnDiffRequested = OnDiffRequested;
    newDoc->OnGoToDefRequested = [this](std::string symbol) {
      GoToDefinition(symbol);
    };
    newDoc->OnAddWatchRequested = OnAddWatchRequested;
    newDoc->OnBreakpointChanged = [this](std::string f, int l,
                                         bool forceCheck) {
      bool exists = GlobalBreakpoints[f].count(l) > 0;
      bool shouldAdd = !exists;
      if (shouldAdd)
        GlobalBreakpoints[f].insert(l);
      else
        GlobalBreakpoints[f].erase(l);
      if (OnBreakpointChanged)
        OnBreakpointChanged(f, l, shouldAdd);
    };

    if (newDoc->RelativePathLower == PausedScriptPath && PausedLine != -1) {
      newDoc->SetExecutionLine(PausedLine);
    }

    newDoc->BringToFront = true;
    Documents.push_back(newDoc);
    ActiveDocument = newDoc;
  }

  void OpenDiffView(const std::string &cachedContent) {
    if (ActiveDocument)
      ActiveDocument->OpenDiffView(cachedContent);
  }

  void JumpToLine(int line) {
    if (ActiveDocument)
      ActiveDocument->JumpToLine(line);
  }

  void ClearExecutionLine() {
    for (auto &doc : Documents)
      doc->ClearExecutionLine();
  }

  void SaveDocument(std::shared_ptr<ScriptDocument> doc) {
    if (!doc)
      return;

    if (doc->FullPath.empty()) {
      ActiveDocument = doc;
      TriggerSaveAs();
      return;
    }

    if (doc->Save()) {
      globalsManager.ScanFile(doc->FullPath);
      globalsManager.GenerateConfig();

      if (AutoReloadOnSave && OnReloadRequested) {
        OnReloadRequested(doc->RelativePathLower);
      }
      if (AutoLintOnSave) {
        doc->MarkDirtyForLint();
      }
    }
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    UpdateLinting();
    HandleGlobalInputs();
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar)) {
      ImGui::End();
      return;
    }

    HandleFileDialog();

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "Ctrl+N")) {
          NewFile();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
          SaveDocument(ActiveDocument);
        }
        if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
          TriggerSaveAs();
        }
        if (ImGui::MenuItem("Compare with Game Cache", nullptr, false,
                            ActiveDocument && !ActiveDocument->IsCachedView)) {
          if (ActiveDocument)
            ActiveDocument->RequestDiff();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Auto-Reload on Save", nullptr,
                            &AutoReloadOnSave)) {
        }
        if (ImGui::MenuItem("Auto-Lint (Luacheck) on Save", nullptr,
                            &AutoLintOnSave)) {
        }
        ImGui::MenuItem("Real-time Analysis", nullptr, &RealTimeLinting);
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Scripts")) {
        if (ImGui::MenuItem("Reload Current Script")) {
          if (ActiveDocument && OnReloadRequested)
            OnReloadRequested(ActiveDocument->RelativePathLower);
        }
        if (ImGui::MenuItem("Reload All Open Scripts")) {
          if (OnReloadRequested) {
            for (auto &doc : Documents)
              OnReloadRequested(doc->RelativePathLower);
          }
        }
        if (ImGui::MenuItem("Reload ALL Scripts (from Disk)")) {
          ReloadAllScriptsFromDisk();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Check Syntax (Luacheck)", "F7")) {
          if (ActiveDocument)
            ActiveDocument->RunLinter();
        }
        if (ImGui::MenuItem("Check Syntax All Open Scripts")) {
          for (auto &doc : Documents)
            doc->RunLinter();
        }
        if (ImGui::MenuItem("Clear All Lint Markers")) {
          for (auto &doc : Documents)
            doc->ClearLinter();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About SecondAID")) {
          OpenAboutModal = true;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    bool modalOpen = true;
    if (PendingCloseDoc) {
      ImGui::OpenPopup("Unsaved Changes?");
      ImVec2 center = ImGui::GetMainViewport()->GetCenter();
      ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

      if (ImGui::BeginPopupModal("Unsaved Changes?", &modalOpen,
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("File '%s' has unsaved changes.\nDo you want to save them "
                    "before closing?",
                    PendingCloseDoc->FileName.c_str());
        ImGui::Separator();

        if (ImGui::Button("Save & Close", ImVec2(120, 0))) {
          SaveDocument(PendingCloseDoc);
          auto it =
              std::find(Documents.begin(), Documents.end(), PendingCloseDoc);
          if (it != Documents.end())
            Documents.erase(it);
          PendingCloseDoc = nullptr;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Discard", ImVec2(120, 0))) {
          auto it =
              std::find(Documents.begin(), Documents.end(), PendingCloseDoc);
          if (it != Documents.end())
            Documents.erase(it);
          PendingCloseDoc = nullptr;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
          PendingCloseDoc = nullptr;
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
    }

    if (ImGui::BeginTabBar("ScriptTabs",
                           ImGuiTabBarFlags_Reorderable |
                               ImGuiTabBarFlags_AutoSelectNewTabs)) {
      auto docsCopy = Documents;
      for (auto &doc : docsCopy) {
        bool isOpen = true;
        ImGuiTabItemFlags flags = 0;

        if (doc->BringToFront) {
          flags |= ImGuiTabItemFlags_SetSelected;
          doc->BringToFront = false;
        }

        std::string tabName = doc->FileName + (doc->IsDirty() ? " *" : "") +
                              (doc->IsCachedView ? " [CACHE]" : "");
        std::string tabId =
            tabName + "###" + std::to_string((uintptr_t)doc.get());

        if (ActiveDocument == doc && ImGui::GetIO().KeyCtrl &&
            ImGui::IsKeyPressed(ImGuiKey_S)) {
          SaveDocument(doc);
        }

        if (ImGui::IsKeyPressed(ImGuiKey_F7) && ActiveDocument) {
          ActiveDocument->RunLinter();
        }

        if (ImGui::BeginTabItem(tabId.c_str(), &isOpen, flags)) {
          ActiveDocument = doc;
          const auto &bps = GlobalBreakpoints[doc->RelativePathLower];
          const auto &errs = GlobalErrors[doc->RelativePathLower];
          doc->DrawContent(bps, errs);
          ImGui::EndTabItem();
        }

        if (!isOpen) {
          if (doc->IsDirty()) {
            PendingCloseDoc = doc;
          } else {
            auto it = std::find(Documents.begin(), Documents.end(), doc);
            if (it != Documents.end()) {
              Documents.erase(it);
              if (ActiveDocument == doc)
                ActiveDocument = Documents.empty() ? nullptr : Documents.back();
            }
          }
        }
      }
      ImGui::EndTabBar();
    }
    if (Documents.empty())
      ImGui::TextDisabled("No scripts open.");
    DrawAboutModal();
    ImGui::End();
  }
  void GoToDefinition(const std::string &symbol) {
    if (symbol.empty())
      return;

    DefinitionLocation loc;
    if (globalsManager.FindDefinition(symbol, loc)) {
      LoadFile(loc.FilePath);
      JumpToLine(loc.Line - 1);

    } else {
      if (ActiveDocument) {
        // TODO do stuff otherwise
      }
    }
  }

  void NewFile() {
    std::string name = "Untitled " + std::to_string(NewFileCounter++) + ".lua";
    auto newDoc = std::make_shared<ScriptDocument>(name, &customLuaLang);

    newDoc->OnDiffRequested = OnDiffRequested;
    newDoc->OnBreakpointChanged = [this](std::string f, int l,
                                         bool forceCheck) {
      if (f.find("untitled") != std::string::npos)
        return;

      bool exists = GlobalBreakpoints[f].count(l) > 0;
      bool shouldAdd = !exists;
      if (shouldAdd)
        GlobalBreakpoints[f].insert(l);
      else
        GlobalBreakpoints[f].erase(l);
      if (OnBreakpointChanged)
        OnBreakpointChanged(f, l, shouldAdd);
    };
    newDoc->OnAddWatchRequested = OnAddWatchRequested;
    newDoc->OnGoToDefRequested = [this](std::string s) { GoToDefinition(s); };

    newDoc->BringToFront = true;
    Documents.push_back(newDoc);
    ActiveDocument = newDoc;
  }
  void TriggerSaveAs() {
    if (!ActiveDocument)
      return;

    IGFD::FileDialogConfig config;
    config.path = ".";
    if (fs::exists("Scripts"))
      config.path = "./Scripts";

    ImGuiFileDialog::Instance()->OpenDialog("SaveAsDlg", "Save Script As...",
                                            ".lua", config);
  }

  void HandleFileDialog() {
    ImVec2 minSize(700, 450);

    if (ImGuiFileDialog::Instance()->Display(
            "SaveAsDlg", ImGuiWindowFlags_NoCollapse, minSize)) {
      if (ImGuiFileDialog::Instance()->IsOk()) {
        std::string filePathName =
            ImGuiFileDialog::Instance()->GetFilePathName();
        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

        fs::path pAbs = fs::absolute(filePathName);
        fs::path pRoot = fs::absolute("Scripts");

        std::string sAbs = pAbs.string();
        std::string sRoot = pRoot.string();

        std::string relPath = fs::relative(pAbs, fs::current_path()).string();
        std::replace(relPath.begin(), relPath.end(), '\\', '/');

        bool isSafe =
            (relPath.find("Scripts/") == 0 || relPath.find("scripts/") == 0);

        if (isSafe && ActiveDocument) {
          ActiveDocument->SetPathAfterSaveAs(filePathName, relPath);
          SaveDocument(ActiveDocument);

          globalsManager.ScanFile(filePathName);
          globalsManager.GenerateConfig();
          OnScriptListChanged();
        } else {
          // std::cout << "[Editor] Save rejected. File must be inside Scripts
          // folder." << std::endl;
        }
      }
      ImGuiFileDialog::Instance()->Close();
    }
  }

  void DrawAboutModal() {
    if (OpenAboutModal) {
      ImGui::OpenPopup("About SecondAID");
      OpenAboutModal = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("About SecondAID", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {

      ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "SecondAID");
      ImGui::Text("Author: pawelktk");
      ImGui::Separator();
      ImGui::Spacing();

      if (ImGui::BeginTabBar("AboutTabs")) {

        if (ImGui::BeginTabItem("License (GPLv3)")) {
          ImGui::BeginChild("GPLText", ImVec2(500, 300), true);
          ImGui::TextWrapped(
              "This program is free software: you can redistribute it and/or "
              "modify "
              "it under the terms of the GNU General Public License as "
              "published by "
              "the Free Software Foundation, either version 3 of the License, "
              "or "
              "(at your option) any later version.\n\n"
              "This program is distributed in the hope that it will be useful, "
              "but WITHOUT ANY WARRANTY; without even the implied warranty of "
              "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
              "GNU General Public License for more details.\n\n"
              "You should have received a copy of the GNU General Public "
              "License "
              "along with this program.  If not, see "
              "<https://www.gnu.org/licenses/>.");
          ImGui::EndChild();
          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Third Party Components")) {
          ImGui::BeginChild("ThirdPartyText", ImVec2(500, 300), true);

          auto TextMIT = [](const char *libName, const char *author) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.9f, 0.5f, 1));
            ImGui::Text("%s", libName);
            ImGui::PopStyleColor();
            ImGui::Text("Copyright (c) %s", author);
            ImGui::TextDisabled("Licensed under the MIT License.");
            ImGui::Separator();
          };

          TextMIT("Dear ImGui", "Omar Cornut");
          TextMIT("ImGuiColorTextEdit", "Johan A. Goossens");
          TextMIT("ImGuiFileDialog", "Stephane Cuillerdier (Aiekick)");

          ImGui::Spacing();
          ImGui::TextWrapped(R"(
The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
)");
          ImGui::EndChild();
          ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
      }

      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      float width = ImGui::GetContentRegionAvail().x;
      ImGui::SetCursorPosX((width - 120) * 0.5f);
      if (ImGui::Button("Close", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
  }
};
