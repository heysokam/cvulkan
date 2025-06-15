# cvulkan | Comfy Vulkan API
`cvulkan` is a library of helpers to reduce redundant boilerplate when using Vulkan.

## How to
> TODO: Write a simple howto intro.  
> Reference the @[examples](./examples/) folder in the meantime.  

## TODO
```md
# VkResult
- [x] toString
- [x] check
- [x] assert
```
```md
# VkInstance
- [x] Minimal creation
- [x] Application Info
- [x] Validation Layers
- [x] Debug Messenger
- [ ] Extensions
  - [x] Minimal
  - [ ] Custom
```
```md
# VkSurfaceKHR
- [x] Accept a Surface handle from the user
- [x] Destroy helper for naming consistency
```
```md
# Device
## Physical
- [x] Minimal creation
      Using `isSuitable` and surface support
- [ ] Suitability
  - [x] Boolean: Overridable `device_physical_isSuitable` function
        Returns true/false for whether the device is valid or not
  - [ ] Ranked : Overridable `device_physical_rank` function
        Track scores and pick the best
## Queue
- [x] Minimal creation (1 graphics+present Q)
- [ ] Arbitrary Queue creation (multi-queue)
      Needs support on the device.logical function
## Logical
- [x] Minimal creation
- [ ] Support Multi-Queue creation
```
```md
# Memory Allocators
- [x] Minimal creation (cpu:stdlib, gpu:null)
## CPU
- [x] Custom Allocator Support
- [x] stdlib: malloc -> alloc
- [x] stdlib: calloc -> allocZ
- [x] stdlib: free   -> free
- [ ] stdlib: memcpy -> copy
- [ ] stdlib: memset -> set
## GPU
- [x] Custom Allocator Support
- [x] NULL Allocator
- [ ] Pure C Replacement for VMA
```

## Buildsystem
> @note  
> You can completely ignore this library's buildsystem for your app.  
> Read the [License & Linking](#license--linking) section, add the files into your project, and good to go.  

This library offers support for both Multifile and Single Compilation Unit (SCU).  


### Multifile
To build this library in multifile mode,
add every `.c` file contained inside [./src](./src/) into your project's buildsystem.  
You can add every file recursively. The structure is prepared for that.  

This library's files can be compiled into separate modules without issues.  
This means that you can use only those parts of the library that you find useful,  
and potentially ignore _(as in: not compile at all)_ those parts that you don't use.  

Some modules depend on others (eg: `instance.c` -requires-> `validation.c`).  
But most of the files are independent and only use the other modules's typedefs.  


### Single Header / Single Unit  (SCU)
SCU support is controlled with the `cvk_Implementation` set of `#define` switches.  
Similar to other header-only libraries, you'd add `#define cvk_Implementation` to the top of **one** `.c` file,  
before importing the main header of this library,  
and that file will become the `cvulkan` implementation that the compiler will build.  

> @note  
> Every header has its own `cvk_Implementation_*` sub-flag,  
> which behaves in the same way as the top-level `cvk_Implementation` flag,  
> but only for the contents of that one specific file.  
> `cvk_Implementation` is just an ergonomic way of adding the switches from all files, all at once.  

```c
// yourapp.c
#define cvk_Implementation
#include <cvulkan.h>
int main() {
  cvk_print("Hello %s!", cvk_config_Name);
}
```

### Build Flags
> @note  
> The library has been developed using `-Weverything`,  
> with minimal filters to trim the list down into a very strict set of flags.  
> You can use any set of flags you'd normally use, no matter how strict or lenient they are.

`cvulkan` aims to support `-std=c11` or higher,  
and it compiles perfectly with both sanitization active and the strictest set of flags possible.  
eg: `-Wall -Wextra -Werror -Wpedantic -pedantic-errors -fsanitize=all`

You'll need to add `-lvulkan` to your list of link flags to make the Vulkan symbols available.  


### Dependency: C stdlib
This project depends only on the C standard library.  
This is only by default, as every usage of stdlib **can be overriden**.  
Every stdlib usage is contained within the [<cvulkan/base.h>](./src/cvulkan/base.h) header file.  


### License & Linking
This library uses the LGPL license.  

You _can_ use this project in a closed source application,  
but you **must** build the code of this project into a static/dynamic library, and **link** directly **to the result**.  
Otherwise you'd be breaking the terms and conditions of this library's license.

Both SCU and Multifile are supported for such a case,  
but you must build them separately from your project and link your app to the result.  


### Examples
All examples must be compiled with `-std=c23`.  
They use helper code from @[`heysokam/cdk`](https://github.com/heysokam/cdk), which is a c23 library.  

