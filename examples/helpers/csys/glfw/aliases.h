//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
// GLFW: Aliases
#ifndef H_csys_glfw_aliases
#define H_csys_glfw_aliases
#include "./base.h"

//____________________________
// General Tools
#define glfw_init                    glfwInit
#define glfw_term                    glfwTerminate
#define glfw_sync                    glfwPollEvents

//____________________________
// Window Tools
#define glfw_Window                  GLFWwindow
#define glfw_Window_hint             glfwWindowHint
#define glfw_ClientApi               GLFW_CLIENT_API
#define glfw_NoApi                   GLFW_NO_API
#define glfw_Resizable               GLFW_RESIZABLE
#define glfw_Window_close            glfwWindowShouldClose
#define glfw_Window_create           glfwCreateWindow
#define glfw_Window_destroy          glfwDestroyWindow
#define glfw_Window_setClose         glfwSetWindowShouldClose
#define glfw_Window_setUserdata      glfwSetWindowUserPointer

//____________________________
// Callbacks
#define glfw_Fn_Error                GLFWerrorfun
#define glfw_callback_setError       glfwSetErrorCallback
#define glfw_Fn_Resize               GLFWframebuffersizefun
#define glfw_callback_setResize      glfwSetFramebufferSizeCallback
#define glfw_Fn_Key                  GLFWkeyfun
#define glfw_callback_setKey         glfwSetKeyCallback
#define glfw_Fn_MousePos             GLFWcursorposfun
#define glfw_callback_setMousePos    glfwSetCursorPosCallback
#define glfw_Fn_MouseBtn             GLFWmousebuttonfun
#define glfw_callback_setMouseBtn    glfwSetMouseButtonCallback
#define glfw_Fn_MouseScroll          GLFWscrollfun
#define glfw_callback_setMouseScroll glfwSetScrollCallback

//____________________________
// Keys
typedef enum glfw_Action {
  glfw_Action_Press   = GLFW_PRESS,
  glfw_Action_Release = GLFW_RELEASE,
  glfw_Action_Repeat  = GLFW_REPEAT,
  glfw_Action_Force32 = 0x7FFFFFFF
} glfw_Action;
typedef enum glfw_Key {
  glfw_Key_Esc = GLFW_KEY_ESCAPE,
} glfw_Key;

#endif  //H_csys_glfw_aliases
