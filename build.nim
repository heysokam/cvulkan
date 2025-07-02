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
const dir_helpers  = dir_examples/"helpers"
const dir_shd      = dir_examples/"shaders"


#_______________________________________
# @section Examples: Shaders
#_____________________________
proc spirv *(src :PathLike, trg :PathLike) :CommandList=
  result = @[]
  let spv = src&".spv"
  # glslc src -o src.ext.spv
  var glslc = confy.Command()
  glslc.add "glslc"
  glslc.add src
  glslc.add "-o"
  glslc.add spv
  result.add glslc
  # xxd -i file.ext.spv trg.ext
  var xxd = confy.Command()
  xxd.add "xxd"
  xxd.add "-i"
  xxd.add spv
  xxd.add trg
  result.add xxd
#_____________________________
let triangle_vert = spirv( dir_shd/"triangle.vert", dir_shd/"triangle.vert.c" )
let triangle_frag = spirv( dir_shd/"triangle.frag", dir_shd/"triangle.frag.c" )


#_______________________________________
# @section Build the Examples
#_____________________________
const examples_flags = Flags(cc: @[&"-I{dir_cvk}", &"-I{dir_helpers}"], ld: @["-lvulkan", "-lglfw"])
# cvk
confy.cfg.dirs.src = dir_examples
let example_wip = Program.new("wip.c",           flags= examples_flags)
let example_001 = Program.new("001.instance.c",  flags= examples_flags)
let example_002 = Program.new("002.bootstrap.c", flags= examples_flags)
let example_003 = Program.new("003.triangle.c",  flags= examples_flags)

when isMainModule:
  triangle_vert.run()
  triangle_frag.run()
  example_001.build.run
  example_002.build.run
  example_003.build.run
  example_wip.build.run

