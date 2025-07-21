#:___________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
#:___________________________________________________________
import confy


#_______________________________________
# @section Configuration
#_____________________________
const srcDir       = "."
const dir_root     = srcDir
const dir_cvk      = dir_root/"src"
const dir_ffi_base = dir_cvk/"ffi"
const dir_ffi_zig  = dir_ffi_base/"zig"
const dir_ffi_nim  = dir_ffi_base/"nim"
const dir_examples = dir_root/"examples"
const dir_helpers  = dir_examples/"helpers"
const dir_shd      = dir_examples/"shaders"


#_______________________________________
# @section Create the FFI Bindings
#_____________________________
const file_ffi_base = dir_ffi_base/"impl.c"
const file_ffi_zig  = dir_ffi_zig/"src"/"cvulkan.zig"
const file_ffi_nim  = dir_ffi_nim/"src"/"cvulkan.nim"
when isMainModule:
  # Nim Bindings
  discard  # TODO:.. 

when isMainModule:
  # Zig Bindings
  sh confy.cfg.zig.bin, "translate-c", "-I/usr/include", file_ffi_base, ">", file_ffi_zig
  # dir_ffi_zig


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
let triangle_hardcoded_vert = spirv( dir_shd/"triangle.vert", dir_shd/"triangle.vert.c" )
let triangle_hardcoded_frag = spirv( dir_shd/"triangle.frag", dir_shd/"triangle.frag.c" )
let triangle_buffered_vert  = spirv( dir_shd/"triangle_buffered.vert", dir_shd/"triangle_buffered.vert.c" )
let triangle_buffered_frag  = spirv( dir_shd/"triangle_buffered.frag", dir_shd/"triangle_buffered.frag.c" )
template example_shaders_compile=
  triangle_hardcoded_vert.run()
  triangle_hardcoded_frag.run()
  triangle_buffered_vert.run()
  triangle_buffered_frag.run()



#_______________________________________
# @section Build the Examples
#_____________________________
const examples_flags = Flags(
  cc: @[&"-I{dir_cvk}", &"-I{dir_helpers}", "-Wno-documentation-unknown-command"],
  ld: @["-lvulkan", "-lglfw"])
confy.cfg.dirs.src = dir_examples
let example_wip = Program.new("wip.c",           flags= examples_flags)
let example_001 = Program.new("001.instance.c",  flags= examples_flags)
let example_002 = Program.new("002.bootstrap.c", flags= examples_flags)
let example_003 = Program.new("003.triangle.c",  flags= examples_flags)

when isMainModule:
  example_shaders_compile
  example_001.build.run
  example_002.build.run
  example_003.build.run
  example_wip.build.run

