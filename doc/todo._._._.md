## ?.?.? - API Polish & Documentation  (1.0.0 minus 0.2.0)
**Goal**: Polish the API design and create comprehensive documentation.

### Success Criteria
- API is consistent and user-friendly
- Documentation is complete and accurate
  - Error handling is comprehensive and clear
  - Customization is properly documented
  - Bring-your-own-Allocator pattern is properly explained
  - Slice types are documented, and have simple usage examples

### Tasks
```md
## General Tasks
- [ ] Review `_create` function arguments
  - [ ] @internal Functions should be marked/tagged as such, and take individual/separate arguments.
  - [ ] All external API functions should take a `_args` object.
  - [ ] Cleanup all inconsistencies in this rule across the entire lib.
- [ ] TODO/FIX/etc tags hanging in the code
  - [ ] Complete all `TODO:` tags
  - [ ] Solve the issues for every `FIX:` tag
- [ ] Write doc-comments for everything at `cvulkan/types.h`
```

```md
# Memory Allocators
## CPU
- [ ] stdlib: memset     -> set
## Docs
- [ ] How to use the API
- [ ] Philosophy: Bring your own Allocator (Zig-inspired)
- [ ] Slices and Slice-like objects
```
```md
## Queue
- [ ] Arbitrary Queue creation (multi-queue)
      Needs support on the device.logical function
```
```md
## Swapchain
- [ ] Swapchain.recreate // Almost done.  FIX: Triggers a validation error on recreation.
```
```md
## Shaders
- [ ] **Test multi-stage shader support**
  - [ ] Verify vertex-fragment shader combination
  - [ ] Check shader stage linking
  - [ ] Test shader stage validation
  - [ ] Ensure proper stage management
```


```md
##  Documentation for 0.1.0: Instance, Validation & General Purpose Helpers
- [ ] **Instance & Validation**
  - [x] Update instance.h documentation
  - [ ] Add usage examples to readme
  - [ ] Document validation layer configuration
  - [ ] Instance Example
    - [x] Validation enabled
    - [ ] Validation disabled
    - [ ] Custom debug callback
    - [x] Custom Extensions
    - [x] Does not add extensions that are not supported
- [ ] **Allocator**
  - [x] Update allocator.h documentation
  - [ ] Document memory allocation patterns
```
```md
## Documentation for 0.2.0 - Physical Device Selection
- [ ] **Documentation updates**
  - [x] Update device.h documentation
  - [ ] Add device selection examples
  - [ ] Document feature checking patterns
  - [ ] **Device selection example**
    - [ ] Automatic device selection
    - [ ] Manual device selection
    - [ ] Specific feature requirements
    - [ ] Multiple device support
```
```md
## Documentation for 0.3.0 - Logical Device
- [ ] **Documentation updates**
  - [ ] Update device.h documentation
  - [ ] Add queue management examples
  - [ ] Document device feature patterns
  - [ ] **Device Example**
```
```md
## Documentation for 0.4.0 - Device Queue
- [ ] **Documentation updates**
  - [ ] **Queue Example**
    - [ ] Graphics queue only
    - [ ] Compute queue
    - [ ] Transfer queue
    - [ ] Multiple queue families
```
```md
## Documentation for 0.5.0 - Surface & Swapchain
- [ ] **Documentation updates**
  - [ ] Update surface.h documentation
  - [ ] Add swapchain management examples
  - [ ] Document surface integration patterns
  - [ ] **Swapchain Example**
    - [ ] Example with basic swapchain setup
    - [ ] Example with swapchain recreation
    - [ ] Example with different surface formats
    - [ ] Example with multiple swapchain images
    - [ ] **Fix swapchain recreation issues**
      - [ ] Resolve validation errors during recreation
      - [ ] Implement proper cleanup before recreation
      - [ ] Test recreation with different surface sizes
      - [ ] Ensure robust error handling
```
```md
## Documentation for 0.6.0 - Shaders
- [ ] **Documentation updates**
  - [ ] Update shader.h documentation
  - [ ] Add shader compilation examples
  - [ ] **Shader Example**
    - [x] Single-stage shader
    - [ ] Multi-stage shader
    - [ ] Shader hot-reloading
```
```md
## Documentation for 0.7.0 - Static Rendering Elements
- [ ] **Documentation updates**
  - [ ] Update rendering.h documentation
  - [ ] Document render pass patterns
  - [ ] **Static Rendering example**
    - [ ] Example with basic triangle rendering
    - [ ] Example with texture rendering
    - [ ] Example with multiple render targets
    - [ ] Example with render pass dependencies
```
```md
## Documentation for 0.8.0 - Synchronization Structures
- [ ] **Documentation updates**
  - [ ] Update sync.h documentation
  - [ ] Add synchronization examples
  - [ ] Document command recording patterns
  - [ ] **Synchronization Example**
    - [ ] Basic Command recording
    - [ ] Frame Synchronization
    - [ ] Resource Synchronization
    - [ ] Multi-Queue operations
```
```md
## Documentation for 0.9.0 - Graphics Pipeline
- [ ] **Documentation updates**
  - [ ] Update pipeline.h documentation
  - [ ] Document pipeline state patterns
  - [ ] Add rendering pipeline examples
  - [ ] **Triangle Rendering example**
    - [ ] Basic colored triangle
    - [ ] Textured triangle
    - [ ] Multiple triangle rendering
    - [ ] Dynamic triangle updates
  - [ ] **Model Loading+Rendering example**
    - [ ] GLTF model loading
    - [ ] Model transformation
    - [ ] Multiple model rendering
    - [ ] Model animation support
```
```md
## Documentation for 0.10.0 - Buffers
- [ ] **Documentation updates**
  - [ ] Update data.h documentation about Buffers and Memory
  - [ ] Add buffer management examples
  - [ ] **Create comprehensive buffer example**
    - [ ] Example with vertex buffer creation
    - [ ] Example with uniform buffer creation
    - [ ] Example with staging buffer operations
    - [ ] Example with buffer data transfer
```
```md
## Documentation for 0.11.0 - Images & Textures
- [ ] **Documentation updates**
  - [ ] Update data.h documentation
  - [ ] Add image management examples
  - [ ] Document texture loading patterns
  - [ ] **Image Example**
    - [ ] Texture loading
    - [ ] Image transitions
    - [ ] Image copying
    - [ ] Texture sampling
```




```md
- [ ] **API consistency review**
  - [ ] Function naming consistency
  - [ ] Parameter ordering consistency
  - [ ] Error handling consistency
  - [ ] Resource management consistency
```

```md
- [ ] **Error handling improvements**
  - [ ] Comprehensive error codes
  - [ ] Error message clarity
  - [ ] Error recovery mechanisms
  - [ ] Error reporting improvements
```

```md
- [ ] **Documentation completion**
  - [ ] API reference documentation
  - [ ] Usage examples and tutorials
  - [ ] Best practices guide
  - [ ] Performance guidelines
```

```md
- [ ] **API design validation**
  - [ ] Usability testing
  - [ ] Performance validation
  - [ ] Memory safety validation
  - [ ] Thread safety validation
```

---

## ?.?.? - Cross-Language FFI  (1.0.0 minus 0.1.0)
**Goal**: Implement and validate FFI bindings for other languages.

### Success Criteria
- FFI bindings work reliably
- Cross-language examples demonstrate functionality
- Performance is acceptable across languages
- API design is validated across different paradigms

### Tasks
```md
- [ ] **Nim FFI implementation**
  - [ ] Core API bindings
  - [ ] Type system integration
  - [ ] Memory management integration
  - [ ] Error handling integration
```

```md
- [ ] **Zig FFI implementation**
  - [ ] Core API bindings
  - [ ] Type system integration
  - [ ] Memory management integration
  - [ ] Error handling integration
```

```md
- [ ] **Cross-language examples**
  - [ ] Basic triangle rendering in Nim
  - [ ] Basic triangle rendering in Zig
  - [ ] Advanced features in both languages
  - [ ] Performance comparison
```

```md
- [ ] **FFI validation**
  - [ ] API compatibility testing
  - [ ] Memory safety validation
  - [ ] Performance validation
  - [ ] Error handling validation
```


## ?.?.? - VMA Replacement   (1.0.0 plus ?.?.0)
```md
# Memory Allocators
## GPU
- [ ] Pure C Replacement for VMA
```

