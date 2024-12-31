#include "../include/platform/lotus_platform.h"

#if defined(LOTUS_PLATFORM_MACOS)

#include "lotus_platform.h"
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#include <OpenGL/gl3.h>
#include <dlfcn.h>
#include <time.h>
#include <unistd.h>

// Forward declarations for internal structures
typedef struct {
    NSWindow* window;
    NSOpenGLContext* gl_context;
} Platform_Window_Data;

// Global platform state
static Lotus_Platform_State internal_platform_state = {0};

// Get platform state
static Lotus_Platform_State* _macos_get_state_impl(void) {
    return &internal_platform_state;
}

// Shutdown platform
static void _macos_shutdown_impl(void) {
    // Perform any necessary cleanup
}

// Get current time in seconds
static f64 _macos_get_time_impl(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (ts.tv_nsec / 1.0e9);
}

// Sleep for a given duration
static void _macos_sleep_impl(f64 seconds) {
    usleep((useconds_t)(seconds * 1e6));
}

// Load a dynamic library
static Lotus_DyLib _macos_load_library_impl(const char* path, const char* name) {
    Lotus_DyLib lib = {0};
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s.dylib", path, name);
    lib.handle = dlopen(full_path, RTLD_LAZY);
    if (!lib.handle) {
        printf("Failed to load library: %s\n", dlerror());
    }
    lib.name = name;
    return lib;
}

// Get a symbol from a dynamic library
static void* _macos_get_library_symbol_impl(Lotus_DyLib* library, const char* symbol_name) {
    return dlsym(library->handle, symbol_name);
}

// Unload a dynamic library
static ubyte _macos_unload_library_impl(Lotus_DyLib* library) {
    if (dlclose(library->handle) == 0) {
        library->handle = NULL;
        return 1;
    }
    return 0;
}

// Poll events
static ubyte _macos_poll_events_impl(void) {
    NSEvent* event;
    while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                       untilDate:nil
                                          inMode:NSDefaultRunLoopMode
                                         dequeue:YES])) {
        [NSApp sendEvent:event];
        [NSApp updateWindows];
    }
    return 1;
}

// Create a window
static Lotus_Window _macos_create_window_impl(const char* title, int width, int height) {
    @autoreleasepool {
        NSApplication* app = [NSApplication sharedApplication];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSRect frame = NSMakeRect(0, 0, width, height);
        NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                        styleMask:(NSWindowStyleMaskTitled |
                                                                   NSWindowStyleMaskClosable |
                                                                   NSWindowStyleMaskResizable)
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];
        [window setTitle:[NSString stringWithUTF8String:title]];
        [window makeKeyAndOrderFront:nil];
        [app activateIgnoringOtherApps:YES];

        Platform_Window_Data* data = malloc(sizeof(Platform_Window_Data));
        data->window = window;
        data->gl_context = NULL;

        Lotus_Window result = {0};
        strncpy(result.title, title, sizeof(result.title) - 1);
        result.size[0] = width;
        result.size[1] = height;
        result.aspect_ratio = (float)width / (float)height;
        result.internal_data = data;
        return result;
    }
}

// Destroy a window
static void _macos_destroy_window_impl(Lotus_Window* window) {
    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
    if (data) {
        [data->window close];
        free(data);
    }
    window->internal_data = NULL;
}

// Create OpenGL context
static ubyte _macos_create_gl_context_impl(Lotus_Window* window) {
    @autoreleasepool {
        Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
        if (!data) return 0;

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAStencilSize, 8,
            NSOpenGLPFADoubleBuffer,
            0
        };

        NSOpenGLPixelFormat* pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if (!pixel_format) return 0;

        NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixel_format shareContext:nil];
        [context setView:[data->window contentView]];
        [context makeCurrentContext];

        data->gl_context = context;
        return 1;
    }
}

// Get OpenGL context
static void* _macos_get_gl_context_impl(Lotus_Window* window) {
    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
    return data ? (__bridge void*)data->gl_context : NULL;
}

// Swap buffers
static void _macos_swap_buffers_impl(Lotus_Window* window) {
    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
    if (data && data->gl_context) {
        [data->gl_context flushBuffer];
    }
}

// Destroy OpenGL context
static void _macos_destroy_gl_context_impl(Lotus_Window* window) {
    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
    if (data && data->gl_context) {
        [data->gl_context clearDrawable];
        data->gl_context = NULL;
    }
}

// Platform API initialization
Lotus_Platform_API* lotus_init_platform(void) {
    internal_platform_state.platform = LOTUS_MACOS_TAG;
    
    internal_platform_state.event_state = lotus_init_event();
    internal_platform_state.input_state = lotus_init_input();

    static Lotus_Platform_API api = {
        .get_state = _macos_get_state_impl,
        .shutdown = _macos_shutdown_impl,
        .get_time = _macos_get_time_impl,
        .sleep = _macos_sleep_impl,

        .load_library = _macos_load_library_impl,
        .get_library_symbol = _macos_get_library_symbol_impl,
        .unload_library = _macos_unload_library_impl,
        
        .poll_events = _macos_poll_events_impl,
        
        .create_window = _macos_create_window_impl,
        .destroy_window = _macos_destroy_window_impl,
        
        .create_gl_context = _macos_create_gl_context_impl,
        .get_gl_context = _macos_get_gl_context_impl,
        .swap_buffers = _macos_swap_buffers_impl,
        .destroy_gl_context = _macos_destroy_gl_context_impl
    };

    return &api;
}

#endif
