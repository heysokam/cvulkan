# cvulkan | Comfy Vulkan API
`cvulkan` is a Pure C library of helpers to make Vulkan more comfortable to use  
by reducing the redundant boilerplate required by the API.  

_(Think `vk-bootstrap`, but with Modern C and a touch of `wgpu-native` instead)_  


## How to
> TODO: Write a simple howto intro.  
> Reference the @[examples](./examples/) folder in the meantime.  
> Current state of the library can render a fully textured GLTF model.  

`cvulkan` focuses exclusively on ergonomics & helper code.  
The goal is to fully respect Vulkan concepts and constructs,  
with little to no abstraction.  

At any time, you'll be able to drop the `cvulkan` API  
and write standard Vulkan using the Vulkan specification as normal.  

### Bootstrapping
You can see an example of how bootstrapping works in the @[bootstrap](./examples/002.bootstrap.c) example file.  
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
`cvulkan` is completely windowing-system agnostic.  
The `cvulkan` functions take a `VkSurfaceKHR` as input.  
How you request that handle is _up to you_ to decide.  

The examples use GLFW for requesting a `VkSurfaceKHR` handle in a cross-platform way.  
But this library is **NOT** tied to GLFW in any way.  
You could use SDL, SFML, or any other method of your choosing.  


## TODO & Done
Current state of the library can render a fully textured GLTF model.  
You can see an example of the bootstrapping code in the @[bootstrap](./examples/002.bootstrap.c) example file.  

Up-to-date list of features pending and done:

| Version | Focus                             | Status    | File            |
|---------|-----------------------------------|-----------|-----------------|
|  0.1.0  | Instance, Validation & Helpers    | 🔄 Review | [doc/todo.0.1.0.md](doc/todo.0.1.0.md)    |
|  0.2.0  | Physical Device Selection         | 🔄 Review | [doc/todo.0.2.0.md](doc/todo.0.2.0.md)    |
|  0.3.0  | Logical Device                    | 🔄 Review | [doc/todo.0.3.0.md](doc/todo.0.3.0.md)    |
|  0.4.0  | Device Queue                      | 🔄 Review | [doc/todo.0.4.0.md](doc/todo.0.4.0.md)    |
|  0.5.0  | Surface & Swapchain               | 🔄 Review | [doc/todo.0.5.0.md](doc/todo.0.5.0.md)    |
|  0.6.0  | Shaders                           | 🔄 Review | [doc/todo.0.6.0.md](doc/todo.0.6.0.md)    |
|  0.7.0  | Static Rendering Elements         | 🔄 Review | [doc/todo.0.7.0.md](doc/todo.0.7.0.md)    |
|  0.8.0  | Synchronization Structures        | 🔄 Review | [doc/todo.0.8.0.md](doc/todo.0.8.0.md)    |
|  0.9.0  | Graphics Pipeline                 | 🔄 Review | [doc/todo.0.9.0.md](doc/todo.0.9.0.md)    |
| 0.10.0  | Buffers                           | 🔄 Review | [doc/todo.0.10.0.md](doc/todo.0.10.0.md)  |
| 0.11.0  | Images & Textures                 | 🔄 Review | [doc/todo.0.11.0.md](doc/todo.0.11.0.md)  |
| 0.12.0  | Compute Pipeline                  | 📋 TODO   | [doc/todo.0.12.0.md](doc/todo.0.12.0.md)  |
| 0.13.0  | Dynamic Rendering                 | 📋 TODO   | [doc/todo.0.13.0.md](doc/todo.0.13.0.md)  |
| 0.14.0  | Descriptor Indexing _(bindless)_  | 📋 TODO   | [doc/todo.0.14.0.md](doc/todo.0.14.0.md)  |
| 0.15.0  | Synchonization 2                  | 📋 TODO   | [doc/todo.0.15.0.md](doc/todo.0.15.0.md)  |
|  ?.?.?  | API Polish & Documentation        | 📋 TODO   | [doc/todo.?.?.?.md](doc/todo._._._.md)   |
|  ?.?.?  | Cross-Language FFI : Zig          | 📋 TODO   | [doc/todo.?.?.?.md](doc/todo._._._.md)   |
|  ?.?.?  | Cross-Language FFI : Nim          | 📋 TODO   | [doc/todo.?.?.?.md](doc/todo._._._.md)   |
|  1.0.0  | Production Ready                  | 🎯 Target | [doc/todo.1.0.0.md](doc/todo.1.0.0.md)    |
|  ?.?.?  | Unlikely/Experimental/Backlog     | (N/A)     | [doc/todo.unlikely.md](doc/todo.unlikely.md) |

```md
# Legend
- 🔄 Review : Existing work, ready to review and polish
- 📋 TODO   : New features to implement
- ️✅ Done   : Completed task
- 🎯 Target : Production release
```


## Buildsystem
> @note  
> You can completely ignore this library's buildsystem for your app.  
> Read the [License & Usage](#license--usage) section, add the files into your project, and good to go.  

This library offers support for both Multifile and Single Compilation Unit (SCU).  


### Dependency: C stdlib  _(optional)_
This project depends only on the C standard library.  
This is only by default. Every usage of stdlib **can be overriden**.  
Any C stdlib usage is contained within the [<cvulkan/base.h>](./src/cvulkan/base.h) header file.  


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
> that affects only the contents of that one specific file.  
> `cvk_Implementation` is just an ergonomic way of adding the switches for all files, all at once.  

```c
// yourapp.c
#define cvk_Implementation
#include <cvulkan.h>
/* ... Use cvulkan here ... */
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


## License & Usage
This project is covered under the [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/).

The [MPL v2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/) is a per-file weak copyleft license.  

Just like with MIT, you _can_ use this project in a closed source application,  
without the license spreading to your own files, or having to build the code into a dynamic library.  
_(ie: like GPL/LGPL would require)_  
If you don't modify anything, you can use _(and relicense)_ the code into your app as if it was MIT/BSD.  

The MPL is a not-so-well-known license.  
Please read their [FAQ](https://www.mozilla.org/en-US/MPL/2.0/FAQ/) if you were not aware of its existence.  

