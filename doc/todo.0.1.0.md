## 0.1.0 - Instance, Validation & General Purpose Helpers
**Goal**: Instance Creation, Validation Layers, Debug Messaging, Memory allocation, Result Helpers

```md
# VkResult
- [x] toString
- [x] check
- [x] assert
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
- `src/cvulkan/validation.c`, `src/cvulkan/validation.h`
- [ ] **Review validation layer integration**
  - [ ] Verify debug callback registration
  - [ ] Check validation layer availability detection
  - [ ] Test validation error reporting
  - [ ] Ensure proper cleanup of validation resources
```

```md
- `src/cvulkan/extensions.h`
- [ ] **Review extension management**
  - [ ] Verify extension enumeration
  - [ ] Check extension availability validation
  - [ ] Test extension loading and usage
```

```md
- `src/cvulkan/instance.c`, `src/cvulkan/instance.h`
- [ ] **Review instance creation flow**
  - [ ] Verify all required extensions are properly handled
  - [ ] Check validation layer setup is complete
  - [ ] Ensure debug messenger configuration is robust
```

```md
- [ ] **Documentation updates**
  - [ ] Update instance.h documentation
  - [ ] Add usage examples to readme
  - [ ] Document validation layer configuration
  - [ ] Instance Example
    - [ ] Validation enabled
    - [ ] Validation disabled
    - [ ] Custom debug callback
    - [ ] Custom Extensions
```

```md
- `src/cvulkan/memory.c` and `src/cvulkan/memory.h`
- [ ] **Review memory allocation system**
  - [ ] Verify memory type selection logic
  - [ ] Check memory allocation error handling
  - [ ] Test memory mapping and unmapping
  - [ ] Ensure proper memory cleanup
```

```md
- [ ] **Documentation updates**
  - [ ] Update memory.h documentation
  - [ ] Document memory allocation patterns
```

