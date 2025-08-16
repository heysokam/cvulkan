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
- [x] stdlib: malloc     -> alloc
- [x] stdlib: calloc     -> allocZ
- [x] stdlib: free       -> free
- [x] stdlib: memcpy     -> copy
- [x] stdlib: calloc+cpy -> duplicate
- [x] stdlib: realloc    -> resize
- [x] stdlib: strcpy     -> string_copy
- [x] stdlib: strdup     -> string_duplicate
## GPU
- [x] Custom Allocator Support
- [x] NULL Allocator
```

```md
# VkInstance
- [x] Minimal creation
- [x] Application Info
- [x] Layers
  - [x] Validation
  - [x] Custom
- [x] Debug Messenger
- [x] Extensions
  - [x] Minimal
  - [x] Custom
  - [x] Filter out extensions not supported by the system
```

```md
- `src/cvulkan/validation.c`, `src/cvulkan/validation.h`
- [x] **Review validation layer integration**
  - [x] Verify debug callback registration
  - [x] Check validation layer availability detection
  - [x] Test validation error reporting
  - [x] Ensure proper cleanup of validation resources
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
    - [x] Custom Extensions
    - [ ] Do not add extensions that are not supported
```

```md
- `src/cvulkan/allocator.c` and `src/cvulkan/allocator.h`
- [ ] **Review memory allocation system**
  - [ ] Verify memory type selection logic
  - [ ] Check memory allocation error handling
  - [ ] Test memory mapping and unmapping
  - [ ] Ensure proper memory cleanup
```

```md
- [ ] **Documentation updates**
  - [ ] Update allocator.h documentation
  - [ ] Document memory allocation patterns
```

