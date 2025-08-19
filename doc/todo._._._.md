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

