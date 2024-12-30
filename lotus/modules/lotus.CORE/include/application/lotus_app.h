#pragma once

// TODO: application default callbacks, and events (push out frame events, to register external frame callbacks to) 
// eliminating redundant functions (set mid/fixed/...) just use the register_event_callback() function

#include "../platform/lotus_input.h"
#include "../platform/lotus_event.h"
#include "../platform/lotus_platform.h"

#include "../graphics/lotus_gl.h"

#include "../utility/lotus_hashmap.h"

#include "../../../lotus.SCENE/include/lotus_scene.h"
#include "../../../lotus.PLUG/include/lotus_plug.h"

#define LOTUS_ENGINE_SCENE_MAX  (1U << 4)
#define LOTUS_DEFAULT_APPLICATION_HEAP_SIZE 1024 * 1024 // 1kb application heap for runtime allocations

typedef struct Lotus_Application Lotus_Application;
typedef ubyte4(*Lotus_Application_Preframe_Callback)(void);
typedef ubyte4(*Lotus_Application_Fixedframe_Callback)(void);
typedef ubyte4(*Lotus_Application_Midframe_Callback)(void);
typedef ubyte4(*Lotus_Application_Postframe_Callback)(void);

struct Lotus_Application {
    struct info {
        const char* name;
        ubyte4 window_size[2];
    } info;

    struct state {
        ubyte running;
        ubyte scene_count;
    } state;

    struct resource {
        Lotus_Window window;
        Lotus_Allocator allocator;
        
        Lotus_Plug_Manager* plug_manager;
        Lotus_Scene* scene_map[LOTUS_ENGINE_SCENE_MAX];

        Lotus_Plug_API* plug_api;
        Lotus_Platform_API* platform_api;
    } resource;

    Lotus_Application_Preframe_Callback preframe_callback;
    Lotus_Application_Fixedframe_Callback fixedframe_callback;
    Lotus_Application_Midframe_Callback midframe_callback;
    Lotus_Application_Postframe_Callback postframe_callback;
};

typedef struct Lotus_Application_API {
    Lotus_Application* (*initialize)(const char* app_name, ubyte4 window_size[2]);
    void (*shutdown)(void);

    ubyte (*create_window)(const char* title, ubyte4 size[2]);
    void (*destroy_window)(void);
    
    sbyte (*create_scene)(const char* scene_name);
    Lotus_Scene* (*get_scene)(ubyte scene_id);
    void (*destroy_scene)(ubyte scene_id);

    ubyte (*set_preframe)(Lotus_Application_Preframe_Callback callback);
    ubyte (*set_fixedframe)(Lotus_Application_Fixedframe_Callback callback);
    ubyte (*set_midframe)(Lotus_Application_Midframe_Callback callback);
    ubyte (*set_postframe)(Lotus_Application_Postframe_Callback callback);
    ubyte (*run)(void);
} Lotus_Application_API;

LOTUS_API_ENTRY Lotus_Application_API* lotus_init_application(void);
