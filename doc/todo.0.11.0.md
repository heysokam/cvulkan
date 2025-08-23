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

- [ ] **Review image views and samplers**
  - [ ] Verify image view creation for different formats
  - [ ] Check sampler creation with various parameters
  - [ ] Test image view and sampler binding
  - [ ] Ensure proper resource cleanup

- [ ] **Review image transitions and layouts**
  - [ ] Verify image layout transitions
  - [ ] Check image layout optimization
  - [ ] Test image layout synchronization
  - [ ] Ensure proper transition handling

- [ ] **Review image operations**
  - [ ] Verify buffer-to-image copy operations
  - [ ] Check image-to-buffer copy operations
  - [ ] Test image blitting operations
  - [ ] Ensure proper operation synchronization
```

