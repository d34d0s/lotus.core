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

typedef struct lotus_window{
    char title[256];       // Window title
    int location[2];       // Window x, y position
    int size[2];           // Window width and height
    float aspect_ratio;    // Aspect ratio (width / height)
    void* gl_context;      // Pointer to an OpenGL context
    void* internal_data;   // Pointer to platform-specific window data
} lotus_window;

// Platform Initialization
lotus_platform_state* platform_init(void);
void platform_exit(void);

// Window Management
lotus_window platform_create_window(const char* title, int width, int height);
void platform_destroy_window(lotus_window* window);

// OpenGL Context Management
bool platform_create_gl_context(lotus_window* window);
void platform_destroy_gl_context(lotus_window* window);

// Platform events
ubyte platform_pump(void);

// Platform Time
double platform_get_time(void);  // Returns time in seconds
void platform_sleep(f64 seconds);  // Sleep for the specified duration

// Swap Buffers
void platform_swap_buffers(lotus_window* window);
