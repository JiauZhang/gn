// Copyright (c) 2025 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_OUTPUT_STREAM_H_
#define TOOLS_GN_OUTPUT_STREAM_H_

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

// GN generates a lot of text that it sends to various
// output streams. Initially, this was done using std::ostream
// but this interface (and implementation) is inefficient due
// to legacy feature requirements that GN does not need.
//
// OutputStream is an abstract interface for an output stream
// that provides a subset of the std::ostream API, but performs
// far faster. In practice, using it results in 6% faster
// `gn gen` times for large build plans that generate huge
// Ninja build plans.
class OutputStream {
 public:
  virtual ~OutputStream() {}

  // Add |len| bytes of data to the output stream.
  virtual void write(const char* str, size_t len) = 0;

  // Add a single byte of data to the output stream.
  virtual void put(char ch) = 0;

  // Convenience helpers for C literals and standard strings.
  void write(const char* str) { write(str, ::strlen(str)); }
  void write(const std::string& str) { write(str.data(), str.size()); }

  // Operator << overload for std::ostream compatibility.
  OutputStream& operator<<(char ch) {
    put(ch);
    return *this;
  }
  OutputStream& operator<<(const char* str) {
    write(str);
    return *this;
  }
  OutputStream& operator<<(const std::string& str) {
    write(str);
    return *this;
  }
  OutputStream& operator<<(const std::string_view& str) {
    write(str.data(), str.size());
    return *this;
  }

  // Add decimal representations to the output stream.
  OutputStream& operator<<(int value);
  OutputStream& operator<<(long value);
  OutputStream& operator<<(long long value);
  OutputStream& operator<<(unsigned value);
  OutputStream& operator<<(unsigned long value);
  OutputStream& operator<<(unsigned long long value);
};

// A StringOutputStream stores all input into an std::string.
// This is a replacement for std::ostringstream.
class StringOutputStream : public OutputStream {
 public:
  // Constructor creates empty string.
  StringOutputStream() {}

  virtual ~StringOutputStream() {}

  // Retrieve reference to result.
  const std::string str() const { return str_; }

  // Move result out of the instance.
  std::string release() { return std::move(str_); }

  // OutputStream overrides
  void write(const char* str, size_t len) override { str_.append(str, len); }
  void put(char ch) override { str_.push_back(ch); }

 protected:
  std::string str_;
};

// A FileOutputStream writes all input into a file.
class FileOutputStream : public OutputStream {
 public:
  // Constructor opens a FILE instance in binary mode.
  // Use fail() after the call to verify for errors.
  FileOutputStream(const char* utf8_path);

  // Destructor closes the FILE instance.
  virtual ~FileOutputStream();

  // Return true if an error occured during construction
  // or a write or put call.
  bool fail() const;

  // OutputStream overrides.
  void write(const char* str, size_t len) override;
  void put(char ch) override;

 protected:
  FILE* file_ = nullptr;
};

#endif  // TOOLS_GN_OUTPUT_STREAM_H_
