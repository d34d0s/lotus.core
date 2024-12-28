#pragma once

#include "../defines.h"

#include "../utility/lotus_math.h"
#include "../memory/lotus_memory.h"

#define LOTUS_MAX_DRAW_BUFFERS 255

#define LOTUS_BUFFER_VBO 0
#define LOTUS_BUFFER_EBO 1
#define LOTUS_BUFFER_VAO 2

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

typedef struct lotus_renderer {
    ubyte4 mode;
    ubyte8 draws;
    ubyte8 passes;
    ubyte4 shader;
    lotus_mat4 projection;
    lotus_draw_buffer* queue;
} lotus_renderer;

void lotus_renderer_init(void);
void lotus_renderer_begin(ubyte4 mode, lotus_mat4 projection);
void lotus_renderer_submit(ubyte4 vbo, ubyte4 ebo, ubyte4 vao, lotus_mat4 matrix, ubyte4 index_count, ubyte4 vertex_count);
void lotus_renderer_flush(void);
void lotus_renderer_destroy(void);

// lotus_draw_buffer lotus_make_draw_buffer(lotus_buffer_attr attrs, lotus_memory vertices, const lotus_memory* indices);
