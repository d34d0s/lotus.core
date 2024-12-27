#include "lotus_mesh.h"

#include <stdlib.h>
#include "../lotus_component.h"

unsigned char _lotus_init_mesh_data(lotus_mesh_data* data) {
    data->vbo = (void*)0;
    data->ebo = (void*)0;
    data->vao = (void*)0;
    data->vertices = (void*)0;
    data->raw_vertices = (void*)0;

    return 1;
}

unsigned char _lotus_validate_mesh_data(lotus_mesh_data* data) {
    return (
        data->vbo == (void*)0 ||
        data->ebo == (void*)0 ||
        data->vao == (void*)0 ||
        data->vertices == (void*)0 ||
        data->raw_vertices == (void*)0
    ) ? 0 : 1;
}

void _lotus_add_mesh(void* data, lotus_entity entity) {
    lotus_mesh_data* mesh_data = (lotus_mesh_data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return;
}

void _lotus_rem_mesh(void* data, lotus_entity entity) {
    lotus_mesh_data* mesh_data = (lotus_mesh_data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return;
}

void _lotus_set_mesh(void* data, lotus_component component, lotus_entity entity) {
    lotus_mesh_data* mesh_data = (lotus_mesh_data*)data;
    if (!_lotus_validate_mesh_data(mesh_data) || component.type != LOTUS_MESH_COMPONENT) return;

    if (component.type == LOTUS_MESH_COMPONENT) {
        mesh_data->vbo[entity] = component.data.mesh_data.vbo;
        mesh_data->ebo[entity] = component.data.mesh_data.ebo;
        mesh_data->vbo[entity] = component.data.mesh_data.vbo;
        mesh_data->vertices[entity] = component.data.mesh_data.vertices;
        mesh_data->raw_vertices[entity] = component.data.mesh_data.raw_vertices;
    } else { return; }
}

lotus_component _lotus_get_mesh(void* data, lotus_entity entity) {
    lotus_mesh_data* mesh_data = (lotus_mesh_data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return (lotus_component){ .type = LOTUS_COMPONENT_MAX+1 };
    
    return (lotus_component){
        .type = LOTUS_MESH_COMPONENT,
        .data.mesh_data.vbo = mesh_data->vbo[entity],
        .data.mesh_data.ebo = mesh_data->ebo[entity],
        .data.mesh_data.vbo = mesh_data->vbo[entity],
        .data.mesh_data.vertices = mesh_data->vertices[entity],
        .data.mesh_data.raw_vertices = mesh_data->raw_vertices[entity]
    };
}
