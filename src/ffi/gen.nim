#:___________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
#:___________________________________________________________
# @deps std
import std/os
import std/strutils
# @deps external
import futhark
# @deps generator
import ./gen_cfg as cfg

#_____________________________
proc rename (
    n           : string;
    k           : futhark.SymbolKind;
    p           : string;
    overloading : var bool;
  ) :string=
  overloading = true
  result = n
  # General Rename
  for entry in cfg.replaceList:
    result = result.replace( entry[0], entry[1] )
  # Start Rename
  for entry in cfg.stripPrefix:
    if result.startsWith( entry ) : result = result[entry.len .. ^1]
  for entry in cfg.stripStart:
    if result.startsWith( entry ) : result = result[entry.len .. ^1]
  for entry in cfg.replaceStart:
    if result.startsWith( entry[0] ) : result = entry[1] & result[entry[0].len .. ^1]
  # End Rename
  for entry in cfg.stripEnd:
    if result.endsWith( entry ) :
      if entry == "_t": result[0] = result[0].toUpperAscii()
      result = result[0..^entry.len+1]
      if result in cfg.addT: result = result&"T"
#_____________________________
importc:
  renameCallback rename
  path "../"
  outputPath currentSourcePath.parentDir/"nim"/"src"/"cvulkan/api.nim"
  "cvulkan.h"
