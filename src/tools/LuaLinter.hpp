#pragma once
#include <array>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>

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
    std::array<char, 256> buffer;
    std::string result;

#ifdef _WIN32
#define POPEN _popen
#else
#define POPEN popen
#endif

    std::unique_ptr<FILE, PipeCloser> pipe(POPEN(cmd, "r"));

    if (!pipe) {
      std::cout << "[Linter] Failed to start process!" << std::endl;
      return "";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }
    return result;
  }

  static std::vector<LintResult> Run(const std::string &scriptPath) {
    std::vector<LintResult> results;

    std::string command =
        "luacheck \"" + scriptPath + "\" --no-color --formatter=plain --ranges";
    // std::cout << "[Linter] Running: " << command << std::endl;

    std::string output = ExecuteCommand(command.c_str());

    if (output.empty())
      return results;

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
      } else {
      }
    }

    return results;
  }
};
