#:___________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
#:___________________________________________________________
const stripPrefix * = [
  "cvk_",
  ] #:: stripPrefix = [ ... ]
const stripStart  * = [
  "",
  ] #:: stripStart = [ ... ]
const replaceStart * = [
  ("", ""),
  ] #:: replaceStart = [ ... ]
const replaceEnd * = [
  ("",""),
  ] #:: replaceEnd = [ ... ]
const stripEnd * = [
  "",
  ]
const addT *:seq[string]= @[]

const replaceList * = [
  ("ptr",         "Ptr"    ),
  ("cvk_bool",    "Bool"   ),
  ("cvk_pointer", "Pointer"),

  # Fix vulkan name collisions for nim
  ("VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT", "VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT_enumval"),
  ("VK_COLOR_SPACE_SRGB_NONLINEAR_KHR",            "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR_enumval"),
  ] #:: replaceEnd = [ ... ]

