#pragma once
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <functional>
#include <set>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// --- VFS STRUCTURES ---
struct CachedFile {
  std::string Name;
  std::string RelPath;
  bool IsVisible = true;
};

struct CachedFolder {
  std::string Name;
  std::string FullPath;
  std::vector<CachedFolder> SubFolders;
  std::vector<CachedFile> Files;
  bool IsVisible = true;
  bool ForceOpen = false;
};

class ScriptExplorer {
  fs::path RootPath;
  std::string SelectedFileRelPath;
  char SearchBuf[128] = "";

  CachedFolder RootFolderCache;

  // --- STATE ---
  std::set<std::string> ExpandedFolders;
  bool WasFiltering = false;
  std::string LastSearchBuf = "";

  std::function<void(std::string)> OnFileSelected;

  bool StringContains(std::string haystack, std::string needle) {
    if (needle.empty())
      return true;
    auto toLower = [](std::string s) {
      std::transform(s.begin(), s.end(), s.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      return s;
    };
    return toLower(haystack).find(toLower(needle)) != std::string::npos;
  }

  void BuildTreeRecursive(const fs::path &path, CachedFolder &outFolder) {
    outFolder.Name = path.filename().string();
    outFolder.FullPath = path.string();

    if (path == RootPath)
      outFolder.Name = "Scripts";

    try {
      std::vector<fs::directory_entry> entries;
      for (const auto &entry : fs::directory_iterator(path)) {
        entries.push_back(entry);
      }

      std::sort(entries.begin(), entries.end(),
                [](const auto &a, const auto &b) {
                  return a.path().filename() < b.path().filename();
                });

      for (const auto &entry : entries) {
        if (entry.is_directory()) {
          CachedFolder sub;
          BuildTreeRecursive(entry.path(), sub);
          outFolder.SubFolders.push_back(sub);
        } else if (entry.is_regular_file() &&
                   entry.path().extension() == ".lua") {
          CachedFile file;
          file.Name = entry.path().filename().string();

          std::string rel = fs::relative(entry.path(), RootPath).string();
          std::replace(rel.begin(), rel.end(), '\\', '/');
          file.RelPath = rel;

          outFolder.Files.push_back(file);
        }
      }
    } catch (...) {
    }
  }

  bool FilterTreeRecursive(CachedFolder &folder, const std::string &filter) {
    bool anyChildVisible = false;
    bool nameMatch = StringContains(folder.Name, filter);

    for (auto &sub : folder.SubFolders) {
      if (FilterTreeRecursive(sub, filter)) {
        anyChildVisible = true;
      }
    }

    for (auto &file : folder.Files) {
      if (StringContains(file.Name, filter)) {
        file.IsVisible = true;
        anyChildVisible = true;
      } else {
        file.IsVisible = false;
      }
    }

    if (filter.empty()) {
      folder.IsVisible = true;
      folder.ForceOpen = false;
      for (auto &f : folder.Files)
        f.IsVisible = true;
      return true;
    }

    folder.IsVisible = nameMatch || anyChildVisible;
    folder.ForceOpen = anyChildVisible;

    if (nameMatch) {
      for (auto &f : folder.Files)
        f.IsVisible = true;
    }

    return folder.IsVisible;
  }

public:
  ScriptExplorer(const std::string &root = "Scripts") {
    RootPath = fs::absolute(root);
    if (!fs::exists(RootPath)) {
      fs::path lowerPath = fs::absolute("scripts");
      if (fs::exists(lowerPath))
        RootPath = lowerPath;
    }
    Refresh();
  }

  void Refresh() {
    if (!fs::exists(RootPath))
      return;
    RootFolderCache = CachedFolder();
    RootFolderCache.Files.clear();
    RootFolderCache.SubFolders.clear();

    BuildTreeRecursive(RootPath, RootFolderCache);

    std::string filter = SearchBuf;
    FilterTreeRecursive(RootFolderCache, filter);
  }

  void SetSelectionCallback(std::function<void(std::string)> cb) {
    OnFileSelected = cb;
  }

  std::string GetSelectedFile() const { return SelectedFileRelPath; }

  void Draw(const char *title, bool *p_open = nullptr) {
    if (!ImGui::Begin(title, p_open)) {
      ImGui::End();
      return;
    }

    if (std::string(SearchBuf) != LastSearchBuf) {
      LastSearchBuf = SearchBuf;
      FilterTreeRecursive(RootFolderCache, LastSearchBuf);
    }

    bool isFiltering = !LastSearchBuf.empty();
    bool isRestorationFrame = (WasFiltering && !isFiltering);
    WasFiltering = isFiltering;

    float footerHeight = ImGui::GetFrameHeightWithSpacing() + 4.0f;

    ImGui::BeginChild("TreeRegion", ImVec2(0, -footerHeight), false,
                      ImGuiWindowFlags_HorizontalScrollbar);
    DrawRecursive(RootFolderCache, isFiltering, isRestorationFrame);
    ImGui::EndChild();

    ImGui::Separator();

    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##Filter", "Filter scripts...", SearchBuf,
                             IM_ARRAYSIZE(SearchBuf));
    ImGui::PopItemWidth();

    ImGui::End();
  }

private:
  void DrawRecursive(const CachedFolder &folder, bool isFiltering,
                     bool isRestorationFrame) {
    for (const auto &sub : folder.SubFolders) {
      if (!sub.IsVisible)
        continue;

      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.5f, 1.0f));

      if (isFiltering && sub.ForceOpen) {
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);
      } else if (isRestorationFrame) {
        bool wasOpen = ExpandedFolders.count(sub.FullPath) > 0;
        ImGui::SetNextItemOpen(wasOpen, ImGuiCond_Always);
      }

      bool node_open =
          ImGui::TreeNode((void *)sub.FullPath.c_str(), "%s", sub.Name.c_str());
      ImGui::PopStyleColor();

      if (!isFiltering) {
        if (node_open)
          ExpandedFolders.insert(sub.FullPath);
        else
          ExpandedFolders.erase(sub.FullPath);
      }

      if (node_open) {
        DrawRecursive(sub, isFiltering, isRestorationFrame);
        ImGui::TreePop();
      }
    }

    for (const auto &file : folder.Files) {
      if (!file.IsVisible)
        continue;

      bool is_selected = (SelectedFileRelPath == file.RelPath);
      if (ImGui::Selectable(file.Name.c_str(), is_selected)) {
        SelectedFileRelPath = file.RelPath;
        if (OnFileSelected) {
          OnFileSelected(file.RelPath);
        }
      }
    }
  }
};
