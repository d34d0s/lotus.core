#pragma once

#include <math.h>
#include "../types.h"
#include "../defines.h"

#define SOGL_PI 3.14159265358979323846

typedef struct tagVec2 {
    f32 x, y;
} sogl_vec2;

typedef struct tagVec2i {
    int x, y;
} sogl_vec2i;

typedef struct tagVec2b {
    ubyte x, y;
} sogl_vec2b;

typedef struct tagVec3 {
    f32 x, y, z;
} sogl_vec3;

typedef struct tagVec3i {
    int x, y, z;
} sogl_vec3i;

typedef struct tagVec3b {
    ubyte x, y, z;
} sogl_vec3b;

typedef struct tagVec4 {
    f32 x, y, z, w;
} sogl_vec4;

typedef struct tagVec4i {
    int x, y, z, w;
} sogl_vec4i;

typedef struct tagVec4b {
    ubyte x, y, z, w;
} sogl_vec4b;

typedef struct tagMat4{
    f32 m[16];
} sogl_mat4;

#define SOGL_VEC2(type, x, y) (type[2]){x, y}
#define SOGL_VEC3(type, x, y, z) (type[3]){x, y, z}
#define SOGL_VEC4(type, x, y, z, w) (type[4]){x, y, z, w}

#define SOGL_MAT4(type) (type[16]){0}
#define SOGL_IDENTITY(type) (type[16]){ \
    1.0, 0.0, 0.0, 0.0, \
    0.0, 1.0, 0.0, 0.0, \
    0.0, 0.0, 1.0, 0.0, \
    0.0, 0.0, 0.0, 1.0  \
}

SOGL_API_ENTRY float sogl_to_radians(float deg);

SOGL_API_ENTRY sogl_vec2 sogl_new_vec2(float x, float y);
SOGL_API_ENTRY sogl_vec2 sogl_scale_vec2(sogl_vec2 v, float scale);
SOGL_API_ENTRY sogl_vec2 sogl_add_vec2(sogl_vec2 v1, sogl_vec2 v2);
SOGL_API_ENTRY sogl_vec2 sogl_sub_vec2(sogl_vec2 v1, sogl_vec2 v2);
SOGL_API_ENTRY float sogl_dot_vec2(sogl_vec2 v1, sogl_vec2 v2);
SOGL_API_ENTRY sogl_vec2 sogl_norm_vec2(sogl_vec2 v);

SOGL_API_ENTRY sogl_vec3 sogl_new_vec3(float x, float y, float z);
SOGL_API_ENTRY sogl_vec3 sogl_scale_vec3(sogl_vec3 v, float scale);
SOGL_API_ENTRY sogl_vec3 sogl_add_vec3(sogl_vec3 v1, sogl_vec3 v2);
SOGL_API_ENTRY sogl_vec3 sogl_sub_vec3(sogl_vec3 v1, sogl_vec3 v2);
SOGL_API_ENTRY float sogl_dot_vec3(sogl_vec3 a, sogl_vec3 b);
SOGL_API_ENTRY sogl_vec3 sogl_norm_vec3(sogl_vec3 v);
SOGL_API_ENTRY sogl_vec3 sogl_cross_vec3(sogl_vec3 v1, sogl_vec3 v2);

SOGL_API_ENTRY sogl_vec4 sogl_new_vec4(float x, float y, float z, float w);
SOGL_API_ENTRY sogl_vec4 sogl_scale_vec4(sogl_vec4 v, float scale);
SOGL_API_ENTRY sogl_vec4 sogl_add_vec4(sogl_vec4 v1, sogl_vec4 v2);
SOGL_API_ENTRY sogl_vec4 sogl_sub_vec4(sogl_vec4 v1, sogl_vec4 v2);
SOGL_API_ENTRY float sogl_dot_vec4(sogl_vec4 v1, sogl_vec4 v2);
SOGL_API_ENTRY sogl_vec4 sogl_norm_vec4(sogl_vec4 v);

SOGL_API_ENTRY sogl_mat4 sogl_identity();
SOGL_API_ENTRY sogl_mat4 sogl_mul_mat4(sogl_mat4 a, sogl_mat4 b);
SOGL_API_ENTRY sogl_vec3 sogl_mul_mat4_vec3(sogl_mat4 m, sogl_vec3 v);
SOGL_API_ENTRY sogl_mat4 sogl_trans_mat4(float x, float y, float z);
SOGL_API_ENTRY sogl_mat4 sogl_scale_mat4(float x, float y, float z);
SOGL_API_ENTRY sogl_mat4 sogl_look_at(sogl_vec3 eye, sogl_vec3 center, sogl_vec3 up);
SOGL_API_ENTRY sogl_mat4 sogl_rot_mat4(float x, float y, float z, float angle);
SOGL_API_ENTRY sogl_mat4 sogl_perspective(float fov, float aspect, float near, float far);
SOGL_API_ENTRY sogl_mat4 sogl_ortho(float left, float right, float bottom, float top, float near, float far);
