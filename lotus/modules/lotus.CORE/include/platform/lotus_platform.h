#pragma once

#include "../types.h"
#include "../defines.h"

#include "lotus_event.h"
#include "lotus_input.h"

typedef enum Lotus_Platform_Tag {
    LOTUS_WINDOWS_TAG,
    LOTUS_MACOS_TAG,
    LOTUS_PLATFORM_TAGS
} Lotus_Platform_Tag;

typedef struct Lotus_Dll {
    char* name;
    void* module;
} Lotus_Dll;

typedef struct Lotus_Platform_State {
    void* state;
    f64 clock_frequency;
    Lotus_Platform_Tag platform;
    Lotus_Event_State* event_state;
    Lotus_Input_State* input_state;
} Lotus_Platform_State;

typedef struct Lotus_Window{
    char title[256];       // Window title
    int location[2];       // Window x, y position
    int size[2];           // Window width and height
    float aspect_ratio;    // Aspect ratio (width / height)
    void* gl_context;      // Pointer to an OpenGL context
    void* internal_data;   // Pointer to platform-specific window data
} Lotus_Window;

// Platform Initialization
Lotus_Platform_State* lotus_init_platform(void);
void lotus_exit_platform(void);

// Window Management
Lotus_Window lotus_create_platform_window(const char* title, int width, int height);
void lotus_destroy_platform_window(Lotus_Window* window);

// OpenGL Context Management
bool lotus_create_platform_gl_context(Lotus_Window* window);
void lotus_destroy_platform_gl_context(Lotus_Window* window);

// Platform events
ubyte lotus_pump_platform_messages(void);

// Platform Time
double lotus_get_platform_time(void);  // Returns time in seconds
void lotus_sleep_platform(f64 seconds);  // Sleep for the specified duration

// Swap Buffers
void lotus_swap_platform_buffers(Lotus_Window* window);
