## 0.9.0 - Graphics Pipeline
**Goal**:
Pipeline.State management, Pipeline.Layout creation, graphics.Pipeline creation,
end-to-end rendering examples and validate the complete graphics pipeline.


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
- [x] Customizable Options
- [x] Won't support. The Pipeline.create function is a tiny wrapper, and really easy to reimplement.
  - [_] Sane Defaults when `NULL`
  - [_] Multi-pipeline
```

```md
- `src/cvulkan/pipeline/state.c`
- [ ] **Review pipeline state management**
  - [ ] Verify pipeline state structure population
  - [ ] Check pipeline state validation
  - [ ] Test pipeline state customization
  - [ ] Ensure proper state defaults
```

```md
- `src/cvulkan/pipeline/layout.c`
- [ ] **Review pipeline layout creation**
  - [ ] Verify descriptor set layout creation
  - [ ] Check push constant range setup
  - [ ] Test pipeline layout binding
  - [ ] Ensure proper layout cleanup
```

```md
- `src/cvulkan/pipeline/graphics.c`
- [ ] **Review graphics pipeline creation**
  - [ ] Verify pipeline creation with all stages
  - [ ] Check pipeline state validation
  - [ ] Test pipeline customization options
  - [ ] Ensure proper pipeline cleanup
- [ ] **Test end-to-end rendering pipeline**
  - [ ] Verify complete rendering flow
  - [ ] Check rendering synchronization
  - [ ] Test rendering error handling
  - [ ] Ensure proper resource management
```

```md
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

