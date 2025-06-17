#:__________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
#:__________________________________________________________
import confy


#_______________________________________
# @section Configuration
#_____________________________
const srcDir       = "."
const dir_root     = srcDir
const dir_cvk      = dir_root/"src"
const dir_examples = dir_root/"examples"
const dir_shd      = dir_examples/"shaders"

# For csys connector to glfw
const cdk = Dependency.new("cdk", "https://github.com/heysokam/cdk")


#_______________________________________
# @section Examples: Shaders
#_____________________________
proc spirv *(src :PathLike, trg :PathLike) :Command=
  result = confy.Command()
  result.add "glslc"
  result.add src
  result.add "-o"
  result.add trg
#_____________________________
let triangle_vert = spirv( dir_shd/"triangle.vert", dir_shd/"triangle.vert.spv" )
let triangle_frag = spirv( dir_shd/"triangle.frag", dir_shd/"triangle.frag.spv" )


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
  triangle_vert.run()
  triangle_frag.run()
  cvk_wip.build.run

