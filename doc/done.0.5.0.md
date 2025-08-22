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
- [x] **Review swapchain creation and management**
  - [x] Verify swapchain creation with optimal settings
  - [x] Check swapchain image count optimization
  - [x] Test swapchain format selection
- [x] **Review swapchain image views**
  - [x] Verify image view creation for all swapchain images
  - [x] Check image view format compatibility
  - [x] Test image view lifecycle management
  - [x] Ensure proper image view cleanup
```

