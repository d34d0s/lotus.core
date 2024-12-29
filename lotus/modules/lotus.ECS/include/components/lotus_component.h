#pragma once

#include "../lotus_ecs_types.h"

#include "lotus_mesh.h"
#include "lotus_texture.h"
#include "lotus_transform.h"

typedef union lotus_component_data {
    lotus_mesh_component mesh;
    lotus_transform_component transform;
} lotus_component_data;

// tagged component union
struct lotus_component {
    lotus_component_data data;
    lotus_component_type type;
};

typedef void (*_add_component_ptr)(void* component_data, lotus_entity entity);
typedef void (*_rem_component_ptr)(void* component_data, lotus_entity entity);
typedef void (*_set_component_ptr)(void* component_data, lotus_component component, lotus_entity entity);
typedef lotus_component (*_get_component_ptr)(void* component_data, lotus_entity entity);

typedef struct lotus_component_manager {
    struct component_handler {
        _add_component_ptr add_component;
        _rem_component_ptr rem_component;
        _set_component_ptr set_component;
        _get_component_ptr get_component;
    } component_handler[LOTUS_COMPONENT_TYPES];
    
    struct component_register {
        void* data;
        ubyte active[LOTUS_ENTITY_MAX];
    } component_register[LOTUS_COMPONENT_TYPES];
} lotus_component_manager;

LOTUS_API_ENTRY ubyte lotus_register_component(
    lotus_component_manager* manager,
    lotus_component_type type,
    void* data,
    _add_component_ptr add_component,
    _rem_component_ptr rem_component,
    _set_component_ptr set_component,
    _get_component_ptr get_component
);
LOTUS_API_ENTRY ubyte lotus_unregister_component(lotus_component_manager* manager, lotus_component_type type);

LOTUS_API_ENTRY void lotus_add_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity);
LOTUS_API_ENTRY ubyte lotus_has_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity);
LOTUS_API_ENTRY void lotus_rem_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity);
LOTUS_API_ENTRY void lotus_set_component(lotus_component_manager* manager, lotus_component component, lotus_entity entity);
LOTUS_API_ENTRY lotus_component lotus_get_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity);
