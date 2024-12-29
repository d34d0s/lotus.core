#pragma once

#include "../../lotus.CORE/include/platform/lotus_memory.h"
#include "../../lotus.CORE/include/platform/lotus_logger.h"

#include "../../lotus.CORE/include/application/lotus_app.h"

#include "../../lotus.ECS/include/lotus_ecs.h"

#define LOTUS_DEFAULT_SCENE_HEAP_SIZE 1024 * 1024   // 1kb

typedef struct Lotus_Scene_Info {
    char* name;
} Lotus_Scene_Info;

typedef struct Lotus_Scene {
    Lotus_Scene_Info info;
    Lotus_Allocator allocator;
    Lotus_Application application;
    Lotus_Entity_Manager entity_namager;
    Lotus_Component_Manager component_manager;
} Lotus_Scene;

ubyte _Lotus_Scene_Preframe(void);
ubyte _Lotus_Scene_Fixedframe(void);
ubyte _Lotus_Scene_Midframe(void);
ubyte _Lotus_Scene_Postframe(void);

LOTUS_API_ENTRY Lotus_Scene lotus_init_scene(char* scene_name, char* application_name, ubyte4 window_size[2]);
LOTUS_API_ENTRY void lotus_destroy_scene(Lotus_Scene* scene);
