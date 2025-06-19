//:_________________________________________________________________
//  cstd  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
#ifndef H_cstd
#define H_cstd
// @deps std
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


//______________________________________
// @section Primitive Types
//____________________________
#if __STDC_VERSION__ < 202311L
typedef enum std_bool { std_false = 0, std_true } std_bool;
#endif

#ifndef std_pointer
#define std_pointer void*
#endif//std_pointer

//______________________________________
// @section Aliases: Generic Slices
//____________________________
typedef struct std_Slice std_Slice;
struct std_Slice {
  size_t len;
  void*  ptr;
};


//______________________________________
// @section Aliases: Strings
//____________________________

typedef char const* std_String;
typedef std_String* std_StringList;
#define std_String_equal(A,B) strcmp((A),(B)) == 0
#define std_String_equal_caseInsensitive(A,B) strcmpi((A),(B)) == 0
#define std_String_add(A,B) strcat((A),(B))
#define std_String_len(A) strlen((A))



//______________________________________
// @section Aliases: General Purpose Tools
//____________________________

#ifndef std_discard
#define std_discard(arg) (void)((arg))
#endif//std_discard

#ifndef std_exit
#define std_exit(code) exit((int)code)
#endif//std_exit

#ifndef std_assert
#include <assert.h>
#define std_assert(condition) assert(condition)
#endif//std_assert

#ifndef std_print
#include <stdio.h>
#define std_print printf
#endif//std_print


//______________________________________
// @section Aliases: Memory Management Tools
//____________________________

#ifndef std_malloc
#define std_malloc(T,len) (T)malloc((len))
#endif//std_malloc

#ifndef std_free
#define std_free(P) free((P))
#endif//std_free



//______________________________________
// @section Aliases: Compiler Pragmas
//____________________________

/// @description
/// Mark the last entry of an enum such that the enum's size is forced to fit inside an `int32_t` type
#define std_enum_Force32 0x7FFFFFFF

#if !defined std_pragma_Pure
/// @description
/// Inform the compiler that the marked function must have no side effects
#define std_pragma_Pure __attribute__((const))
#endif

#if !defined std_pragma_NoReturn
/// @description
/// Inform the compiler that the marked function will never return
#define std_pragma_NoReturn [[noreturn]]
#endif

#if !defined std_pragma_MayNotReturn
/// @description
/// Information marker for functions that may potentially not return in one of their branches
#define std_pragma_MayNotReturn /** May Not Return */
#endif

#if !defined std_pragma_Nullable
/// @description
/// Information marker for values that can potentially be null
#define std_pragma_Nullable /** Nullable */
#endif

/// @description
/// Information marker for values that only allow accept read access after assignment
#if !defined std_pragma_Readonly
#define std_pragma_Readonly /** ReadOnly */
#endif // std_pragma_Readonly


#endif  // H_cstd

