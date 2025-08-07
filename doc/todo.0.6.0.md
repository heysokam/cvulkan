## 0.6.0 - Shaders
**Goal**: Shader Module management

```md
# Shaders
- [x] Single-stage
- [x] Multi-stage
      ref: https://www.leadwerks.com/community/blogs/entry/2403-single-file-spir-v-shaders-for-vulkan/
      note: Currently Untested, but should work
```

```md
- `src/cvulkan/shader.c` and `src/cvulkan/shader.h`
- [ ] **Review shader compilation and loading**
  - [ ] Verify SPIR-V shader loading
  - [ ] Check shader module creation
  - [ ] Test shader compilation error handling
  - [ ] Ensure proper shader cleanup
```

```md
- [ ] **Test multi-stage shader support**
  - [ ] Verify vertex-fragment shader combination
  - [ ] Check shader stage linking
  - [ ] Test shader stage validation
  - [ ] Ensure proper stage management
```

```md
- [ ] **Shader Example**
  - [ ] Single-stage shader
  - [ ] Multi-stage shader
  - [ ] Shader compilation errors
  - [ ] Shader hot-reloading
```

```md
- [ ] **Documentation updates**
  - [ ] Update shader.h documentation
  - [ ] Add shader compilation examples
```

