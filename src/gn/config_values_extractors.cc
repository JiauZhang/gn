// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gn/config_values_extractors.h"

#include "gn/escape.h"
#include "gn/output_stream.h"

namespace {

class EscapedStringWriter {
 public:
  explicit EscapedStringWriter(const EscapeOptions& escape_options)
      : escape_options_(escape_options) {}

  void operator()(const std::string& s, OutputStream& out) const {
    out << " ";
    EscapeStringToStream(out, s, escape_options_);
  }

 private:
  const EscapeOptions& escape_options_;
};

}  // namespace

void RecursiveTargetConfigStringsToStream(
    RecursiveWriterConfig config,
    const Target* target,
    const std::vector<std::string>& (ConfigValues::*getter)() const,
    const EscapeOptions& escape_options,
    OutputStream& out) {
  RecursiveTargetConfigToStream(config, target, getter,
                                EscapedStringWriter(escape_options), out);
}
