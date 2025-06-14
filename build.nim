#:____________________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
#:____________________________________________________________________
import confy


#_______________________________________
# @section Configuration
#_____________________________
const srcDir       = "."
const dir_root     = srcDir
const dir_cvk      = dir_root/"src"
const dir_examples = dir_root/"examples"

# For csys connector to glfw
const cdk = Dependency.new("cdk", "https://github.com/heysokam/cdk")

#_______________________________________
# @section Build the Examples
#_____________________________
const examples_flags = Flags(cc: @[&"-I{dir_cvk}"], ld: @["-lvulkan", "-lglfw"])
# cvk
confy.cfg.dirs.src = dir_examples
let cvk_wip = Program.new("wip.c",
  flags = examples_flags,
  deps  = @[cdk],
)

when isMainModule:
  cvk_wip.build.run

