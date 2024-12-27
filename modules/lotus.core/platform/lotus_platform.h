#pragma once

#include "../types.h"
#include "../defines.h"

typedef struct lotus_dll {
    char* name;
    void* module;
} lotus_dll;

typedef struct lotus_platform_state {
    ubyte platform;
    void* state;
} lotus_platform_state;

typedef struct lotus_window {
    char title[256];
    void* state;
    ubyte4 size[2];
    ubyte4 location[2];
} lotus_window;

ubyte lotus_platform_init(lotus_platform_state* state);
void lotus_platform_exit(lotus_platform_state* state);

lotus_window lotus_platform_make_window(char* title, sbyte4 x, sbyte4 y, sbyte4 w, sbyte4 h);
ubyte lotus_platform_make_glcontext(lotus_window* window);
void lotus_platform_swap_buffers(lotus_window* window);
