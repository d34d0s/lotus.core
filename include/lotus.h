#pragma once

#include "../modules/lotus.core/types.h"
#include "../modules/lotus.core/defines.h"

#ifdef LOTUS_APPLICATION
    #include "../modules/lotus.core/application/soapp.h"
#else 
#ifdef LOTUS_PLATFORM
    #include "../modules/lotus.core/memory/somemory.h"
    #include "../modules/lotus.core/platform/soinput.h"
    #include "../modules/lotus.core/platform/soevent.h"
    #include "../modules/lotus.core/platform/soplatform.h"
#else
#ifdef LOTUS_GRAPHICS
    #include "../modules/lotus.core/graphics/sogfx.h"
    #include "../modules/lotus.core/graphics/soglapi.h"
#endif // LOTUS_GRAPHICS
#endif  // LOTUS_PLATFORM
#endif  // LOTUS_APPLICATION

/* define any of these directives to include sogl utilities from this header */
#if defined(LOTUS_MATH) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/somath.h"
#if defined(LOTUS_FILEIO) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/sofileio.h"
#if defined(LOTUS_DATASTRUCTS) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/soarray.h"
    #include "../modules/lotus.core/utility/sohashmap.h"
#endif  // LOTUS_DATASTRUCTS
#endif  // LOTUS_FILEIO
#endif  // LOTUS_MATH

#ifdef LOTUS_CORE
    #include "../modules/lotus.core/application/soapp.h"

    #include "../modules/lotus.core/memory/somemory.h"
    
    #include "../modules/lotus.core/platform/soinput.h"
    #include "../modules/lotus.core/platform/soevent.h"
    #include "../modules/lotus.core/platform/soplatform.h"
    
    #include "../modules/lotus.core/graphics/sogfx.h"
    #include "../modules/lotus.core/graphics/soglapi.h"
    
    #include "../modules/lotus.core/utility/somath.h"
    #include "../modules/lotus.core/utility/sofileio.h"
    #include "../modules/lotus.core/utility/soarray.h"
    #include "../modules/lotus.core/utility/sohashmap.h"    
#endif  // LOTUS_CORE
