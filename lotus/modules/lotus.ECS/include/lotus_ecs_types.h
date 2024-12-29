#pragma once

#include "../../lotus.core/include/defines.h"

typedef unsigned int lotus_entity;
#define LOTUS_ENTITY_MAX ((1U << 16) - 1)

typedef struct lotus_component lotus_component;
#define LOTUS_DEFINE_COMPONENT_TYPE(t) t##_COMPONENT
typedef enum lotus_component_type {
    LOTUS_DEFINE_COMPONENT_TYPE(LOTUS_MESH),
    LOTUS_DEFINE_COMPONENT_TYPE(LOTUS_TEXTURE),
    LOTUS_DEFINE_COMPONENT_TYPE(LOTUS_TRANSFORM),
    /* user defined components should have IDs < 255 */
    LOTUS_COMPONENT_TYPES,
    LOTUS_COMPONENT_MAX = ((1U << 8) - 1)
} lotus_component_type;

