## 0.3.0 - Logical Device
**Goal**: Logical device creation and device feature enabling.

```md
# Device
## Logical
- [x] Minimal creation
- [x] Extensions
  - [x] Minimal
  - [x] Custom
- [ ] Features
- [ ] Support Multi-Queue creation
```

```md
- `src/cvulkan/device/extensions.c`
- [x] **Review device extension support**
  - [x] Verify device extension enumeration
  - [x] Check device extension availability
  - [x] Test device extension enablement
  - [x] Ensure proper extension validation
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
- `src/cvulkan/device/logical.c`
- [ ] **Review device feature enabling**
  - [ ] Verify feature structure population
  - [ ] Check feature enablement during device creation
  - [ ] Test feature availability after device creation
  - [ ] Ensure proper feature validation
- [ ] **Logical device creation**
  - [ ] Verify device creation with all required features
  - [x] Check device extension enablement
  - [ ] Test device creation error handling
  - [ ] Ensure proper device cleanup
```

