#:___________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
#:___________________________________________________________
# Automatic Buildsystem
{.define:cvulkan.}
include ./cvulkan/compile
# API
import ./cvulkan/api ; export api
# cvulkan.nim extras
when not defined(NoGLFW):
  import ./cvulkan/extras ; export extras

