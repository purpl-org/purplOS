#!/usr/bin/env python2

# Copyright (c) 2014 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""
Verifies libraries have proper mtime.
"""

import TestGyp

import sys

if sys.platform == 'darwin':
  test = TestGyp.TestGyp(formats=['ninja', 'make', 'xcode'])

  CHDIR = 'libtool-zero'

  test.run_gyp('test.gyp', chdir=CHDIR)

  test.build('test.gyp', 'mylib', chdir=CHDIR)

  test.up_to_date('test.gyp', 'mylib', chdir=CHDIR)

  test.pass_test()
