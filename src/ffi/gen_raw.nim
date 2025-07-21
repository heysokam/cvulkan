#:___________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
#:___________________________________________________________
import std/os
import futhark
importc:
  path "../"
  outputPath currentSourcePath.parentDir/"nim"/"src"/"cvulkan"/"raw.nim"
  "cvulkan.h"
