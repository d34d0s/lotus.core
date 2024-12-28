#include "../lotus_platform.h"

#if defined(LOTUS_PLATFORM_MACOS)

typedef struct _lotus_window_macos {
    void* state;
} _lotus_window_macos;

lotus_platform_state* lotus_platform_init(void) {}

void lotus_platform_exit(void) {}

lotus_window lotus_platform_make_window(char* title, sbyte4 x, sbyte4 y, sbyte4 w, sbyte4 h) {}

void lotus_platform_destroy_window(lotus_window* window) {}

f64 lotus_platform_get_time(void) {}

void lotus_platform_sleep(ubyte8 ms) {}

ubyte lotus_platform_pump(void) {}

ubyte lotus_platform_make_glcontext(lotus_window* window) {}

void lotus_platform_swap_buffers(lotus_window* window) {}

#endif
