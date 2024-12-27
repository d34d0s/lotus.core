#include "lotus_ecs.h"

static lotus_mesh_data internal_mesh_data = {0};

unsigned char lotus_init_ecs(lotus_entity_manager* e_manager, lotus_component_manager* c_manager) {
    e_manager->next = 0;
    e_manager->count = 0;

    lotus_register_component(
        c_manager, LOTUS_MESH_COMPONENT, &internal_mesh_data,
        _lotus_add_mesh,
        _lotus_rem_mesh,
        _lotus_set_mesh,
        _lotus_get_mesh
    );

    return 1;
}

lotus_entity lotus_make_entity(lotus_entity_manager* manager) {
    if (manager->count + 1 > LOTUS_ENTITY_MAX) return LOTUS_ENTITY_MAX + 1;
    lotus_entity entity = manager->next++;
    manager->count++;
    manager->alive[entity] = 1;
    return entity;
}

unsigned char lotus_kill_entity(lotus_entity_manager* manager, lotus_entity entity) {
    if (manager->count - 1 < 0) return 0;
    manager->next--;
    manager->count--;
    manager->alive[entity] = 0;
    return 1;
}