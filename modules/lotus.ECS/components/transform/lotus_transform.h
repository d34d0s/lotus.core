#pragma once

#include "../../lotus_ecs_types.h"

#include "../../../lotus.core/utility/lotus_math.h"

typedef struct lotus_transform_component {
    lotus_mat4 model;
    lotus_vec3 scale;
    lotus_vec3 rotation;
    lotus_vec3 location;
} lotus_transform_component;

typedef struct lotus_transform_data {
    lotus_mat4* model;
    lotus_vec3* scale;
    lotus_vec3* rotation;
    lotus_vec3* location;
} lotus_transform_data;

ubyte _lotus_init_transform_data(lotus_transform_data* data);
void _lotus_destroy_transform_data(lotus_transform_data* data);

void _lotus_add_transform(void* data, lotus_entity entity);
void _lotus_rem_transform(void* data, lotus_entity entity);
void _lotus_set_transform(void* data, lotus_component component, lotus_entity entity);
lotus_component _lotus_get_transform(void* data, lotus_entity entity);
