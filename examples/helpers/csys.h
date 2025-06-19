//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
#ifndef H_csys
#define H_csys
#include <cstd.h>
#include "./csys/backend.h"
#include "./csys/glfw/core.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpre-c23-compat"

//______________________________________
// @section Window
//____________________________

typedef struct csys_Window {
  csys_backend_Window* ct;
  uint32_t             width;
  uint32_t             height;
  std_String           title;
} csys_Window;

typedef struct csys_Window_init_Options {
  uint32_t       width;
  uint32_t       height;
  std_String     title;
  glfw_Fn_Resize cb_resize;
  void*          userdata;
  bool           resize;
  char           priv_pad[7];
} csys_Window_init_Options;
std_pragma_Pure csys_Window csys_Window_init (csys_Window_init_Options config);
void                        csys_Window_term (csys_Window* const window);
std_pragma_Pure bool        csys_Window_close (csys_Window const* const sys);


//______________________________________
// @section Input
//____________________________

typedef struct csys_Input {
  char handle;
  char priv_pad[7];
} csys_Input;

typedef struct csys_Input_init_Options {
  glfw_Fn_Key         cb_key;
  glfw_Fn_MousePos    cb_mousePos;
  glfw_Fn_MouseBtn    cb_mouseBtn;
  glfw_Fn_MouseScroll cb_mouseScroll;
} csys_Input_init_Options;
std_pragma_Pure csys_Input csys_Input_init (csys_Input_init_Options in, csys_Window const* const window);


//______________________________________
// @section Core
//____________________________

typedef struct csys_System {
  csys_Window window;
  csys_Input  input;
} csys_System;

typedef struct csys_init_Options {
  csys_Fn_Error            cb_error;
  csys_Window_init_Options window;
  csys_Input_init_Options  input;
} csys_init_Options;
std_pragma_Pure csys_init_Options csys_init_Options_defaults ();
std_pragma_Pure csys_System       csys_init (csys_init_Options config);
std_pragma_Pure bool              csys_close (csys_System const* const sys);
void                              csys_term (csys_System* const sys);
void                              csys_update (csys_System* const sys);


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef csys_Implementation
#define csys_Implementation_callbacks
#define csys_Implementation_window
#define csys_Implementation_input
#endif  // csys_Implementation
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef csys_Implementation_callbacks

void csys_Callback_error (int code, char const* descr);
void csys_Callback_resize (glfw_Window* window, int W, int H);
void csys_Callback_key (glfw_Window* win, int key, int code, int action, int mods);


void csys_Callback_error (
  int         code,
  char const* descr
) {
  std_discard(code);
  std_discard(descr);
}

void csys_Callback_resize (
  glfw_Window* window,
  int          W,
  int          H
) {
  std_discard(window);
  std_discard(W);
  std_discard(H);
}

void csys_Callback_key (
  glfw_Window* win,
  int          key,
  int          code,
  int          action,
  int          mods
) {  // clang-format off
  std_discard(win); std_discard(code); std_discard(mods);
  if (key == glfw_Key_Esc && action == glfw_Action_Press) glfw_Window_setClose(win, true);
}
  // clang-format on

#endif  // csys_Implementation_callbacks
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef csys_Implementation_window

csys_Window csys_Window_init (
  csys_Window_init_Options in
) {
  glfw_init();
  glfw_Window_hint(glfw_ClientApi, glfw_NoApi);
  glfw_Window_hint(glfw_Resizable, in.resize);
  csys_Window result = {
    .ct     = glfw_Window_create((int)in.width, (int)in.height, in.title, NULL, NULL),
    .width  = in.width,
    .height = in.height,
    .title  = in.title,
  };
  // TODO:
  // if (!result.ct) { in.error("csys_Window_init could not to create the GLFW window"); }
  glfw_Window_setUserdata(result.ct, in.userdata);
  glfw_callback_setResize(result.ct, in.cb_resize);
  return result;
}
void csys_Window_term (
  csys_Window* const window
) {
  glfw_Window_destroy(window->ct);
}

std_pragma_Pure bool csys_Window_close (
  csys_Window const* const window
) {
  return glfw_Window_close(window->ct);
}

#endif  // csys_Implementation_window
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef csys_Implementation_input

csys_Input csys_Input_init (
  csys_Input_init_Options  in,
  csys_Window const* const window
) {
  glfw_callback_setKey(window->ct, in.cb_key);
  glfw_callback_setMousePos(window->ct, in.cb_mousePos);
  glfw_callback_setMouseBtn(window->ct, in.cb_mouseBtn);
  glfw_callback_setMouseScroll(window->ct, in.cb_mouseScroll);
  return (csys_Input){
    .handle = csys_backend_Input_handle(),
  };
}

#endif  // csys_Implementation_input
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#ifdef csys_Implementation

std_pragma_Pure bool csys_close (
  csys_System const* const sys
) {
  return csys_Window_close(&sys->window);
}

#ifdef csys_Implementation_callbacks
std_pragma_Pure csys_init_Options csys_init_Options_defaults () {
  // clang-format off
  return (csys_init_Options){
    .cb_error         = csys_Callback_error,
    .input            = (csys_Input_init_Options){
      .cb_key         = csys_Callback_key,
      .cb_mouseBtn    = NULL,
      .cb_mousePos    = NULL,
      .cb_mouseScroll = NULL,
    },
    .window           = (csys_Window_init_Options){
      .title          = "csys_Window",
      .width          = 960,
      .height         = 540,
      .resize         = false,
      .cb_resize      = csys_Callback_resize,
    }
  };    // clang-format on
}
#endif  // csys_Implementation_callbacks

std_pragma_Pure csys_System csys_init (
  csys_init_Options in
) {
  glfw_callback_setError(in.cb_error);
  csys_backend_init();
  csys_System result;
  result.window = csys_Window_init(in.window);
  result.input  = csys_Input_init(in.input, &result.window);
  return result;
}

void csys_term (
  csys_System* const sys
) {
  csys_Window_term(&sys->window);
  csys_backend_term();
}

void csys_update (
  csys_System* const sys
) {
  std_discard(sys);
  glfw_sync();
}

#endif  // csys_Implementation
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#pragma GCC diagnostic pop // ignored "-Wpre-c23-compat"
#endif  // H_csys

