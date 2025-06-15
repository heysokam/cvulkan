//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "./math.h"

inline cvk_Pure uint32_t cvk_min_u32 (
  uint32_t const val,
  uint32_t const m
) {
  return (val < m) ? m : val;
}

inline cvk_Pure uint32_t cvk_max_u32 (
  uint32_t const val,
  uint32_t const M
) {
  return (val > M) ? M : val;
}


inline cvk_Pure uint32_t cvk_clamp_u32 (
  uint32_t const val,
  uint32_t const m,
  uint32_t const M
) {
  return cvk_max_u32(cvk_min_u32(val, m), M);
}

