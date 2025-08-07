## 0.2.0 - Physical Device Selection
**Goal**: Physical device enumeration, feature checking, and queue family selection.

```md
# Device
- [ ] Extensions
  - [x] Minimal
  - [ ] Custom
- [ ] Features
## Physical
- [x] Minimal creation
      Using `isSuitable` and surface support
- [ ] Suitability
  - [x] Boolean: Overridable `device_physical_isSuitable` function
        Returns true/false for whether the device is valid or not
  - [ ] Ranked : Overridable `device_physical_rank` function
        Track scores and pick the best
```

```md
- `src/cvulkan/device/features.c`
- [ ] **Review feature checking**
  - [ ] Verify feature structure population
  - [ ] Check feature availability validation
  - [ ] Test feature requirement checking
  - [ ] Ensure proper feature enablement
```

```md
- `src/cvulkan/device/extensions.c`
- [ ] **Review device extension support**
  - [ ] Verify device extension enumeration
  - [ ] Check device extension availability
  - [ ] Test device extension enablement
  - [ ] Ensure proper extension validation
```

```md
- `src/cvulkan/device/physical.c`
- [ ] **Physical Device enumeration**
  - [ ] Verify device enumeration covers all available devices
  - [ ] Check device property retrieval is complete
  - [ ] Test device capability reporting
  - [ ] Ensure proper device ranking/selection logic
- [ ] **Queue Family selection**
  - [ ] Verify queue family enumeration
  - [ ] Check queue family property validation
  - [ ] Test queue family selection logic
  - [ ] Ensure proper queue family assignment
```

```md
- [ ] **Documentation updates**
  - [ ] Update device.h documentation
  - [ ] Add device selection examples
  - [ ] Document feature checking patterns
  - [ ] **Device selection example**
    - [ ] Automatic device selection
    - [ ] Manual device selection
    - [ ] Specific feature requirements
    - [ ] Multiple device support
```
