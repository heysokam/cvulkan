## 0.7.0 - Static Rendering Elements
**Goal**: RenderPass setup, Framebuffer management.

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
- `src/cvulkan/rendering/renderpass.c` and `src/cvulkan/rendering/renderpass.h`
- [ ] **RenderPass setup**
  - [ ] Verify render pass creation with attachments
  - [ ] Check render pass subpass setup
  - [ ] Test render pass dependency configuration
  - [ ] Ensure proper render pass cleanup
```

```md
- `src/cvulkan/rendering/framebuffer.c` and `src/cvulkan/rendering/framebuffer.h`
- [ ] **Framebuffer creation**
  - [ ] Verify framebuffer creation with attachments
  - [ ] Check framebuffer attachment binding
  - [ ] Test framebuffer size validation
  - [ ] Ensure proper framebuffer cleanup
```

```md
- [ ] **Documentation updates**
  - [ ] Update rendering.h documentation
  - [ ] Document render pass patterns
  - [ ] **Static Rendering example**
    - [ ] Example with basic triangle rendering
    - [ ] Example with texture rendering
    - [ ] Example with multiple render targets
    - [ ] Example with render pass dependencies
```

