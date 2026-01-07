#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

struct LintResult {
  std::string file;
  int line;
  int startCol;
  int endCol;
  std::string type;
  std::string message;
};

struct PipeCloser {
  void operator()(FILE *f) const {
    if (f) {
#ifdef _WIN32
      _pclose(f);
#else
      pclose(f);
#endif
    }
  }
};

class LuaLinter {
public:
  static std::string ExecuteCommand(const char *cmd) {
    std::string result;

#ifdef _WIN32
    // --- WINDOWS (CreateProcess + Pipes) ---
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hChildStd_OUT_Rd = NULL;
    HANDLE hChildStd_OUT_Wr = NULL;

    if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0)) {
      return "";
    }

    SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hChildStd_OUT_Wr;
    si.hStdOutput = hChildStd_OUT_Wr;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    std::string cmdStr = std::string("cmd.exe /C ") + cmd;
    std::vector<char> cmdMutable(cmdStr.begin(), cmdStr.end());
    cmdMutable.push_back(0);

    BOOL bSuccess = CreateProcessA(NULL,
                                   cmdMutable.data(), // command line
                                   NULL, // process security attributes
                                   NULL, // primary thread security attributes
                                   TRUE, // handles are inherited
                                   CREATE_NO_WINDOW, // don't create window
                                   NULL,             // use parent's environment
                                   NULL, // use parent's current directory
                                   &si,  // STARTUPINFO pointer
                                   &pi); // receives PROCESS_INFORMATION

    CloseHandle(hChildStd_OUT_Wr);

    if (bSuccess) {
      DWORD dwRead;
      CHAR chBuf[4096];
      bool bSuccessRead = FALSE;

      while (true) {
        bSuccessRead = ReadFile(hChildStd_OUT_Rd, chBuf, 4096, &dwRead, NULL);
        if (!bSuccessRead || dwRead == 0)
          break;
        result.append(chBuf, dwRead);
      }
      WaitForSingleObject(pi.hProcess, 1000);
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }

    CloseHandle(hChildStd_OUT_Rd);

#else
    // --- LINUX/MACOS (popen) ---
    std::array<char, 256> buffer;
    std::unique_ptr<FILE, PipeCloser> pipe(popen(cmd, "r"));

    if (!pipe) {
      std::cout << "[Linter] Failed to start process!" << std::endl;
      return "";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }
#endif

    return result;
  }

  static std::vector<LintResult> Run(const std::string &scriptPath) {
    std::vector<LintResult> results;

    std::string command = "luacheck \"" + scriptPath +
                          "\" --no-color --formatter=plain --ranges --codes";
    if (!configPath.empty()) {
        command += " --config \"" + configPath + "\"";
    }
    std::string output = ExecuteCommand(command.c_str());

    if (output.empty())
      return results;

    // Format: file:line:col_start-col_end: (Type) Message
    std::regex re(
        R"(^(.+):(\d+):(\d+)-(\d+):\s+(?:(?:\(([WE])\d+\)\s+)|)(.*)$)");
    std::smatch match;

    std::stringstream ss(output);
    std::string line;

    while (std::getline(ss, line)) {
      if (!line.empty() && line.back() == '\r')
        line.pop_back();

      if (std::regex_search(line, match, re)) {
        LintResult res;
        res.file = match[1];
        res.line = std::stoi(match[2]);
        res.startCol = std::stoi(match[3]);
        res.endCol = std::stoi(match[4]);

        if (match[5].matched) {
          res.type = match[5];
        } else {
          res.type = "W";
        }

        res.message = match[6];
        results.push_back(res);
      }
    }

    return results;
  }
};
