#pragma once

#include <math.h>
#include "../types.h"
#include "../defines.h"

#define LOTUS_PI 3.14159265358979323846

typedef struct tagVec2 {
    f32 x, y;
} lotus_vec2;

typedef struct tagVec2i {
    int x, y;
} lotus_vec2i;

typedef struct tagVec2b {
    ubyte x, y;
} lotus_vec2b;

typedef struct tagVec3 {
    f32 x, y, z;
} lotus_vec3;

typedef struct tagVec3i {
    int x, y, z;
} lotus_vec3i;

typedef struct tagVec3b {
    ubyte x, y, z;
} lotus_vec3b;

typedef struct tagVec4 {
    f32 x, y, z, w;
} lotus_vec4;

typedef struct tagVec4i {
    int x, y, z, w;
} lotus_vec4i;

typedef struct tagVec4b {
    ubyte x, y, z, w;
} lotus_vec4b;

typedef struct tagMat4{
    f32 m[16];
} lotus_mat4;

#define LOTUS_VEC2(type, x, y) (type[2]){x, y}
#define LOTUS_VEC3(type, x, y, z) (type[3]){x, y, z}
#define LOTUS_VEC4(type, x, y, z, w) (type[4]){x, y, z, w}

#define LOTUS_MAT4(type) (type[16]){0}
#define LOTUS_IDENTITY(type) (type[16]){ \
    1.0, 0.0, 0.0, 0.0, \
    0.0, 1.0, 0.0, 0.0, \
    0.0, 0.0, 1.0, 0.0, \
    0.0, 0.0, 0.0, 1.0  \
}

LOTUS_API_ENTRY float lotus_to_radians(float deg);

LOTUS_API_ENTRY lotus_vec2 lotus_new_vec2(float x, float y);
LOTUS_API_ENTRY lotus_vec2 lotus_scale_vec2(lotus_vec2 v, float scale);
LOTUS_API_ENTRY lotus_vec2 lotus_add_vec2(lotus_vec2 v1, lotus_vec2 v2);
LOTUS_API_ENTRY lotus_vec2 lotus_sub_vec2(lotus_vec2 v1, lotus_vec2 v2);
LOTUS_API_ENTRY float lotus_dot_vec2(lotus_vec2 v1, lotus_vec2 v2);
LOTUS_API_ENTRY lotus_vec2 lotus_norm_vec2(lotus_vec2 v);

LOTUS_API_ENTRY lotus_vec3 lotus_new_vec3(float x, float y, float z);
LOTUS_API_ENTRY lotus_vec3 lotus_scale_vec3(lotus_vec3 v, float scale);
LOTUS_API_ENTRY lotus_vec3 lotus_add_vec3(lotus_vec3 v1, lotus_vec3 v2);
LOTUS_API_ENTRY lotus_vec3 lotus_sub_vec3(lotus_vec3 v1, lotus_vec3 v2);
LOTUS_API_ENTRY float lotus_dot_vec3(lotus_vec3 a, lotus_vec3 b);
LOTUS_API_ENTRY lotus_vec3 lotus_norm_vec3(lotus_vec3 v);
LOTUS_API_ENTRY lotus_vec3 lotus_cross_vec3(lotus_vec3 v1, lotus_vec3 v2);

LOTUS_API_ENTRY lotus_vec4 lotus_new_vec4(float x, float y, float z, float w);
LOTUS_API_ENTRY lotus_vec4 lotus_scale_vec4(lotus_vec4 v, float scale);
LOTUS_API_ENTRY lotus_vec4 lotus_add_vec4(lotus_vec4 v1, lotus_vec4 v2);
LOTUS_API_ENTRY lotus_vec4 lotus_sub_vec4(lotus_vec4 v1, lotus_vec4 v2);
LOTUS_API_ENTRY float lotus_dot_vec4(lotus_vec4 v1, lotus_vec4 v2);
LOTUS_API_ENTRY lotus_vec4 lotus_norm_vec4(lotus_vec4 v);

LOTUS_API_ENTRY lotus_mat4 lotus_identity();
LOTUS_API_ENTRY lotus_mat4 lotus_mul_mat4(lotus_mat4 a, lotus_mat4 b);
LOTUS_API_ENTRY lotus_vec3 lotus_mul_mat4_vec3(lotus_mat4 m, lotus_vec3 v);
LOTUS_API_ENTRY lotus_mat4 lotus_trans_mat4(float x, float y, float z);
LOTUS_API_ENTRY lotus_mat4 lotus_scale_mat4(float x, float y, float z);
LOTUS_API_ENTRY lotus_mat4 lotus_look_at(lotus_vec3 eye, lotus_vec3 center, lotus_vec3 up);
LOTUS_API_ENTRY lotus_mat4 lotus_rot_mat4(float x, float y, float z, float angle);
LOTUS_API_ENTRY lotus_mat4 lotus_perspective(float fov, float aspect, float near, float far);
LOTUS_API_ENTRY lotus_mat4 lotus_ortho(float left, float right, float bottom, float top, float near, float far);
