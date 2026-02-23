#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#define ANSI_RESET "\033[0m"

#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN "\033[36m"

#define ANSI_DIM_GRAY "\033[38;2;96;96;96m"         // 0x00606060
#define ANSI_DARK_SLATE_BLUE "\033[38;2;72;61;139m" // 0x00483d8b
#define ANSI_PURE_RED "\033[38;2;255;0;0m"          // 0x00ff0000
#define ANSI_PURE_BLUE "\033[38;2;0;0;255m"         // 0x000000ff

inline void InitAnsiConsole() {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
    return;
  DWORD dwMode = 0;
  if (GetConsoleMode(hOut, &dwMode)) {
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  }
#endif
}

inline std::string ToAnsiColour(int32_t col) {
  if (col == 0)
    return ANSI_RESET;

  int r = (col >> 16) & 0xFF;
  int g = (col >> 8) & 0xFF;
  int b = col & 0xFF;

  return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" +
         std::to_string(b) + "m";
}
