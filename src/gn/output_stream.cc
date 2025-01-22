// Copyright (c) 2025 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gn/output_stream.h"

#include <limits.h>

OutputStream& OutputStream::operator<<(unsigned long long value) {
  const size_t buffer_size = 24;
  char buffer[buffer_size];
  char* end = buffer + buffer_size;
  char* pos = end;
  do {
    *(--pos) = '0' + static_cast<char>(value % 10);
    value /= 10;
  } while (value != 0);
  write(pos, static_cast<size_t>(end - pos));
  return *this;
}

OutputStream& OutputStream::operator<<(long long value) {
  const size_t buffer_size = 24;
  char buffer[buffer_size];
  char* end = buffer + buffer_size;
  char* pos = end;

  bool has_sign = (value < 0);
  if (has_sign) {
    // NOTE: |LLONG_MIN == -LLONG_MIN| must be handled here.
    if (value == LLONG_MIN) {
      *(--pos) = '8';
      value /= 10;
    }
    value = -value;
  }

  do {
    *(--pos) = '0' + static_cast<char>(value % 10);
    value /= 10;
  } while (value != 0);
  if (has_sign)
    *(--pos) = '-';
  write(pos, static_cast<size_t>(end - pos));
  return *this;
}

OutputStream& OutputStream::operator<<(unsigned value) {
  return *this << static_cast<unsigned long long>(value);
}

OutputStream& OutputStream::operator<<(int value) {
  return *this << static_cast<long long>(value);
}

OutputStream& OutputStream::operator<<(unsigned long value) {
  return *this << static_cast<unsigned long long>(value);
}

OutputStream& OutputStream::operator<<(long value) {
  return *this << static_cast<long long>(value);
}

FileOutputStream::FileOutputStream(const char* utf8_path) {
  file_ = fopen(utf8_path, "rw");
}

FileOutputStream::~FileOutputStream() {
  fclose(file_);
}

bool FileOutputStream::fail() const {
  return ferror(file_) != 0;
}

void FileOutputStream::write(const char* str, size_t len) {
  fwrite(str, 1, len, file_);
}

void FileOutputStream::put(char ch) {
  fputc(ch, file_);
}
