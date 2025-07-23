#:___________________________________________________________
#  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
#:___________________________________________________________
# @deps std
from std/os import absolutePath
from std/strutils import replace
# @deps builder
import confy

#_______________________________________
# @section Configuration
#_____________________________
# Folders
const srcDir       = "."
const dir_root     = srcDir
const dir_cvk      = dir_root/"src"
const dir_ffi_base = dir_cvk/"ffi"
const dir_ffi_zig  = dir_ffi_base/"zig"
const dir_ffi_nim  = dir_ffi_base/"nim"
const dir_examples = dir_root/"examples"
const dir_helpers  = dir_examples/"helpers"
const dir_shd      = dir_examples/"shaders"
let   dir_local    = os.absolutePath(".")
# Files
const file_ffi_base    = dir_ffi_base/"impl.c"
const file_ffi_zig     = dir_ffi_zig/"src"/"cvulkan.zig"
const file_ffi_nim_api = dir_ffi_nim/"src"/"cvulkan"/"api.nim"
const file_ffi_nim_raw = dir_ffi_nim/"src"/"cvulkan"/"raw.nim"



#_______________________________________
# @section FFI Bindings: Nim
#_____________________________
proc gen_nim=
  info "Generating the cvulkan bindings for nim with Futhark ..."
  #___________________
  const deps_futhark = Dependencies.new(
    Dependency.new( "futhark",      "https://github.com/PMunch/futhark"      ),
    Dependency.new( "libclang-nim", "https://github.com/PMunch/libclang-nim" ),
    Dependency.new( "termstyle",    "https://github.com/PMunch/termstyle"    ),
    Dependency.new( "macroutils",   "https://github.com/PMunch/macroutils"   ),
    Dependency.new( "nimbleutils",  "https://github.com/PMunch/nimbleutils"  ),
    ) #:: deps_futhark
  #___________________
  # Install Opir if it doesn't exist
  let opir = getEnv("HOME")/".local"/"bin"/"opir"
  if not fileExists(opir):
    info &"Opir does not exist at {opir.parentDir()}"
    info "Installing Futhark and Opir ..."
    let nimble = confy.cfg.nimble.bin.absolutePath
    withDir confy.cfg.dirs.lib/"futhark": sh &"{nimble} install"
    ln getEnv("HOME")/".nimble"/"bin"/"opir", opir
    info "Done installing Futhark and Opir."
  #___________________
  confy.cfg.dirs.src = dir_ffi_base
  var gen = Program.new("gen.nim",
    deps  = deps_futhark,
    args  = @["--maxLoopIterationsVM:1_000_000_000", "-d:futharkRebuild", "-d:nodeclguards"],
    cfg   = cfg,
    ) #:: Program.new( ... )
  confy.build gen
  writeFile( file_ffi_nim_api, readFile(file_ffi_nim_api).replace(dir_local, ".") )
  #___________________
  info "Generating the raw cvulkan bindings for nim with Futhark ..."
  gen.src[0] = confy.cfg.dirs.src/"gen_raw.nim"
  confy.build gen
  writeFile( file_ffi_nim_raw, readFile(file_ffi_nim_raw).replace(dir_local, ".") )
  #___________________
  info "Done creating the nim bindings."

#_______________________________________
# @section FFI Bindings: Zig
#_____________________________
proc gen_zig=
  info "Generating the cvulkan bindings for zig with translate-c ..."
  sh confy.cfg.zig.bin, "translate-c", "-I/usr/include", file_ffi_base, ">", file_ffi_zig
  info "Done creating the zig bindings."


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
# @section Examples
#_____________________________
const examples_flags = Flags(
  cc: @[&"-I{dir_cvk}", &"-I{dir_helpers}", "-Wno-documentation-unknown-command"],
  ld: @["-lvulkan", "-lglfw"])
confy.cfg.dirs.src = dir_examples
let example_wip = Program.new("wip.c",           flags= examples_flags)
let example_001 = Program.new("001.instance.c",  flags= examples_flags)
let example_002 = Program.new("002.bootstrap.c", flags= examples_flags)
let example_003 = Program.new("003.triangle.c",  flags= examples_flags)


#_______________________________________
# @section Order to build
#_____________________________
when isMainModule:
  example_shaders_compile
  example_001.build.run
  example_002.build.run
  example_003.build.run
  example_wip.build.run
  # gen_zig()
  # gen_nim()

