#include "../include/platform/lotus_platform.h"

#if defined(LOTUS_PLATFORM_MACOS)

typedef struct Platform_Window_Data {
    void* state;
} Platform_Window_Data;

Lotus_Platform_State* lotus_platform_init(void) {}

void lotus_platform_exit(void) {}

Lotus_Window lotus_platform_make_window(char* title, sbyte4 x, sbyte4 y, sbyte4 w, sbyte4 h) {}

void lotus_platform_destroy_window(Lotus_Window* window) {}

f64 lotus_platform_get_time(void) {}

void lotus_platform_sleep(ubyte8 ms) {}

ubyte lotus_platform_pump(void) {}

ubyte lotus_platform_make_glcontext(Lotus_Window* window) {}

void lotus_platform_swap_buffers(Lotus_Window* window) {}

#endif
