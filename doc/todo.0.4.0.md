## 0.3.0 - Device Queue
**Goal**: Queue creation

```md
## Queue
- [x] Minimal creation (1 graphics+present Q)
- [ ] Arbitrary Queue creation (multi-queue)
      Needs support on the device.logical function
- [x] Wait
```

```md
- `src/cvulkan/device/queue.c`
- [ ] **Review queue creation and management**
  - [ ] Verify queue creation for all required families
  - [ ] Check queue property validation
  - [ ] Test queue submission and synchronization
  - [ ] Ensure proper queue lifecycle management
  - [ ] **Queue Example**
    - [ ] Graphics queue only
    - [ ] Compute queue
    - [ ] Transfer queue
    - [ ] Multiple queue families
```

