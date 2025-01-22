// Copyright (c) 2025 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gn/output_stream.h"

#include <limits>

#include "util/test/test.h"

TEST(OutputStream, AppendIntDecimals) {
  static const int kValues[] = {
      0, 1, -1, 12345678, -12345678, INT_MIN, INT_MAX,
  };
  for (const auto value : kValues) {
    char expected[20];
    snprintf(expected, sizeof(expected), "%d", value);

    StringOutputStream s;
    s << value;
    EXPECT_EQ(s.str(), expected) << value;
  }
}

TEST(OutputStream, AppendUIntDecimals) {
  static const unsigned kValues[] = {
      0,
      1,
      12345678,
      UINT_MAX,
  };
  for (const auto value : kValues) {
    char expected[20];
    snprintf(expected, sizeof(expected), "%u", value);

    StringOutputStream s;
    s << value;
    EXPECT_EQ(s.str(), expected) << value;
  }
}

TEST(OutputStream, AppendLongDecimals) {
  static const long kValues[] = {
      0, 1, -1, 12345678, -12345678, INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
  };
  for (const auto value : kValues) {
    char expected[32];
    snprintf(expected, sizeof(expected), "%ld", value);

    StringOutputStream s;
    s << value;

    EXPECT_EQ(s.str(), expected) << value;
  }
}

TEST(OutputStream, AppendULongDecimals) {
  static const unsigned long kValues[] = {
      0, 1, 12345678, UINT_MAX, ULONG_MAX,
  };
  for (const auto value : kValues) {
    char expected[32];
    snprintf(expected, sizeof(expected), "%lu", value);

    StringOutputStream s;
    s << value;
    EXPECT_EQ(s.str(), expected) << value;
  }
}

TEST(OutputStream, AppendLongLongDecimals) {
  static const long long kValues[] = {
      0,       1,        -1,       12345678,  -12345678, INT_MIN,
      INT_MAX, LONG_MIN, LONG_MAX, LLONG_MIN, LLONG_MAX,
  };
  for (const auto value : kValues) {
    char expected[48];
    snprintf(expected, sizeof(expected), "%lld", value);

    StringOutputStream s;
    s << value;
    EXPECT_EQ(s.str(), expected) << value;
  }
}

TEST(OutputStream, AppendULongLongDecimals) {
  static const unsigned long long kValues[] = {
      0, 1, 12345678, UINT_MAX, ULONG_MAX, ULLONG_MAX,
  };
  for (const auto value : kValues) {
    char expected[48];
    snprintf(expected, sizeof(expected), "%llu", value);

    StringOutputStream s;
    s << value;
    EXPECT_EQ(s.str(), expected) << value;
  }
}
