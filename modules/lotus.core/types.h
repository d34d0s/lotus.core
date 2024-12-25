#pragma once

#include "defines.h"

typedef enum lotus_error_type {
    LOTUS_ERR_NONE=0,
    LOTUS_ERR_INIT,
    LOTUS_ERR_TYPE,
    LOTUS_ERR_FUNC,
    LOTUS_ERR_MALLOC,
    LOTUS_ERR_TYPES
} lotus_error_type;

typedef ubyte4(*lotus_preframe_callback)(void);
typedef ubyte4(*lotus_fixedframe_callback)(void);
typedef ubyte4(*lotus_midframe_callback)(void);
typedef ubyte4(*lotus_postframe_callback)(void);

typedef struct lotus_application lotus_application;
