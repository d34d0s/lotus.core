#include "../include/application/lotus_app.h"

#include "../include/graphics/lotus_glapi.h"
#include "../include/platform/lotus_logger.h"

// internal application instance pointer
static Lotus_Application* instance_ptr;

// default application event callbacks
ubyte quit_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_EVENT_APP_QUIT) {
        instance_ptr->state.running = LOTUS_FALSE;
        return LOTUS_FALSE;
    }; return LOTUS_TRUE;
}


// application API implementation
ubyte lotus_init_application(Lotus_Application* instance, const char* app_name, ubyte4 window_size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance) return LOTUS_FALSE;
    if (!app_name) app_name = "Lotus Application";

    instance->info.name = app_name;
    instance->info.window_size[0] = window_size[0];
    instance->info.window_size[1] = window_size[1];
    
    instance->state.running = LOTUS_TRUE;

    instance->resource.platform_api = lotus_init_platform();
    if (!instance->resource.platform_api) {
        lotus_log_fatal("Failed to initialize platform_api layer!");
    }

    if (!lotus_create_application_window(instance, instance->info.name, instance->info.window_size)) {
        lotus_log_fatal("Failed to create application window!");
        return LOTUS_FALSE;
    }

    lotus_init_renderer();

    instance->resource.allocator = lotus_make_allocator(LOTUS_DEFAULT_APPLICATION_HEAP_SIZE);
    
    instance->resource.mod_manager.mod_count = 0;
    
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;

    // register default event callbacks
    // TODO: resize event
    lotus_register_event(LOTUS_EVENT_APP_QUIT, quit_callback);

    instance_ptr = instance;

    return LOTUS_TRUE;
}

ubyte lotus_set_application_preframe(Lotus_Application* instance, Lotus_Preframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->preframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_application_fixedframe(Lotus_Application* instance, Lotus_Fixedframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->fixedframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_application_midframe(Lotus_Application* instance, Lotus_Midframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->midframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_application_postframe(Lotus_Application* instance, Lotus_Postframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->postframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_create_application_window(Lotus_Application* instance, const char* title, ubyte4 size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->resource.window = instance->resource.platform_api->create_window((title != NULL) ? title : instance->info.name, size[0], size[1]);

    if (!instance->resource.platform_api->create_gl_context(&instance->resource.window)) {
        lotus_log_fatal("Failed to create GL Context!");
        return LOTUS_FALSE;
    }
    
    if (!lotus_load_fptrs()) {
        lotus_log_fatal("Failed to load GL function pointers!");
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

void lotus_destroy_application_window(Lotus_Application* instance) {
    if (!instance || !instance->state.running) return;
    instance->resource.platform_api->destroy_window(&instance->resource.window);
    instance->resource.platform_api->destroy_gl_context(&instance->resource.window);
}

ubyte lotus_run_application(Lotus_Application* instance) {
    ubyte result = LOTUS_FALSE;
    if (!instance || !instance->state.running) return result;
    
    lotus_set_log_level(LOTUS_LOG_FATAL);
    while (instance->state.running) {
        if (instance->midframe_callback == NULL) {
            lotus_log_fatal("Lotus Application Missing Mid-Frame Callback Pointer!");
            return result;
        }
        
        // preframe logic
        instance->resource.platform_api->poll_events();
        if (instance->preframe_callback != NULL) {
            result = instance->preframe_callback();
        }
        
        if (instance->fixedframe_callback != NULL) {
            result = instance->fixedframe_callback();
        }

        result = instance->midframe_callback();
        lotus_draw_flush();
        
        // postframe logic
        if (instance->postframe_callback != NULL) {
            result = instance->postframe_callback();
        }
        instance->resource.platform_api->swap_buffers(&instance->resource.window);
        lotus_update_input(0);

    }; lotus_exit_application(instance);

    return result;
}

sbyte lotus_load_mod(Lotus_Application* instance, const char* path, const char* name) {
    lotus_set_log_level(LOTUS_LOG_FATAL);

    Lotus_Mod_Manager* manager = &instance->resource.mod_manager;
    if (manager->mod_count >= LOTUS_ENGINE_MOD_MAX) {
        lotus_log_fatal("Maximum loaded mods reached");
        return -1;
    }

    ubyte mod_id = manager->mod_count;
    Lotus_Mod* mod = &manager->mods[mod_id];
    
    mod->library = instance->resource.platform_api->load_library(path, name);
    if (!mod->library.handle) {
        lotus_log_error("Failed to load mod library: %s\n", name);
        return -1;
    }

    mod->name = name;
    
    mod->initialize;
    mod->shutdown;
    
    mod->function_count = 0;
    mod->functions;

    if (!mod->initialize(instance)) {
        lotus_log_error("Failed to initialize mod: %s\n", name);
        instance->resource.platform_api->unload_library(&mod->library);
        return -1;
    }

    manager->mod_count++;

    lotus_set_log_level(LOTUS_LOG_INFO);
    lotus_log_info("Mod loaded: %s\n", name);

    return mod_id;
}

ubyte lotus_register_mod_function(Lotus_Mod* mod, const char* name) {

}

Lotus_Function_Pointer lotus_get_mod_function(Lotus_Mod* mod, const char* name) {

}

ubyte lotus_unload_mod(Lotus_Application* instance, const char* name) {
    Lotus_Mod_Manager* manager = &instance->resource.mod_manager;
    lotus_set_log_level(LOTUS_LOG_WARNING);

    for (ubyte i = 0; i < manager->mod_count; i++) {
        Lotus_Mod* mod = &manager->mods[i];
        if (strcmp(mod->name, name) == 0) {
            mod->shutdown(instance);
            instance->resource.platform_api->unload_library(&mod->library);

            mod->name = NULL;
            mod->shutdown = NULL;
            mod->initialize = NULL;
            
            manager->mod_count--;
            lotus_log_info("Mod unloaded: %s\n", name);
            return LOTUS_TRUE;
        }
    }
    
    lotus_log_warning("Mod not found for unloading: %s\n", name);
    return LOTUS_FALSE;
}

sbyte lotus_create_application_scene(Lotus_Application* instance, const char* scene_name) {
    if (!instance || instance->state.scene_count + 1 > LOTUS_ENGINE_SCENE_MAX) { return -1; }

    ubyte scene_id = instance->state.scene_count++;
    instance->resource.scene_map[scene_id] = lotus_init_scene(scene_id, scene_name);

    return scene_id;
}

Lotus_Scene* lotus_get_application_scene(Lotus_Application* instance, ubyte scene_id) {
    if (!instance || scene_id > LOTUS_ENGINE_SCENE_MAX) return NULL;
    Lotus_Scene* scene = instance->resource.scene_map[scene_id];
    return scene;
}

void lotus_destroy_application_scene(Lotus_Application* instance, ubyte scene_id) {
    if (!instance || scene_id > LOTUS_ENGINE_SCENE_MAX || instance->state.scene_count - 1 < 0) { return; }
    instance->state.scene_count--;
    lotus_destroy_scene(instance->resource.scene_map[scene_id]);
}

void lotus_exit_application(Lotus_Application* instance) {
    if (!instance) return;
    instance->state.running = LOTUS_FALSE;
    instance->info.name = "NULL";

    // unload mods
    for (ubyte i = 0; i < instance->resource.mod_manager.mod_count; i++) {
        Lotus_Mod* mod = &instance->resource.mod_manager.mods[i];
        mod->name = NULL;
        mod->shutdown = NULL;
        mod->initialize = NULL;
        instance->resource.platform_api->unload_library(&mod->library);
    }; instance->resource.mod_manager.mod_count = 0;

    // destroy scenes
    for (ubyte i = 0; i < instance->state.scene_count; i++) { lotus_destroy_application_scene(instance, i); }

    lotus_destroy_application_window(instance);

    lotus_destroy_renderer();

    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;

    instance->resource.platform_api->shutdown();
    instance->resource.platform_api = NULL;
}
