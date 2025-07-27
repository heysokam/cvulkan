//:_________________________________________________________
//  ctime  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:_________________________________________________________
#include "./ctime.h"

ctime_Time ctime_time_defaults () {
  return (ctime_Time){
    .data    = (std_Time){ 0 },
    .last    = 0,
    .current = 0,
  };
}

#if defined _WIN32
static ctime_Time ctime_time_start_win () {
  ctime_Time result = ctime_time_defaults();
  QueryPerformanceFrequency(&result.data);
  return result;
}

#elif defined __APPLE__
static ctime_Time ctime_time_start_mac () {
  ctime_Time result = ctime_time_defaults();
  mach_timebase_info(&result.data);
  return result;
}

#else  // posix
static ctime_Time ctime_time_start_lnx () {
  ctime_Time result = ctime_time_defaults();
  clock_getres(NS_CLOCK_ID, &result.data);
  return result;
}
#endif

ctime_Time ctime_start () {
  ctime_Time result = (ctime_Time){ 0 };
#if defined _WIN32
  result = ctime_time_start_win();
#elif defined __APPLE__
  result = ctime_time_start_win();
#elif defined __unix__
  result = ctime_time_start_lnx();
#else
#error "Could not start the Monotonic Clock"
#endif
  result.start = ctime_nsec(&result);
  return result;
}

#if defined _WIN32
static ctime_Nanosecs ctime_time_nsec_win (
  ctime_Time* const clock
) {
  std_Time time;
  QueryPerformanceCounter(&time);
  clock->last    = clock->current;
  clock->current = (ctime_Nanosecs)((1.0e9 * time.QuadPart) / clock->data.QuadPart);
  return clock->current;
}

#elif defined __APPLE__
static ctime_Nanosecs ctime_time_nsec_mac (
  ctime_Time* const clock
) {
  clock->last    = clock->current;
  clock->current = mach_absolute_time();
  clock->current *= clock->data.numer;
  clock->current /= clock->data.denom;
  return clock->current;
}

#else  // posix
static ctime_Nanosecs ctime_time_nsec_lnx (
  ctime_Time* const clock
) {
  std_Time time;
  clock->last = clock->current;
  clock_gettime(NS_CLOCK_ID, &time);
  clock->current = (ctime_Nanosecs)time.tv_sec * (ctime_Nanosecs)1.0e9 + (ctime_Nanosecs)time.tv_nsec;
  return clock->current;
}
#endif

ctime_Nanosecs ctime_nsec (
  ctime_Time* const clock
) {
#if defined _WIN32
  return ctime_time_nsec_win(clock);
#elif defined __APPLE__
  return ctime_time_nsec_mac(clock);
#elif defined __unix__
  return ctime_time_nsec_lnx(clock);
#else
#error "Monotonic Clock not supported on this platform"
#endif
}

