#include "../include/platform/lotus_platform.h"

#if defined(LOTUS_PLATFORM_MACOS)

typedef struct Platform_Window_Data {
    void* state;
} Platform_Window_Data;

static Lotus_Platform_API internal_platform_api = {0};

Lotus_Platform_API* lotus_init_platform(void) { return NULL; }

void lotus_platform_exit(void) { return; }

#endif
