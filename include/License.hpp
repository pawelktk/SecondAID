#pragma once

#include <string>
#include "imgui.h"


namespace LicenseText {
    const char* MIT = R"(
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
)";
    const char* GPL3 = "This program is free software: you can redistribute it and/or "
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
              "<https://www.gnu.org/licenses/>.";
}

namespace AboutModal {
    bool OpenAboutModal = false;
    const char * AboutModalName = "About SecondAID";
    auto TextMIT = [](const char *libName, const char *author) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.9f, 0.5f, 1));
            ImGui::Text("%s", libName);
            ImGui::PopStyleColor();
            ImGui::Text("Copyright (c) %s", author);
            ImGui::TextDisabled("Licensed under the MIT License.");
            ImGui::Separator();
    };

    void DrawAboutModal() {
    if (OpenAboutModal) {
      ImGui::OpenPopup(AboutModalName);
      OpenAboutModal = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(AboutModalName, nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {

      ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "SecondAID");
      ImGui::Text("Author: pawelktk");
      ImGui::Separator();
      ImGui::Spacing();

      if (ImGui::BeginTabBar("AboutTabs")) {

        if (ImGui::BeginTabItem("License (GPLv3)")) {
          ImGui::BeginChild("GPLText", ImVec2(500, 300), true);
          ImGui::TextWrapped(LicenseText::GPL3);
          ImGui::EndChild();
          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Third Party Components")) {
          ImGui::BeginChild("ThirdPartyText", ImVec2(500, 300), true);

          TextMIT("Dear ImGui", "Omar Cornut");
          TextMIT("ImGuiColorTextEdit", "Johan A. Goossens");
          TextMIT("ImGuiFileDialog", "Stephane Cuillerdier (Aiekick)");

          ImGui::Spacing();
          ImGui::TextWrapped(LicenseText::MIT);
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
}