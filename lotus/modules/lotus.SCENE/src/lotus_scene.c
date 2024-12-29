#include "../include/lotus_scene.h"

Lotus_Scene lotus_init_scene(char* scene_name, char* application_name, ubyte4 window_size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    Lotus_Scene scene;

    scene.info.name = (!scene_name) ? "Lotus Scene" : scene_name;
    
    scene.allocator = lotus_make_allocator(LOTUS_DEFAULT_SCENE_HEAP_SIZE);

    if (!lotus_init_application(&scene.application, (!application_name) ? "Lotus Application" : application_name)) {
        lotus_log_fatal("Failed to initialize application (scene): %s\n", scene.info.name);
        lotus_destroy_allocator(&scene.allocator);
    }

    if (!lotus_init_ecs(&scene.entity_namager, &scene.component_manager)) {
        lotus_log_fatal("Failed to initialize ecs (scene): %s\n", scene.info.name);
        lotus_destroy_allocator(&scene.allocator);
        lotus_exit_application(&scene.application);
    }

    if (!lotus_make_application_window(&scene.application, "Lotus Window", window_size)) {
        lotus_log_fatal("Failed to create application window (scene): %s\n", scene.info.name);
        lotus_exit_ecs(&scene.entity_namager, &scene.component_manager);
        lotus_destroy_allocator(&scene.allocator);
        lotus_exit_application(&scene.application);
    }

    lotus_init_renderer();

    return scene;
}

void lotus_destroy_scene(Lotus_Scene* scene) {
    if (!scene) return;
    lotus_exit_ecs(&scene->entity_namager, &scene->component_manager);
    lotus_destroy_application_window(&scene->application);
    lotus_exit_application(&scene->application);
    lotus_destroy_renderer();
    lotus_destroy_allocator(&scene->allocator);
}

