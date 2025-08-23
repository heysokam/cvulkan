## 0.9.0 - Graphics Pipeline
**Goal**:
Pipeline.State management, Pipeline.Layout creation, graphics.Pipeline creation,
end-to-end rendering examples and validate the complete graphics pipeline.


```md
# Pipeline
## State
- [x] Minimal Creation
- [x] Customizable Options
- [x] Sane Defaults
## Layout
- [x] Minimal Creation
- [x] Customizable Options
## Context: Graphics
- [x] Minimal Creation: Single Pipeline
- [x] Customizable Options
- [x] Won't support. The Pipeline.create function is a tiny wrapper, and really easy to reimplement.
  - [_] Sane Defaults when `NULL`
  - [_] Multi-pipeline
```

```md
- `src/cvulkan/pipeline/state.c`
- [x] **Review pipeline state management**
  - [x] Verify pipeline state structure population
  - [x] Test pipeline state customization
  - [x] Ensure proper state defaults
```

```md
- `src/cvulkan/pipeline/layout.c`
- [x] **Review pipeline layout creation**
  - [x] Verify descriptor set layout creation
  - [x] Check push constant range setup
```

```md
- `src/cvulkan/pipeline/graphics.c`
- [x] **Review graphics pipeline creation**
  - [x] Verify pipeline creation with all stages
  - [x] Check pipeline state validation
  - [x] Test pipeline customization options
  - [x] Ensure proper pipeline cleanup
  - [x] Verify complete rendering flow
```

