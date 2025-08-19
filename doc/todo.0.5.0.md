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
```

```md
- `src/cvulkan/surface.h`
- [x] **Review surface.h**
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

