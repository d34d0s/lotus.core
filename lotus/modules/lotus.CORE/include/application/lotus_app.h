#pragma once

#include "../platform/lotus_input.h"
#include "../platform/lotus_event.h"
#include "../platform/lotus_platform.h"

#include "../graphics/lotus_gl.h"

#include "../utility/lotus_hashmap.h"

#include "../../../lotus.SCENE/include/lotus_scene.h"

#define LOTUS_ENGINE_MOD_MAX    ((1U << 8) - 1)
#define LOTUS_ENGINE_SCENE_MAX  (1U << 4)
#define LOTUS_DEFAULT_APPLICATION_HEAP_SIZE 1024 * 1024 // 1kb application heap for runtime allocations

typedef void (*Lotus_Function_Pointer)(void);
typedef struct Lotus_Mod_Function {
    const char* name;
    Lotus_Function_Pointer ptr;
} Lotus_Mod_Function;

typedef ubyte (*Lotus_Mod_Initialize)(Lotus_Application* caller);
typedef void (*Lotus_Mod_Shutdown)(Lotus_Application* caller);
typedef struct Lotus_Mod {
    const char* name;
    Lotus_DyLib library;
    Lotus_Mod_Shutdown shutdown;
    Lotus_Mod_Initialize initialize;

    ubyte function_count;
    Lotus_Function_Pointer* functions;
} Lotus_Mod;

typedef struct Lotus_Mod_Manager {
    ubyte mod_count;
    Lotus_Mod mods[LOTUS_ENGINE_MOD_MAX];
} Lotus_Mod_Manager;

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
        
        Lotus_Mod_Manager mod_manager;
        Lotus_Scene* scene_map[LOTUS_ENGINE_SCENE_MAX];

        Lotus_Allocator allocator;
        Lotus_Platform_API* platform_api;
    } resource;

    Lotus_Preframe_Callback preframe_callback;
    Lotus_Fixedframe_Callback fixedframe_callback;
    Lotus_Midframe_Callback midframe_callback;
    Lotus_Postframe_Callback postframe_callback;
};

ubyte lotus_create_application_window(Lotus_Application* instance, const char* title, ubyte4 size[2]);
void lotus_destroy_application_window(Lotus_Application* instance);

LOTUS_API_ENTRY ubyte lotus_init_application(Lotus_Application* instance, const char* app_name, ubyte4 window_size[2]);
LOTUS_API_ENTRY void lotus_exit_application(Lotus_Application* instance);

LOTUS_API_ENTRY ubyte lotus_run_application(Lotus_Application* instance);

// modding API
LOTUS_API_ENTRY sbyte lotus_load_mod(Lotus_Application* instance, const char* path, const char* name);
LOTUS_API_ENTRY ubyte lotus_register_mod_function(Lotus_Mod* mod, const char* name);
LOTUS_API_ENTRY Lotus_Function_Pointer lotus_get_mod_function(Lotus_Mod* mod, const char* name);
LOTUS_API_ENTRY ubyte lotus_unload_mod(Lotus_Application* instance, const char* name);

LOTUS_API_ENTRY sbyte lotus_create_application_scene(Lotus_Application* instance, const char* scene_name);
LOTUS_API_ENTRY Lotus_Scene* lotus_get_application_scene(Lotus_Application* instance, ubyte scene_id);
LOTUS_API_ENTRY void lotus_destroy_application_scene(Lotus_Application* instance, ubyte scene_id);

LOTUS_API_ENTRY ubyte lotus_set_application_preframe(Lotus_Application* instance, Lotus_Preframe_Callback callback);
LOTUS_API_ENTRY ubyte lotus_set_application_fixedframe(Lotus_Application* instance, Lotus_Fixedframe_Callback callback);
LOTUS_API_ENTRY ubyte lotus_set_application_midframe(Lotus_Application* instance, Lotus_Midframe_Callback callback);
LOTUS_API_ENTRY ubyte lotus_set_application_postframe(Lotus_Application* instance, Lotus_Postframe_Callback callback);
