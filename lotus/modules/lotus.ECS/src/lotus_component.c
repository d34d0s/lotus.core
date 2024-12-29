#include "../include/components/lotus_component.h"

ubyte lotus_register_component(
    lotus_component_manager* manager,
    lotus_component_type type,
    void* data,
    _add_component_ptr add_component,
    _rem_component_ptr rem_component,
    _set_component_ptr set_component,
    _get_component_ptr get_component
){
    manager->component_register[type].data = data;
    manager->component_handler[type].add_component = add_component;
    manager->component_handler[type].rem_component = rem_component;
    manager->component_handler[type].set_component = set_component;
    manager->component_handler[type].get_component = get_component;

    return 1;
}

ubyte lotus_unregister_component(lotus_component_manager* manager, lotus_component_type type) {
    manager->component_register[type].data = (void*)0;
    manager->component_handler[type].add_component = (void*)0;
    manager->component_handler[type].rem_component = (void*)0;
    manager->component_handler[type].get_component = (void*)0;

    return 1;
}

void lotus_add_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity) {
    if (type > LOTUS_COMPONENT_MAX || entity > LOTUS_ENTITY_MAX || entity < 0) return;
    manager->component_register[type].active[entity] = 1;
    manager->component_handler[type].add_component(
        manager->component_register[type].data,
        entity
    );
}

ubyte lotus_has_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity) {
    if (type > LOTUS_COMPONENT_MAX || entity > LOTUS_ENTITY_MAX || entity < 0) return 0;
    return manager->component_register[type].active[entity];
}

void lotus_rem_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity) {
    if (!lotus_has_component(manager, type, entity)) return;
    manager->component_register[type].active[entity] = 0;
    manager->component_handler[type].rem_component(
        manager->component_register[type].data,
        entity
    );
}

void lotus_set_component(lotus_component_manager* manager, lotus_component component, lotus_entity entity) {
    if (!lotus_has_component(manager, component.type, entity)) return;
    manager->component_handler[component.type].set_component(
        manager->component_register[component.type].data,
        component,
        entity
    );
}

lotus_component lotus_get_component(lotus_component_manager* manager, lotus_component_type type, lotus_entity entity) {
    if (!lotus_has_component(manager, type, entity)) return (lotus_component){0};
    return manager->component_handler[type].get_component(
        manager->component_register[type].data,
        entity
    );
}

