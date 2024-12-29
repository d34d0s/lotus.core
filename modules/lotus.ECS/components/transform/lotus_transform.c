#include "lotus_transform.h"

#include "../lotus_component.h"

#include "../../../lotus.core/utility/lotus_array.h"

ubyte _lotus_init_transform_data(lotus_transform_data* data) {
    data->model = (lotus_mat4*)lotus_make_array(sizeof(lotus_mat4), LOTUS_ENTITY_MAX);
    data->scale = (lotus_vec3*)lotus_make_array(sizeof(lotus_vec3), LOTUS_ENTITY_MAX);
    data->rotation = (lotus_vec3*)lotus_make_array(sizeof(lotus_vec3), LOTUS_ENTITY_MAX);
    data->location = (lotus_vec3*)lotus_make_array(sizeof(lotus_vec3), LOTUS_ENTITY_MAX);

    return LOTUS_TRUE;
}

void _lotus_destroy_transform_data(lotus_transform_data* data) {
    lotus_destroy_array(data->model);
    lotus_destroy_array(data->scale);
    lotus_destroy_array(data->rotation);
    lotus_destroy_array(data->location);
    data->model = NULL;
    data->scale = NULL;
    data->rotation = NULL;
    data->location = NULL;
}

ubyte _lotus_validate_transform_data(lotus_transform_data* data) {
    return (
        data->model == NULL     ||
        data->scale == NULL     ||
        data->rotation == NULL  ||
        data->location == NULL
    ) ? LOTUS_FALSE : LOTUS_TRUE;
}

void _increment_transform_data_arrays(lotus_transform_data* data, ubyte4 count) {
    lotus_transform_data* transform_data = (lotus_transform_data*)data;
    if (!_lotus_validate_transform_data(transform_data)) return;
        
    LOTUS_ARRAY_SET_HEADER_FIELD(transform_data->model, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(transform_data->scale, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(transform_data->rotation, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(transform_data->location, LOTUS_ARRAY_LENGTH_FIELD, count);
}

void _lotus_add_transform(void* data, lotus_entity entity) {
    lotus_transform_data* transform_data = (lotus_transform_data*)data;
    if (!_lotus_validate_transform_data(transform_data)) return;
    ubyte4 count = LOTUS_ARRAY_GET_HEADER_FIELD(transform_data->model, LOTUS_ARRAY_LENGTH_FIELD);
    _increment_transform_data_arrays(transform_data, count+1);
    
    transform_data->model[entity] = lotus_identity();
    transform_data->scale[entity] = lotus_new_vec3(0, 0, 0);
    transform_data->rotation[entity] = lotus_new_vec3(0, 0, 0);
    transform_data->location[entity] = lotus_new_vec3(0, 0, 0);
}

void _lotus_rem_transform(void* data, lotus_entity entity) {
    lotus_transform_data* transform_data = (lotus_transform_data*)data;
    if (!_lotus_validate_transform_data(transform_data)) return;
    ubyte4 count = LOTUS_ARRAY_GET_HEADER_FIELD(transform_data->model, LOTUS_ARRAY_LENGTH_FIELD);
    _increment_transform_data_arrays(transform_data, count-1);
    
    transform_data->model[entity] = (lotus_mat4){-1};
    transform_data->scale[entity] = lotus_new_vec3(-1, -1, -1);
    transform_data->rotation[entity] = lotus_new_vec3(-1, -1, -1);
    transform_data->location[entity] = lotus_new_vec3(-1, -1, -1);
}

void _lotus_set_transform(void* data, lotus_component component, lotus_entity entity) {
    lotus_transform_data* transform_data = (lotus_transform_data*)data;
    if (!_lotus_validate_transform_data(transform_data)) return;
    
    if (component.type == LOTUS_TRANSFORM_COMPONENT) {
        transform_data->model[entity] = component.data.transform.model;
        transform_data->scale[entity] = component.data.transform.scale;
        transform_data->rotation[entity] = component.data.transform.rotation;
        transform_data->location[entity] = component.data.transform.location;
    } else { return; }
}

lotus_component _lotus_get_transform(void* data, lotus_entity entity) {
    lotus_transform_data* transform_data = (lotus_transform_data*)data;
    if (!_lotus_validate_transform_data(transform_data)) return (lotus_component){ .type = LOTUS_COMPONENT_MAX+1 };

    return (lotus_component){
        .type = LOTUS_TRANSFORM_COMPONENT,
        .data.transform.model = transform_data->model[entity],
        .data.transform.scale = transform_data->scale[entity],
        .data.transform.rotation = transform_data->rotation[entity],
        .data.transform.location = transform_data->location[entity]
    };
}
