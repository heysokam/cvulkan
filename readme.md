# cvulkan | Comfy Vulkan API
`cvulkan` is a Pure C library of helpers to make Vulkan more comfortable to use  
by reducing the redundant boilerplate required by the API.  

## Current Status
See the @[TODO & Done](#todo--done) section for an up-to-date list of features pending and done.  
Current state of the library can get you up to par with `vk-bootstrap` _(aka: up to Swapchain creation)_.  
You can see an example of the bootstrapping code in the @[bootstrap](./examples/002.bootstrap.c) example file.  

## How to
> TODO: Write a simple howto intro.  
> Reference the @[examples](./examples/) folder in the meantime.  

Current state of the library can get you up to par with `vk-bootstrap` _(aka: up to Swapchain creation)_.  
You can see an example of this in the @[bootstrap](./examples/002.bootstrap.c) example file.  
Bootstrapping includes:  
- Instance  
- Validation Layers  
- Debug Messenger  
- Physical Device  
- Queue  
- Logical Device  
- Swapchain  
- Swapchain.Images  
- Swapchain.ImageViews  

### Surface Creation
`cvulkan` is completely system-agnostic.  
The `cvulkan` functions take a `VkSurfaceKHR` as input.  
How you request that handle is _up to you_ to decide.  

The examples use GLFW for requesting a `VkSurfaceKHR` handle in a cross-platform way.  
But this library is **NOT** tied to GLFW in any way.  
You could use SDL, SFML, or any other method of your choosing.  


## Buildsystem
> @note  
> You can completely ignore this library's buildsystem for your app.  
> Read the [License & Usage](#license--usage) section, add the files into your project, and good to go.  

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
Similar to other header-only libraries, add `#define cvk_Implementation` to the top of **one** `.c` file,  
and that file will become the `cvulkan` implementation that the compiler will build.  

> @note  
> Every header has its own `cvk_Implementation_*` sub-flag,  
> which behaves in the same way as the top-level `cvk_Implementation` flag,  
> but only for the contents of that one specific file.  
> `cvk_Implementation` is just an ergonomic way of adding the switches for all files, all at once.  

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


### License & Usage
This project is covered under the [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/).

#### MPL in layman terms
The [MPL v2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/) is a per-file weak copyleft license.  

This means that you _can_ use this project in a closed source application,  
without any need to build the code into a dynamic library in order to link with it.  
_(ie: like you would need to do with LGPL)_

It is also NOT a viral license.  
You are only liable to publish and/or contribute back the code for the specific files **that you modified**.  
So, if you don't modify anything, you can use it _(and relicense it)_ into your app almost as if it was MIT/BSD.  

The MPL is a not-so-well-known license.  
Please read the MPL FAQ before using this library:
https://www.mozilla.org/en-US/MPL/2.0/FAQ/


### Examples
All examples must be compiled with `-std=c23`.  

The examples default buildsystem relies on `glslc` and `xxd` installed on the system,  
but they are not needed for using the library or compiling them in a different way.  


## Naming Convention
### Casing
Regular symbols use `camelCase`.  
Typedefs and Constants use `PascalCase`.  
Namespaces are separated by `_`.  
### Name Structure
This library follows [SOV](https://en.wikipedia.org/wiki/Subject%E2%80%93object%E2%80%93verb_word_order) word order **strictly**.
```md
- Subject : The owner of the function.  `eg: vk`
- Object  : The target of an action.    `eg: instance`
- Verb    : The action being executed.  `eg: create`
- SVO     : `vkCreateInstance`
- SOV     : `vk_instance_create`
```
> @note  
> This does NOT respect english syntactical/gramattical rules, and this is **by design**.  
> _SVO causes tremendous amount of inconsistencies when creating a systematic naming convention._  


## TODO & Done
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
- [ ] Extensions
  - [x] Minimal
  - [ ] Custom
- [ ] Features
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
- [x] Wait
## Logical
- [x] Minimal creation
- [ ] Support Multi-Queue creation
## Swapchain
- [x] Context creation
- [x] Swapchain.Images Request
- [x] Swapchain.ImageViews Request
- [ ] Swapchain.recreate // Almost done.  FIX: Triggers a validation error on recreation.
```
```md
# Shaders
- [x] Single-stage
- [x] Multi-stage
      ref: https://www.leadwerks.com/community/blogs/entry/2403-single-file-spir-v-shaders-for-vulkan/
      note: Currently Untested, but should work
```
```md
# Pipeline
## State
- [x] Minimal Creation
- [ ] Customizable Options
- [ ] Sane Defaults when `NULL`
## Layout
- [x] Minimal Creation
- [ ] Customizable Options
## Context: Graphics
- [x] Minimal Creation: Single Pipeline
- [ ] Customizable Options
- [ ] Sane Defaults when `NULL`
- [ ] Multi-pipeline
```
```md
# Framebuffer & Renderpass
## Framebuffer
- [x] Minimal Creation
- [ ] Customizable Options
## Renderpass
- [x] Minimal Creation
- [ ] Customizable Options
```
```md
# Synchronization Structures
## Command Pool
- [x] Minimal Creation
- [ ] Customizable Options
## Command Buffer
- [x] Allocate: Customizable
- [x] Begin
  - [x] Minimal
  - [x] Customizable
- [x] End
- [x] Reset
## Semaphore
- [x] Creation (has no options)
## Fence
- [x] Creation: Customizable
- [x] Wait
- [x] Reset
```
```md
# Data
## Buffer
- [x] Creation: Customizable
- [x] Bind
- [x] Copy
  - [x] Buffer to Buffer
## Memory
- [x] Creation: Customizable
## Image
### Data   (aka VkImage)
- [x] Creation: Customizable
- [x] Bind
- [x] Transition
- [x] Copy
  - [x] Buffer to Image
### View
- [x] Creation: Customizable
### Sampler
- [x] Creation: Customizable
```
```md
# Memory Allocators
- [x] Minimal creation (cpu:stdlib, gpu:null)
## CPU
- [x] Custom Allocator Support
- [x] stdlib: malloc -> alloc
- [x] stdlib: calloc -> allocZ
- [x] stdlib: free   -> free
- [x] stdlib: memcpy -> copy
- [x] stdlib: ??     -> duplicate
- [ ] stdlib: memset -> set
- [ ] stdlib: ??     -> resize
## GPU
- [x] Custom Allocator Support
- [x] NULL Allocator
- [ ] Pure C Replacement for VMA
## Docs
- [ ] How to use the API
- [ ] Philosophy: Bring your own Allocator (Zig-inspired)
- [ ] Slices and Slice-like objects
```


