#pragma once

#include "../defines.h"

#include "../utility/lotus_math.h"
#include "../utility/lotus_hashmap.h"

#include "../platform/lotus_memory.h"

// GL equivalent defines
#define LOTUS_STATIC_DRAW                0x88E4

#define LOTUS_ARRAY_BUFFER               0x8892
#define LOTUS_ELEMENT_ARRAY_BUFFER       0x8893

#define LOTUS_VERTEX_SHADER              0x8B31
#define LOTUS_FRAGMENT_SHADER            0x8B30

#define LOTUS_LINK_STATUS                0x8B82
#define LOTUS_COMPILE_STATUS             0x8B81
#define LOTUS_ACTIVE_ATTRIBUTES          0x8B89

#define LOTUS_LINE                       0x1B01
#define LOTUS_FRONT_AND_BACK             0x0408

#define LOTUS_SHADING_LANGUAGE_VERSION   0x8B8C

#define LOTUS_TEXTURE0                   0x84C0
#define LOTUS_TEXTURE1                   0x84C1
#define LOTUS_TEXTURE2                   0x84C2
#define LOTUS_TEXTURE3                   0x84C3
#define LOTUS_ACTIVE_TEXTURE             0x84E0
#define LOTUS_MAX_TEXTURE_UNITS          0x84E2

#define LOTUS_MAX_DRAW_BUFFERS 255

#define LOTUS_BUFFER_VBO 0
#define LOTUS_BUFFER_EBO 1
#define LOTUS_BUFFER_VAO 2

#define LOTUS_COLOR_V3(r, g, b) lotus_new_vec3(r/255.0, g/255.0, b/255.0)
#define LOTUS_COLOR_V4(r, g, b, a) lotus_new_vec4(r/255.0, g/255.0, b/255.0, a/255.0)

typedef enum Lotus_Uniform_Type {
    LOTUS_UNIFORM_NONE=0,
    LOTUS_UNIFORM_VEC2,
    LOTUS_UNIFORM_VEC3,
    LOTUS_UNIFORM_VEC4,
    LOTUS_UNIFORM_MAT4,
    LOTUS_UNIFORM_TYPES
} Lotus_Uniform_Type;

typedef struct Lotus_Shader {
    sbyte4 program;
    Lotus_Hashmap* uniforms;
} Lotus_Shader;

typedef struct Lotus_Uniform {
    sbyte4 location;
    void* value;
    const char* name;
} Lotus_Uniform;

typedef struct Lotus_Draw_Buffer {
    ubyte4 handle[3];
    Lotus_Mat4* matrix;
    ubyte4 index_count;
    ubyte4 vertex_count;
} Lotus_Draw_Buffer;

// GL equivalents
typedef enum Lotus_Draw_Mode {
    LOTUS_POINT_MODE = 0x0000,
    LOTUS_LINE_MODE = 0x0001,
    LOTUS_TRIANGLE_MODE = 0x0004,
    LOTUS_QUAD_MODE = 0x0007,
} Lotus_Draw_Mode;

typedef struct Lotus_Renderer {
    ubyte8 draws;
    ubyte8 passes;
    Lotus_Shader* shader;
    Lotus_Mat4 projection;
    Lotus_Draw_Mode mode;
    Lotus_Vec4 clear_color;
    Lotus_Draw_Buffer* queue;
} Lotus_Renderer;

void lotus_init_renderer(void);
void lotus_draw_begin(Lotus_Draw_Mode mode, f32 r, f32 g, f32 b, f32 a, Lotus_Mat4 projection);
void lotus_draw_submit(ubyte4 vbo, ubyte4 ebo, ubyte4 vao, Lotus_Mat4* matrix, ubyte4 index_count, ubyte4 vertex_count);
void lotus_draw_flush(void);
void lotus_destroy_renderer(void);

LOTUS_API_ENTRY void lotus_set_renderer_clear_color(f32 r, f32 g, f32 b, f32 a);
LOTUS_API_ENTRY void lotus_set_renderer_shader(Lotus_Shader* shader);

LOTUS_API_ENTRY Lotus_Shader lotus_make_shader(const char* vertex_shader, const char* fragment_shader);
LOTUS_API_ENTRY void lotus_destroy_shader(Lotus_Shader* shader);

LOTUS_API_ENTRY void lotus_set_shader_uniform(Lotus_Shader program, const char* name, void* value);
LOTUS_API_ENTRY Lotus_Uniform lotus_get_shader_uniform(Lotus_Shader program, const char* name);
LOTUS_API_ENTRY void lotus_send_shader_uniform(Lotus_Shader program, Lotus_Uniform_Type type, const char* name);
