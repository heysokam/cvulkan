## Code Formatting
Code must be formatted using the @[clang-format](./.clang-format) file provided by the project.  
Calling clang-format on any file of the project will use the formatting file by default.  

## Coding Style
This project treats namespaces as categories for code organization.  

Global State is strictly forbidden.  
Functions must be pure, unless unavoidable _(eg: IO operations, etc)_.  
Function arguments must be immutable.  
Must be avoided:
- Encapsulation  
- Hidden/Private object members  
  _(must be public and documented for clarity of intent)_
- Hidden control flow
- Macros

## Naming convention
### Namespaces
- The global namespace is `cvk`  
- Namespaces MUST be separated by `_`  
  `cvk_thing()`  
  `cvk_many_more_namespaces_TypeName()`  
- Qualifiers on actions are treated as namespaces  
  `MyType_someAction()`, `MyType_someAction_one()`, `MyType_someAction_other()`  

### Symbols
- Names MUST use SOV order  _(english is SVO)_
  `cvk_object_verb` instead of `cvk_verbObject`
  _(eg: `vkCreateInstance` would be `vk_instance_create` under this rule)_
- Functions and variables are `camelCase`  
  `int myVar = 0;`  
- Typedefs and Comptime Constants are `PascalCase` for their name
  `typedef int Thing;`  
  `#define Thing_Max 42`  
  `int const Thing_Max = 42;`
  `#define cvk_some_namespace_Thing 42`
- Namespaced symbols preserve the case of their symbol, but not for their namespace 
  `typedef int cvk_MyType;`  
  `typedef int cvk_other_namespaced_TypeName;`  
- Actions for types should only preserve `PascalCase` for the typename  
  `int cvk_MyType_actionName();`  
  `int cvk_other_actionName();`  
- Nested Types become namespaces and are treated by namespace rules
  `typedef int cvk_Thing;`
  `typedef int cvk_thing_SubType;`

### Vulkan Renames
- `CreateInfo`          -> `Options`
- `DebugUtilsMessenger` -> `Debug`

