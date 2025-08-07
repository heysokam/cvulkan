## 0.5.0 - Surface & Swapchain
**Goal**: Surface creation, Swapchain management, and image view handling.

```md
# VkSurfaceKHR
- [x] Accept a Surface handle from the user
- [x] Destroy helper for naming consistency
```
```md
## Swapchain
- [x] Context creation
- [x] Swapchain.Images Request
- [x] Swapchain.ImageViews Request
- [ ] Swapchain.recreate // Almost done.  FIX: Triggers a validation error on recreation.
```

```md
- `src/cvulkan/surface.h`
- [ ] **Review surface creation (system-agnostic)**
  - [ ] Verify surface handle validation
  - [ ] Check surface capability querying
  - [ ] Test surface format enumeration
  - [ ] Ensure proper surface cleanup
```

```md
- `src/cvulkan/device/swapchain.c`
- [ ] **Review swapchain creation and management**
  - [ ] Verify swapchain creation with optimal settings
  - [ ] Check swapchain image count optimization
  - [ ] Test swapchain format selection
  - [ ] Ensure proper swapchain recreation
- [ ] **Review swapchain image views**
  - [ ] Verify image view creation for all swapchain images
  - [ ] Check image view format compatibility
  - [ ] Test image view lifecycle management
  - [ ] Ensure proper image view cleanup
```

```md
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

