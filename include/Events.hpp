#pragma once

#include "AIDPacket.hpp"
#include <cstdint>
#include <string>
class EventGameLog {
public:
  std::string channel;
  std::string msg;
  int32_t colour;

  EventGameLog(std::string channel, std::string msg, int32_t colour = 0)
      : channel(channel), msg(msg), colour(colour) {}
};

class EventLuaError {
public:
  std::string script;
  int line;
  std::string errorMsg;

  EventLuaError() = default;

  EventLuaError(std::string script, int line, std::string errorMsg)
      : script(script), line(line), errorMsg(errorMsg) {}
  // regexes are slow, so we don't use them
  EventLuaError(std::string raw) {
    script = "unknown";
    line = 0;
    errorMsg = raw;
    try {
      // [string "PATH"]:LINE: MSG
      std::string signature = "\"]:";
      size_t endOfScriptSig = raw.find(signature);

      if (endOfScriptSig != std::string::npos) {
        size_t firstQuote = raw.find('"');
        if (firstQuote != std::string::npos && firstQuote < endOfScriptSig) {
          script = raw.substr(firstQuote + 1, endOfScriptSig - firstQuote - 1);
        }
        size_t numStart = endOfScriptSig + signature.length();
        size_t numEndOffset = 0;
        line = std::stoi(raw.substr(numStart), &numEndOffset);
        size_t msgStart = numStart + numEndOffset;
        while (msgStart < raw.length() &&
               (raw[msgStart] == ':' || raw[msgStart] == ' ')) {
          msgStart++;
        }
        errorMsg = raw.substr(msgStart);
      }
    } catch (...) {
    }
  }
};

class EventGotScriptContext {
public:
  std::string rootScript;
  std::string function;
  std::string thisObjectName;
  int thisObjectID;

  EventGotScriptContext() = default;

  EventGotScriptContext(std::string raw) {
    thisObjectID = -1;

    try {
      // 1. script:
      std::string scriptTag = "script: ";
      size_t scriptStart = raw.find(scriptTag);
      size_t scriptEnd = raw.find('\n', scriptStart);
      if (scriptStart != std::string::npos && scriptEnd != std::string::npos) {
        size_t valStart = scriptStart + scriptTag.length();
        rootScript = raw.substr(valStart, scriptEnd - valStart);
        trim(rootScript);
      }

      // 2. func:
      std::string funcTag = "func: ";
      size_t funcStart = raw.find(funcTag, scriptEnd);
      size_t funcEnd = raw.find('\n', funcStart);

      if (funcStart != std::string::npos && funcEnd != std::string::npos) {
        size_t valStart = funcStart + funcTag.length();
        function = raw.substr(valStart, funcEnd - valStart);
        trim(function);
      }

      // 3. this:
      std::string thisTag = "this : ";
      size_t thisStart = raw.find(thisTag, funcEnd);

      if (thisStart != std::string::npos) {
        size_t lastParenClose = raw.rfind(')');
        size_t lastParenOpen = raw.rfind('(', lastParenClose);
        if (lastParenClose != std::string::npos &&
            lastParenOpen != std::string::npos) {
          std::string idStr =
              raw.substr(lastParenOpen + 1, lastParenClose - lastParenOpen - 1);
          try {
            thisObjectID = std::stoi(idStr);
          } catch (...) {
            thisObjectID = -1;
          }
          size_t nameStart = thisStart + thisTag.length();
          if (lastParenOpen > nameStart) {
            thisObjectName = raw.substr(nameStart, lastParenOpen - nameStart);
            trim(thisObjectName);
          }
        }
      }

    } catch (...) {
    }
  }

private:
  void trim(std::string &s) { s.erase(s.find_last_not_of(" \n\r\t") + 1); }
};

enum class ConnectionState {
  CONNECTED,
  GAME_READY,
  GAME_DISCONNECTED,
  CONNECTION_LOST,
  FAILED_BIND_PORT
};

enum class PauseReason { STEP, BREAKPOINT };

// unused
class EventUnimplementedPacket {
public:
  AIDPacketHeader packet;
};
