//:_________________________________________________________
//  ctime  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:_________________________________________________________
//! @fileoverview Cross-platform Monotonic timer
//_______________________________________________|
#ifndef H_ctime
#define H_ctime


//______________________________________
// @section stdlib: Monotonic Clock Dependencies
//____________________________
#include <stdint.h>

//__________________
// Mac
#if defined __APPLE__
#include <mach/mach_time.h>
typedef mach_timebase_info_data_t std_Time;

//__________________
// Windows
#elif defined _WIN32
#include <windows.h>
typedef LARGE_INTEGER std_Time;

//__________________
// Linux & BSD
#elif defined __unix__
#ifdef _POSIX_C_SOURCE
#define NS_OLD_POSIX_C_SOURCE _POSIX_C_SOURCE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#undef _POSIX_C_SOURCE
#pragma GCC diagnostic pop  // -Wreserved-macro-identifier
#endif

  // clang-format off
#define _POSIX_C_SOURCE 199309L
  // clang-format on

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#define NS_DEFINED__USE_POSIX199309
#endif

#include <time.h>
#ifdef NS_DEFINED__USE_POSIX199309
#undef __USE_POSIX199309
#endif
#pragma GCC diagnostic pop  // "-Wreserved-macro-identifier"

#ifdef NS_OLD_POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE NS_OLD_POSIX_C_SOURCE
#endif
#ifdef CLOCK_MONOTONIC
#define NS_CLOCK_ID CLOCK_MONOTONIC
#else
#define NS_CLOCK_ID CLOCK_REALTIME
#endif
typedef struct timespec std_Time;
#else  // unknown
#error "Monotonic Clock not supported on this platform"
#endif


//______________________________________
// @section Time Management Tools
//____________________________

/// Nanoseconds Data
typedef uint64_t ctime_Nanosecs;
/// Monotonic Clock Data
typedef struct ctime_Time {
  std_Time       data;
  ctime_Nanosecs last;
  ctime_Nanosecs current;
  ctime_Nanosecs start;  ///< Readonly
} ctime_Time;

ctime_Time     ctime_time_defaults ();
ctime_Time     ctime_start ();
ctime_Nanosecs ctime_nsec (ctime_Time* const clock);


//______________________________________
// @section Single Header Support
//____________________________
#ifdef ctime_Implementation
#define ctime_Implementation
#endif
#ifdef ctime_Implementation
#include "./ctime.c"
#endif


#endif  // H_ctime

