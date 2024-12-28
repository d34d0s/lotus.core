#pragma once

#include "../../lotus_ecs_types.h"

typedef enum lotus_vertex_attr {
    LOTUS_LOCATION_ATTR = 1 << 0, // 1 (0b0001)
    LOTUS_COLOR_ATTR    = 1 << 1, // 2 (0b0010)
    LOTUS_TCOORD_ATTR   = 1 << 2, // 4 (0b0100)
    LOTUS_NORMAL_ATTR   = 1 << 3, // 8 (0b1000)
    LOTUS_VERTEX_ATTRIBS  = 1 << 4 
} lotus_vertex_attr;

typedef struct lotus_mesh_itf {
    ubyte attrs;
    ubyte4 vbo;
    ubyte4 ebo;
    ubyte4 vao;
    ubyte4 n_vertices;
    ubyte4 n_indices;
    f32* vertices;
    ubyte4* indices;
} lotus_mesh_itf;

typedef struct lotus_mesh_data {
    ubyte* attrs;
    ubyte4* vbo;
    ubyte4* ebo;
    ubyte4* vao;
    ubyte4* n_vertices;
    ubyte4* n_indices;
    f32** vertices;   // raw floating-point vertex data
    ubyte4** indices;
} lotus_mesh_data;

ubyte _lotus_init_mesh_data(lotus_mesh_data* data);
void _lotus_destroy_mesh_data(lotus_mesh_data* data);

void _lotus_add_mesh(void* data, lotus_entity entity);
void _lotus_rem_mesh(void* data, lotus_entity entity);
void _lotus_set_mesh(void* data, lotus_component component, lotus_entity entity);
lotus_component _lotus_get_mesh(void* data, lotus_entity entity);
