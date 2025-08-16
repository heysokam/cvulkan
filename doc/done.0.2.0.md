## 0.2.0 - Physical Device Selection
**Goal**: Physical device enumeration, feature checking, and queue family selection.

```md
# Device
## Physical
- [x] Minimal creation
      Using `isSuitable` and surface support
- [x] Suitability
  - [x] Boolean: Overridable `device_physical_isSuitable` function
        Returns true/false for whether the device is valid or not
  - [x] Ranked :
    - [x] Overridable `device_physical_getScore` function
    - [x] Track scores and pick the best
    - [x] Pick the first of the bests
```

```md
- `src/cvulkan/device/physical.c`
- [x] **Physical Device enumeration**
  - [x] Verify device enumeration covers all available devices
  - [x] Check device property retrieval is complete
  - [x] Ensure proper device ranking/selection logic
- [x] **Queue Family selection**
  - [x] Verify queue family enumeration
  - [x] Check queue family property validation
  - [x] Test queue family selection logic
  - [x] Ensure proper queue family assignment
- [x] **Swapchain Support**
```

