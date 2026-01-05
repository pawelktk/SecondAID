# SecondAID
**SecondAID** is a powerful, standalone script editor and real-time debugger for The Guild 2 based on reverse engineering of AID protocol.

<img width="1728" height="971" alt="Przechwycenie obrazu ekranu_2026-01-05_02-44-47" src="https://github.com/user-attachments/assets/53aba74f-795b-4f85-a365-162fdf3e44ca" />


# Features

### Script Editor
* **Multi-Tab Editor:** Open and edit multiple scripts simultaneously.
* **Syntax Highlighting:** Full Lua syntax highlighting.
* **Smart Indentation:** Auto-indentation on new lines based on code blocks (`then`, `do`, `function`, etc.).
* **Visual Decorators:** Breakpoints and execution lines are rendered on the left margin and are fully interactible.
* **Find & Replace:** Standard search and replace functionality (`Ctrl+F`).
* **Diff View:** Compare your local script changes against the version currently loaded in the game's cache.

### Debugger
* **Breakpoints:** Toggle breakpoints via `F2` or by clicking the editor margin.
* **Execution Control:** Pause, Resume, and Step-through logic (including multi-stepping).
* **Watches & History:**
    * **Live Monitoring:** Watch variable values update in real-time.
    * **Contextual History:** Tracks variable changes over time, recording the value, file context, line number, and pause reason (Step vs Breakpoint).
    * **Smart Deduplication:** Collapses sequential duplicate values to keep the history clean.
    * **Quick Add:** Add watches via right-click context menu, text selection, or manual entry.
* **Lua Error Handling:** Dedicated window capturing runtime errors from the game, with one-click navigation to the affected file and line.

### Console & Logging
* **Smart Remote Console:** A powerful replacement for the default AID console.
    * **Auto-Complete:** Context-aware suggestions for commands as you type.
    * **Command History:** Seamlessly navigate through your previously executed commands using **Up/Down arrows**.
* **High-Performance Game Logs:**
    * **Zero-Lag Filtering:** Instant, stutter-free searching and filtering through thousands of log lines.
    * **Performance:** Optimized rendering ensures the UI remains responsive even during heavy log spam (unlike AID which often freezes).
    * **Auto-Scroll:** Toggleable auto-scroll to keep up with the latest events.

### Static Analysis (Linter)
* **Async Architecture:** Linting runs in a background thread to ensure the UI never freezes, providing instant feedback as you type.
* **Tailored for The Guild 2:** Custom-tuned to understand the game's specific Lua dialect and built-in engine bindings (e.g., `SimGet`, `MsgBox`, `MeasureRun`) and globals, eliminating false positives common with generic Lua linter configuration.
* **Smart Global Resolution:** Automatically adapts to the game's unique idiosyncrasies, such as the global namespace behavior where local script functions become globally available via prefixes (e.g., `feud_CalculatePrice`).
* **Dynamic Environment Modeling:** The editor actively scans the game files to build an internal definition model. It dynamically recognizes your custom functions across different files without requiring manual configuration.
* **Project-Wide Analysis:**
    * **Overview Window:** Scans the entire `Scripts/` folder to list all errors and warnings in the project.
    * **Filtering:** Filter results by file path, message content, or show "Errors Only".
* **Visual Feedback:** Warnings and errors are underlined in the editor with detailed tooltips.

### Project Management & Navigation
* **High-Performance Script Explorer:**
    * Uses a Virtual File System to cache file structure, ensuring 60+ FPS even with thousands of script files.
    * **Instant Search:** Real-time filtering of the file tree without I/O lag.
    * **State Restoration:** Remembers expanded folders between filter clears.
* **Go To Definition (`F12`):**
    * Intelligent navigation that understands *The Guild 2* naming conventions (e.g., jumping to `feud_CalculatePrice` opens `AI/Feud.lua`).

### Hot-Reloading
* **Auto-Reload on Save:** Toggleable workflow that instantly reloads cached script whenever you save it.
* **Granular Control:**
    * **Reload Current Script:** Update only the file you are currently working on.
    * **Reload All Open Scripts:** Batch update all tabs currently open in the editor.
    * **Reload ALL Scripts:** Forces a complete reload of all scripts

### UI & UX
* **Docking Layout:** Fully customizable window arrangement (drag & drop tabs/windows).
* **Persistent Settings:** Layout and window positions are saved to `imgui.ini`.

# Installation/Running

## Windows

Just copy SecondAID-GUI.exe and luacheck.exe into the game folder (where Guild2.exe is) and run SecondAID-GUI.exe

## Linux

Luacheck is not included with Linux version of SecondAID, so you have to install it via your distros package manager beforehand, eg. on Arch:
```
  sudo pacman -S luacheck
```
Then just copy SecondAID-GUI_linux into your game folder and run.

# Building

## As Release Bundle

You can use the build_release_bundles.sh script to build both a portable linux executable, and cross-compile a Windows executable, zipped with luacheck. 

It's mainly used for automatically generating primary releases for SecondAID.

## For Linux

```
  mkdir build
  cd build
  cmake ..
```

## For Windows (MinGW64)

```
  mkdir build_win
  cd build_win
  cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-mingw.cmake ..
```
*disclaimer: it works for cross-compiling from linux to windows. It should probably work when compiling Windows->Windows, but i don't have a way to test this*

