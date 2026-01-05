#pragma once
#include "imgui.h"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

struct CommandDefinition {
  std::string name;
  std::string description;
  std::string usage;
};

struct ConsoleLogEntry {
  std::string msg;
  int32_t colorCode;
  bool isCommand;
};

class ConsoleWindow {
  char InputBuf[256];
  std::vector<std::string> History;
  int HistoryPos = -1;

  std::vector<ConsoleLogEntry> Items;
  std::set<int> SelectedIndices;
  bool AutoScroll = true;
  bool ScrollToBottom = false;

  std::vector<CommandDefinition> Commands;
  std::vector<int> Candidates;
  int CandidatePos = 0;
  bool IsPopupOpen = false;

  std::function<void(std::string)> SendCommandCallback;

  ImVec4 GetColorForCode(int32_t col) const {
    if (col == 0)
      return ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    int r = (col >> 16) & 0xFF;
    int g = (col >> 8) & 0xFF;
    int b = col & 0xFF;
    return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
  }

  static int TextEditCallbackStub(ImGuiInputTextCallbackData *data) {
    ConsoleWindow *console = (ConsoleWindow *)data->UserData;
    return console->TextEditCallback(data);
  }

  int TextEditCallback(ImGuiInputTextCallbackData *data) {

    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
      const char *word_end = data->Buf + data->CursorPos;
      const char *word_start = word_end;
      while (word_start > data->Buf) {
        const char c = word_start[-1];
        if (c == ' ' || c == '\t' || c == ',' || c == ';')
          break;
        word_start--;
      }

      std::string fragment = std::string(word_start, word_end - word_start);
      std::string fragmentLower = fragment;
      std::transform(fragmentLower.begin(), fragmentLower.end(),
                     fragmentLower.begin(), ::tolower);

      Candidates.clear();
      for (int i = 0; i < Commands.size(); i++) {
        std::string cmdLower = Commands[i].name;
        std::transform(cmdLower.begin(), cmdLower.end(), cmdLower.begin(),
                       ::tolower);
        if (cmdLower.find(fragmentLower) == 0)
          Candidates.push_back(i);
      }

      if (Candidates.empty()) {
      } else if (Candidates.size() == 1) {
        ReplaceWord(data, word_start - data->Buf, word_end - data->Buf,
                    Commands[Candidates[0]].name);
        Candidates.clear();
      } else {
      }
    }

    else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
      const int prev_history_pos = HistoryPos;
      if (data->EventKey == ImGuiKey_UpArrow) {
        if (HistoryPos == -1)
          HistoryPos = History.size() - 1;
        else if (HistoryPos > 0)
          HistoryPos--;
      } else if (data->EventKey == ImGuiKey_DownArrow) {
        if (HistoryPos != -1)
          if (++HistoryPos >= History.size())
            HistoryPos = -1;
      }

      if (prev_history_pos != HistoryPos) {
        const char *history_str =
            (HistoryPos >= 0) ? History[HistoryPos].c_str() : "";
        data->DeleteChars(0, data->BufTextLen);
        data->InsertChars(0, history_str);
      }
    } else if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways) {
    }

    return 0;
  }

  void ReplaceWord(ImGuiInputTextCallbackData *data, int start_pos, int end_pos,
                   const std::string &replacement) {
    data->DeleteChars(start_pos, end_pos - start_pos);
    data->InsertChars(start_pos, replacement.c_str());
    data->InsertChars(data->CursorPos, " ");
  }

  void ExecCommand(const std::string &cmd_line) {
    AddLog("# " + cmd_line, 0x00AAAAAA, true);
    HistoryPos = -1;
    if (History.empty() || History.back() != cmd_line) {
      History.push_back(cmd_line);
    }
    if (SendCommandCallback)
      SendCommandCallback(cmd_line);
    ScrollToBottom = true;
  }

  void InitCommands() {
#define CMD(n, u, d) Commands.push_back({n, d, u})

    CMD("help", "", "list all current registered commands");
    CMD("refresh_panel", "", "refresh the hud");
    CMD("SetNeedValue", "[Need ID, Value]", "Sets the selected need value");
    CMD("ShowNeeds", "", "Shows all need values of the selected object(s)");
    CMD("SetFavor", "[Favor]",
        "Set the selected sims favor for the currently active player char");
    CMD("ShowOptions", "", "Show the currently set options of the game");
    CMD("info", "id(integer)",
        "display's info about a registered guild-object");
    CMD("list", "[ classname(string) ]", "display's all objects of an class");
    CMD("find", "text(string)", "Search for a class, with exact name 'text'");
    CMD("search", "text(string)",
        "Search for all classes, that have 'text' as a part of their name");
    CMD("show", "id(integer)", "Show's an cl_SimObject in the game camera");
    CMD("select", "", "Displays all current selected object from the hud");
    CMD("item", "id(integer) or name(string)",
        "Displays a few info's of an item");
    CMD("stock", "[StockId(integer)]",
        "Display's a few info's over an inventory of an guild object");
    CMD("stockadd", "Item Count [Type] [Id]", "Add's items to an inventory");
    CMD("stockclear", "[StockId(integer)]",
        "Flushes an stock of an guild object");
    CMD("schedule", "id(integer)",
        "List's all scheduled events of the object id");
    CMD("savecheck", "[*]classname", "checks the save/load a class");
    CMD("classlist", "[classname]", "list's all registered classed in game");
    CMD("beam", "xpos zpos", "Beam's the current selected sim(s) to position");
    CMD("hp", "[value]", "Show's or set hp from all selected objects");
    CMD("setprogress", "[value]", "Show's or set the court progress");
    CMD("production", "", "Show's the production list for the ai");
    CMD("evadd", "Type Actor Victim", "Add a memory event to selected object");
    CMD("penalty", "name [Value]", "Adds a penalty to a sim");
    CMD("state", "name value", "Set's a state (0=false, other=true)");
    CMD("memory", "", "Show's a statistic about all current memories");
    CMD("impact", "name value timer",
        "add's an impact to the current selected object");
    CMD("dynimpact", "dynasty name value timer",
        "add's an impact to the dynasty impact list");
    CMD("off", "[Level Index]", "shows info about an office tree or an office");
    CMD("offtake", "Level Index",
        "take's an office for the current selected sim");
    CMD("battleinfo", "", "shows some info of an ongoing battle");
    CMD("battlestat", "[integer]",
        "shows some statistics of an ongoing battle");
    CMD("behavior", "[string]", "shows or sets the standard behavior of a sim");
    CMD("title", "[integer]", "sets or gets the nobility title of a sim");
    CMD("campos", "x y", "sets the camera to a position");
    CMD("camrot", "angle", "sets the camera to a rotation");
    CMD("marry", "[id]", "let the sim marry a random sim or the sim with id");
    CMD("getchild", "", "let the selected, married sim get a child");
    CMD("setability", "", "sets the given ability for the selected sim(s)");
    CMD("seed", "number",
        "seed the resource type 'number' on that base resource");
    CMD("set age", "", "sets the age of the selected sim");
    CMD("xp", "number", "gives and uses the xp to the selected character");
    CMD("updateseg", "", "updateseg");
    CMD("max", "", "maximizes the current selected building");
    CMD("cityinfo", "", "gets additional info about the current visible city");
    CMD("showfamilytree", "[sim]", "shows the family tree");
    CMD("sizeof", "[*]classname", "outputs the size of a class");
    CMD("money", "value", "Changes the money of the selected object");

    std::sort(Commands.begin(), Commands.end(),
              [](const CommandDefinition &a, const CommandDefinition &b) {
                return a.name < b.name;
              });
  }

public:
  ConsoleWindow() {
    memset(InputBuf, 0, sizeof(InputBuf));
    InitCommands();
  }

  void SetSendCommandCallback(std::function<void(std::string)> cb) {
    SendCommandCallback = cb;
  }

  void ClearLog() {
    Items.clear();
    SelectedIndices.clear();
  }

  void AddLog(const std::string &msg, int32_t colorCode, bool isCmd = false) {
    Items.push_back({msg, colorCode, isCmd});
    if (AutoScroll)
      ScrollToBottom = true;
  }

  void Draw(const char *title, bool *p_open = nullptr) {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    const float footer_height_to_reserve =
        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

    bool child_visible = ImGui::BeginChild(
        "ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
        ImGuiWindowFlags_HorizontalScrollbar);

    if (child_visible) {
      if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear"))
          ClearLog();
        ImGui::EndPopup();
      }

      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

      ImGuiListClipper clipper;
      clipper.Begin(Items.size());
      while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          const auto &item = Items[i];
          ImVec4 col = GetColorForCode(item.colorCode);
          if (item.isCommand)
            col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

          ImGui::PushStyleColor(ImGuiCol_Text, col);
          ImGui::PushID(i);

          bool is_selected = SelectedIndices.count(i);
          const char *label = item.msg.empty() ? "##" : item.msg.c_str();

          if (ImGui::Selectable(label, is_selected,
                                ImGuiSelectableFlags_SpanAllColumns)) {
            SelectedIndices.clear();
            SelectedIndices.insert(i);
            if (!item.msg.empty())
              ImGui::SetClipboardText(item.msg.c_str());
          }

          ImGui::PopID();
          ImGui::PopStyleColor();
        }
      }
      clipper.End();
      ImGui::PopStyleVar();

      ImGui::Dummy(ImVec2(0.0f, ImGui::GetStyle().ScrollbarSize + 5.0f));

      if (ScrollToBottom ||
          (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
      ScrollToBottom = false;
    }
    ImGui::EndChild();
    ImGui::Separator();

    bool reclaim_focus = false;
    ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue |
                                      ImGuiInputTextFlags_CallbackHistory |
                                      ImGuiInputTextFlags_CallbackCompletion;

    ImGui::PushItemWidth(-1);

    if (ImGui::InputText("##Input", InputBuf, IM_ARRAYSIZE(InputBuf),
                         input_flags, &TextEditCallbackStub, (void *)this)) {
      std::string s = InputBuf;
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
              }));
      s.erase(std::find_if(s.rbegin(), s.rend(),
                           [](unsigned char ch) { return !std::isspace(ch); })
                  .base(),
              s.end());

      if (!s.empty()) {
        ExecCommand(s);
      }
      strcpy(InputBuf, "");
      reclaim_focus = true;
      Candidates.clear();
    }
    ImGui::PopItemWidth();

    if (ImGui::IsItemActive() || ImGui::IsItemFocused()) {
      std::string inputStr = InputBuf;
      if (!inputStr.empty()) {
        size_t lastSpace = inputStr.find_last_of(" ");
        std::string currentWord = (lastSpace == std::string::npos)
                                      ? inputStr
                                      : inputStr.substr(lastSpace + 1);

        if (!currentWord.empty()) {
          Candidates.clear();
          std::string currentWordLower = currentWord;
          std::transform(currentWordLower.begin(), currentWordLower.end(),
                         currentWordLower.begin(), ::tolower);

          for (int i = 0; i < Commands.size(); i++) {
            std::string cmdLower = Commands[i].name;
            std::transform(cmdLower.begin(), cmdLower.end(), cmdLower.begin(),
                           ::tolower);
            if (cmdLower.find(currentWordLower) == 0) {
              Candidates.push_back(i);
            }
          }
        } else {
          Candidates.clear();
        }
      } else {
        Candidates.clear();
      }
    }

    if (!Candidates.empty()) {
      ImVec2 inputMin = ImGui::GetItemRectMin();
      ImVec2 inputSize = ImGui::GetItemRectSize();
      ImGui::SetNextWindowPos(ImVec2(inputMin.x, inputMin.y - 2.0f),
                              ImGuiCond_Always, ImVec2(0.0f, 1.0f));
      ImGui::SetNextWindowSizeConstraints(ImVec2(inputSize.x, 0),
                                          ImVec2(inputSize.x, 200));

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      if (ImGui::Begin("##Candidates", nullptr,
                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoFocusOnAppearing |
                           ImGuiWindowFlags_AlwaysAutoResize |
                           ImGuiWindowFlags_Tooltip)) {
        for (int idx : Candidates) {
          const auto &cmd = Commands[idx];
          ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%s",
                             cmd.name.c_str());
          if (!cmd.usage.empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s",
                               cmd.usage.c_str());
          }
          ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  %s",
                             cmd.description.c_str());
          if (idx != Candidates.back())
            ImGui::Separator();
        }
        ImGui::End();
      }
      ImGui::PopStyleVar();
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus || ImGui::IsWindowAppearing())
      ImGui::SetKeyboardFocusHere(-1);

    ImGui::End();
  }
};
