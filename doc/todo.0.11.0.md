## 0.11.0 - Images & Textures
**Goal**: Image creation and operations.

```md
# Data
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
- `src/cvulkan/data/image.c`
- [x] **Review image creation and management**
  - [x] Verify image creation
  - [x] Test image memory binding
  - [x] Ensure proper image cleanup

- [ ] **Review image views**
  - [ ] Verify image view creation for different formats
  - [x] Test image view binding
  - [ ] Ensure proper resource cleanup

- [ ] **Review image samplers**
  - [ ] Check sampler creation with various parameters
  - [x] Test image sampler binding
  - [ ] Ensure proper resource cleanup
```

