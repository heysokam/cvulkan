//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
#ifndef H_csys_backend
#define H_csys_backend

#ifdef csys_GLFW
#include "./glfw/core.h"
#else
#define csys_GLFW
#include "./glfw/core.h"
#endif // csys_GLFW

#endif // H_csys_backend
