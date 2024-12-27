#pragma once

#include "../../lotus_ecs_types.h"

typedef struct lotus_mesh_itf {
    unsigned int vbo;
    unsigned int ebo;
    unsigned int vao;
    void* vertices;
    float* raw_vertices;
} lotus_mesh_itf;

typedef struct lotus_mesh_data {
    unsigned int* vbo;
    unsigned int* ebo;
    unsigned int* vao;
    void** vertices;        // array of lotus_vertex arrays
    float** raw_vertices;   // raw floating-point vertex data
} lotus_mesh_data;

unsigned char _lotus_init_mesh_data(lotus_mesh_data* data);

void _lotus_add_mesh(void* data, lotus_entity entity);
void _lotus_rem_mesh(void* data, lotus_entity entity);
void _lotus_set_mesh(void* data, lotus_component component, lotus_entity entity);
lotus_component _lotus_get_mesh(void* data, lotus_entity entity);
