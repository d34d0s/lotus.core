#pragma once

#include "../modules/lotus.CORE/include/types.h"
#include "../modules/lotus.CORE/include/defines.h"

#include "../modules/lotus.CORE/include/utility/lotus_math.h"
#include "../modules/lotus.CORE/include/utility/lotus_fileio.h"
#include "../modules/lotus.CORE/include/utility/lotus_array.h"
#include "../modules/lotus.CORE/include/utility/lotus_hashmap.h"    

#if defined(LOTUS_APPLICATION)
    #include "../modules/lotus.CORE/include/application/lotus_app.h"
#endif  // LOTUS_APPLICATION

#if defined(LOTUS_2D)
    #include "../modules/lotus.2D/include/lotus_2D.h"
#endif  // LOTUS_2D

#if defined(LOTUS_3D)
    #include "../modules/lotus.3D/include/lotus_3D.h"
#endif  // LOTUS_3D

#if defined(LOTUS_SCENE)
    #include "../modules/lotus.SCENE/include/lotus_scene.h"
#endif  // LOTUS_SCENE

#if defined(LOTUS_PLUG)
    #include "../modules/lotus.PLUG/include/lotus_plug.h"
#endif  // LOTUS_PLUG

#if defined(LOTUS_PLATFORM)
    #include "../modules/lotus.CORE/include/platform/lotus_memory.h"
    #include "../modules/lotus.CORE/include/platform/lotus_input.h"
    #include "../modules/lotus.CORE/include/platform/lotus_event.h"
    #include "../modules/lotus.CORE/include/platform/lotus_platform.h"
#endif  // LOTUS_PLATFORM

#if defined(LOTUS_GRAPHICS)
    #include "../modules/lotus.CORE/include/graphics/lotus_gfx.h"
    #include "../modules/lotus.CORE/include/graphics/lotus_glapi.h"
#endif // LOTUS_GRAPHICS

#if defined(LOTUS_ECS)
    #include "../modules/lotus.ECS/include/lotus_ecs.h"
#endif // LOTUS_ECS

#if defined(LOTUS_CORE)
    #include "../modules/lotus.CORE/include/application/lotus_app.h"

    #include "../modules/lotus.CORE/include/platform/lotus_memory.h"
    
    #include "../modules/lotus.CORE/include/platform/lotus_input.h"
    #include "../modules/lotus.CORE/include/platform/lotus_event.h"
    #include "../modules/lotus.CORE/include/platform/lotus_platform.h"
    
    #include "../modules/lotus.CORE/include/graphics/lotus_gfx.h"
    #include "../modules/lotus.CORE/include/graphics/lotus_glapi.h"
#endif  // LOTUS_CORE
