//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
//! @fileoverview GLFW: Backend API for csys
#ifndef H_csys_glfw_backend_api
#define H_csys_glfw_backend_api

#define csys_Fn_Error               glfw_Fn_Error
#define csys_backend_Window         glfw_Window
#define csys_backend_init           glfw_init
#define csys_backend_term           glfw_term
#define csys_backend_Input_handle() 255  // Dummy handle for GLFW

#endif// H_csys_glfw_backend_api
