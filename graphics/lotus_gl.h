#pragma once

#include "../defines.h"

#include "../utility/lotus_math.h"
#include "../memory/lotus_memory.h"

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

typedef enum lotus_buffer_attr {
    LOTUS_LOCATION_ATTR = 1 << 0, // 1 (0b0001)
    LOTUS_COLOR_ATTR    = 1 << 1, // 2 (0b0010)
    LOTUS_TCOORD_ATTR   = 1 << 2, // 4 (0b0100)
    LOTUS_NORMAL_ATTR   = 1 << 3, // 8 (0b1000)
    SO_VERTEX_ATTRIBS  = 1 << 4 
} lotus_buffer_attr;

typedef struct lotus_draw_buffer {
    ubyte4 handle[3];
    lotus_mat4 matrix;
    ubyte4 index_count;
    ubyte4 vertex_count;
} lotus_draw_buffer;

// GL equivalents
typedef enum lotus_render_mode {
    LOTUS_POINT_MODE = 0x0000,
    LOTUS_LINE_MODE = 0x0001,
    LOTUS_TRIANGLE_MODE = 0x0004,
    LOTUS_QUAD_MODE = 0x0007,
} lotus_render_mode;

typedef struct lotus_renderer {
    ubyte8 draws;
    ubyte8 passes;
    ubyte4 shader;
    lotus_mat4 projection;
    lotus_render_mode mode;
    lotus_vec4 clear_color;
    lotus_draw_buffer* queue;
} lotus_renderer;

void lotus_renderer_init(void);
void lotus_renderer_begin(lotus_render_mode mode, f32 r, f32 g, f32 b, f32 a, lotus_mat4 projection);
void lotus_renderer_submit(ubyte4 vbo, ubyte4 ebo, ubyte4 vao, lotus_mat4 matrix, ubyte4 index_count, ubyte4 vertex_count);
void lotus_renderer_flush(void);
void lotus_renderer_destroy(void);

void lotus_renderer_clear_color(f32 r, f32 g, f32 b, f32 a);
