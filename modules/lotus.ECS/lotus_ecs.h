#pragma once

#include "lotus_ecs_types.h"
#include "components/lotus_component.h"

typedef struct lotus_entity_manager {
    unsigned short next;
    unsigned short count;
    unsigned char alive[LOTUS_ENTITY_MAX];
} lotus_entity_manager;

LOTUS_API_ENTRY unsigned char lotus_init_ecs(lotus_entity_manager* e_manager, lotus_component_manager* c_manager);

LOTUS_API_ENTRY lotus_entity lotus_make_entity(lotus_entity_manager* manager);
LOTUS_API_ENTRY unsigned char lotus_kill_entity(lotus_entity_manager* manager, lotus_entity entity);
