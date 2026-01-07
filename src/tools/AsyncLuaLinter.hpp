#pragma once
#include <array>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

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

class AsyncLuaLinter {
  struct LintRequest {
    std::string filePath;
    std::string content;
    bool ready = false;
    bool isFolder = false;
  };

  struct LintResponse {
    std::string reqPath;
    std::vector<LintResult> results;
    bool isFolderResult = false;
  };

  std::thread workerThread;
  std::mutex queueMutex;
  std::condition_variable cv;

  std::mutex resultsMutex;
  std::vector<LintResponse> pendingResults;

  LintRequest currentRequest;
  std::atomic<bool> running{true};
  std::atomic<bool> hasWork{false};
  std::atomic<bool> isWorking{false};

  std::string configPath;

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

  void WorkerLoop() {
    while (running) {
      LintRequest req;
      {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this] { return hasWork || !running; });
        if (!running)
          break;
        req = currentRequest;
        hasWork = false;
        currentRequest.ready = false;
        isWorking = true;
      }

      if (!req.ready) {
        isWorking = false;
        continue;
      }

      std::string cmd;
      std::string targetPath = req.filePath;
      bool useTemp = !req.isFolder;

      if (useTemp) {
        targetPath = req.filePath + ".tmp_lint";
        {
          std::ofstream out(targetPath);
          out << req.content;
        }
      }

      cmd = "luacheck \"" + targetPath +
            "\" --no-color --formatter=plain --ranges --codes";

      if (!configPath.empty()) {
        cmd += " --config \"" + configPath + "\"";
      }

      std::string output = ExecuteCommand(cmd.c_str());

      std::vector<LintResult> results;
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

          if (req.isFolder) {
            res.file = match[1];
          } else {
            res.file = req.filePath;
          }

          res.line = std::stoi(match[2]);
          res.startCol = std::stoi(match[3]);
          res.endCol = std::stoi(match[4]);
          res.type = match[5].matched ? match[5].str() : "W";
          res.message = match[6];
          results.push_back(res);
        }
      }

      if (useTemp) {
        try {
          fs::remove(targetPath);
        } catch (...) {
        }
      }

      {
        std::lock_guard<std::mutex> lock(resultsMutex);
        LintResponse resp;
        resp.reqPath = req.filePath;
        resp.results = results;
        resp.isFolderResult = req.isFolder;
        pendingResults.push_back(resp);
      }

      isWorking = false;
    }
  }

public:
  AsyncLuaLinter() {
    workerThread = std::thread(&AsyncLuaLinter::WorkerLoop, this);
  }

  ~AsyncLuaLinter() {
    running = false;
    cv.notify_one();
    if (workerThread.joinable())
      workerThread.join();
  }

  void SetConfigPath(const std::string &path) { configPath = path; }

  void RequestLint(const std::string &filePath, const std::string &content) {
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      currentRequest.filePath = filePath;
      currentRequest.content = content;
      currentRequest.isFolder = false;
      currentRequest.ready = true;
      hasWork = true;
    }
    cv.notify_one();
  }

  void RequestFolderScan(const std::string &folderPath) {
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      currentRequest.filePath = folderPath;
      currentRequest.content = "";
      currentRequest.isFolder = true;
      currentRequest.ready = true;
      hasWork = true;
    }
    cv.notify_one();
  }

  bool IsScanning() const { return isWorking; }

  bool GetResult(std::string &outReqID, std::vector<LintResult> &outResults,
                 bool &outIsFolder) {
    std::lock_guard<std::mutex> lock(resultsMutex);
    if (pendingResults.empty())
      return false;

    auto &item = pendingResults.back();
    outReqID = item.reqPath;
    outResults = item.results;
    outIsFolder = item.isFolderResult;
    pendingResults.clear();
    return true;
  }
};
