//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
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


inline cvk_Pure float cvk_min_f32 (
  float const val,
  float const m
) {
  return (val < m) ? m : val;
}

inline cvk_Pure float cvk_max_f32 (
  float const val,
  float const M
) {
  return (val > M) ? M : val;
}

inline cvk_Pure float cvk_clamp_f32 (
  float const val,
  float const m,
  float const M
) {
  return cvk_max_f32(cvk_min_f32(val, m), M);
}

