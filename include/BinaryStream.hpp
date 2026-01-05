#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

class BinaryStream {
private:
  std::vector<uint8_t> buf;

public:
  BinaryStream() = default;
  BinaryStream(const BinaryStream &) = default;
  BinaryStream(BinaryStream &&) = default;
  BinaryStream &operator=(const BinaryStream &) = default;
  BinaryStream &operator=(BinaryStream &&) = default;

  BinaryStream(const void *data, size_t size) {
    if (data != nullptr && size > 0) {
      const uint8_t *ptr = static_cast<const uint8_t *>(data);
      buf.assign(ptr, ptr + size);
    }
  }

  template <typename T, typename... Args,
            typename = std::enable_if_t<
                !std::is_same_v<std::decay_t<T>, BinaryStream>>>
  explicit BinaryStream(T &&first, Args &&...args) {
    *this << std::forward<T>(first);
    ((*this << std::forward<Args>(args)), ...);
  }
  template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
  BinaryStream &operator<<(T value) {
    write(static_cast<int32_t>(value));
    return *this;
  }

  void write(int32_t value) {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
    buf.insert(buf.end(), bytes, bytes + sizeof(value));
  }

  void write(uint32_t value) {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
    buf.insert(buf.end(), bytes, bytes + sizeof(value));
  }

  void write(const char *str) {
    if (str != nullptr) {
      size_t len = std::strlen(str);
      const uint8_t *bytes = reinterpret_cast<const uint8_t *>(str);
      buf.insert(buf.end(), bytes, bytes + len);
      buf.push_back(0);
    }
  }

  void write(const std::string &str) { write(str.c_str()); }

  void write(const std::vector<uint8_t> &otherBuf) {
    buf.insert(buf.end(), otherBuf.begin(), otherBuf.end());
  }

  void write(const BinaryStream &other) {
    buf.insert(buf.end(), other.buf.begin(), other.buf.end());
  }

  BinaryStream &operator<<(int32_t value) {
    write(value);
    return *this;
  }
  BinaryStream &operator<<(uint32_t value) {
    write(value);
    return *this;
  }
  BinaryStream &operator<<(const char *value) {
    write(value);
    return *this;
  }
  BinaryStream &operator<<(const std::string &value) {
    write(value);
    return *this;
  }
  BinaryStream &operator<<(const std::vector<uint8_t> &value) {
    write(value);
    return *this;
  }
  BinaryStream &operator<<(const BinaryStream &value) {
    write(value);
    return *this;
  }

  uint8_t *data() { return buf.data(); }
  const uint8_t *data() const { return buf.data(); }
  size_t size() const { return buf.size(); }
  void clear() { buf.clear(); }
  void reserve(size_t size) { buf.reserve(size); }
};
