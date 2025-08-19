## 0.3.0 - Logical Device
**Goal**: Logical device creation and device feature enabling.

```md
# Device
## Logical
- [x] Minimal creation
- [x] Extensions
  - [x] Minimal
  - [x] Custom
- [x] Features
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
- [x] **Review feature checking**
  - [x] Verify feature structure population
  - [x] Check feature availability validation
  - [x] Test feature requirement checking
  - [x] Ensure proper feature enablement
```

```md
- `src/cvulkan/device/logical.c`
- [x] **Review device feature enabling**
  - [x] Check feature enablement during device creation
  - [x] Test feature availability after device creation
  - [x] Ensure proper feature validation
- [x] **Logical device creation**
  - [x] Verify device creation with all required features
  - [x] Check device extension enablement
  - [x] Test device creation error handling
  - [x] Ensure proper device cleanup
```

