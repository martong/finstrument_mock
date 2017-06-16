#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void _clear_function_substitutions();
void _substitute_function(const char* src, const char* dst);

#ifdef __cplusplus
} // extern "C"
#endif
