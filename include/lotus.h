#pragma once

#include "../modules/lotus.core/types.h"
#include "../modules/lotus.core/defines.h"

#ifdef LOTUS_APPLICATION
    #include "../modules/lotus.core/application/lotus_app.h"
#else 
#ifdef LOTUS_PLATFORM
    #include "../modules/lotus.core/memory/lotus_memory.h"
    #include "../modules/lotus.core/platform/lotus_input.h"
    #include "../modules/lotus.core/platform/lotus_event.h"
    #include "../modules/lotus.core/platform/lotus_platform.h"
#else
#ifdef LOTUS_GRAPHICS
    #include "../modules/lotus.core/graphics/lotus_gfx.h"
    #include "../modules/lotus.core/graphics/lotus_glapi.h"
#endif // LOTUS_GRAPHICS
#endif  // LOTUS_PLATFORM
#endif  // LOTUS_APPLICATION

/* define any of these directives to include sogl utilities from this header */
#if defined(LOTUS_MATH) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/lotus_math.h"
#if defined(LOTUS_FILEIO) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/lotus_fileio.h"
#if defined(LOTUS_DATASTRUCTS) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/lotus_array.h"
    #include "../modules/lotus.core/utility/lotus_hashmap.h"
#endif  // LOTUS_DATASTRUCTS
#endif  // LOTUS_FILEIO
#endif  // LOTUS_MATH

#ifdef LOTUS_CORE
    #include "../modules/lotus.core/application/lotus_app.h"

    #include "../modules/lotus.core/memory/lotus_memory.h"
    
    #include "../modules/lotus.core/platform/lotus_input.h"
    #include "../modules/lotus.core/platform/lotus_event.h"
    #include "../modules/lotus.core/platform/lotus_platform.h"
    
    #include "../modules/lotus.core/graphics/lotus_gfx.h"
    #include "../modules/lotus.core/graphics/lotus_glapi.h"
    
    #include "../modules/lotus.core/utility/lotus_math.h"
    #include "../modules/lotus.core/utility/lotus_fileio.h"
    #include "../modules/lotus.core/utility/lotus_array.h"
    #include "../modules/lotus.core/utility/lotus_hashmap.h"    
#endif  // LOTUS_CORE
