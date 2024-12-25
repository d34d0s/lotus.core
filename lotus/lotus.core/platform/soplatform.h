#pragma once

#include "../types.h"
#include "../defines.h"

typedef struct sogl_dll {
    char* name;
    void* module;
} sogl_dll;

typedef struct sogl_platform_state {
    ubyte platform;
    void* state;
} sogl_platform_state;

typedef struct sogl_window {
    char title[256];
    void* state;
    ubyte4 size[2];
    ubyte4 location[2];
} sogl_window;

ubyte sogl_platform_init(sogl_platform_state* state);
void sogl_platform_exit(sogl_platform_state* state);

sogl_window sogl_platform_make_window(char* title, sbyte4 x, sbyte4 y, sbyte4 w, sbyte4 h);
ubyte sogl_platform_make_glcontext(sogl_window* window);
