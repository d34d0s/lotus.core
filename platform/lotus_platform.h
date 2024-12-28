#pragma once

#include "../types.h"
#include "../defines.h"

#include "lotus_event.h"
#include "lotus_input.h"

typedef enum lotus_platform_tag {
    LOTUS_WINDOWS_TAG,
    LOTUS_MACOS_TAG,
    LOTUS_PLATFORM_TAGS
} lotus_platform_tag;

typedef struct lotus_dll {
    char* name;
    void* module;
} lotus_dll;

typedef struct lotus_platform_state {
    void* state;
    f64 clock_frequency;
    lotus_platform_tag platform;
    lotus_event_state* event_state;
    lotus_input_state* input_state;
} lotus_platform_state;

typedef struct lotus_window {
    char title[256];
    void* state;
    ubyte4 size[2];
    ubyte4 location[2];
} lotus_window;

lotus_platform_state* lotus_get_state_ptr_platform(void);

lotus_platform_state* lotus_platform_init(void);
void lotus_platform_exit(void);

lotus_window lotus_platform_make_window(char* title, sbyte4 x, sbyte4 y, sbyte4 w, sbyte4 h);
void lotus_platform_destroy_window(lotus_window* window);

f64 lotus_platform_get_time(void);
void lotus_platform_sleep(ubyte8 ms);
ubyte lotus_platform_pump(void);

ubyte lotus_platform_make_glcontext(lotus_window* window);
void lotus_platform_swap_buffers(lotus_window* window);
