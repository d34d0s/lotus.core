#include "lotus_gfx.h"

#include "lotus_glapi.h"
#include "../utility/lotus_array.h"

static lotus_renderer internal_renderer_state = {0};

void lotus_renderer_init(void) {
    memset(&internal_renderer_state, 0, sizeof(lotus_renderer));

    internal_renderer_state.draws = 0;
    internal_renderer_state.passes = 0;
    internal_renderer_state.shader = -1;
    internal_renderer_state.mode = GL_TRIANGLES;
    internal_renderer_state.projection = lotus_identity();

    internal_renderer_state.queue = lotus_make_array(sizeof(lotus_draw_buffer), LOTUS_MAX_DRAW_BUFFERS);

    lotus_enable(GL_DEPTH_TEST);
    lotus_enable(GL_BLEND);
    lotus_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void lotus_renderer_begin(ubyte4 mode, lotus_mat4 projection) {
    internal_renderer_state.draws = 0;
    internal_renderer_state.mode = mode;
    internal_renderer_state.projection = projection;
}

void lotus_renderer_submit(ubyte4 vbo, ubyte4 ebo, ubyte4 vao, lotus_mat4 matrix, ubyte4 index_count, ubyte4 vertex_count) {
    if (internal_renderer_state.draws >= LOTUS_MAX_DRAW_BUFFERS) {
        return; // error: exceeded max draw buffers
    }

    ubyte8 index = internal_renderer_state.draws;
    lotus_insert_array(internal_renderer_state.queue, index, &(lotus_draw_buffer){
        .handle[LOTUS_BUFFER_VBO] = vbo,
        .handle[LOTUS_BUFFER_VAO] = vao,
        .handle[LOTUS_BUFFER_EBO] = ebo,
        .matrix = matrix,
        .index_count = index_count,
        .vertex_count = vertex_count
    });

    internal_renderer_state.draws++;
}

void lotus_renderer_flush() {
    for (ubyte8 i = 0; i < internal_renderer_state.draws; ++i) {
        lotus_draw_buffer buf;
        lotus_pop_array(internal_renderer_state.queue, &buf);

        lotus_bind_vertex_array(buf.handle[LOTUS_BUFFER_VAO]);

        // Set shader uniforms
        // lotus_set_uniform_mat4(internal_renderer_state.shader, "u_projection", &internal_renderer_state.projection);
        // lotus_set_uniform_mat4(internal_renderer_state.shader, "u_model", buf.matrix);

        // Issue draw call
        if (buf.handle[LOTUS_BUFFER_EBO] != -1) {
            lotus_draw_elements(internal_renderer_state.mode, buf.index_count, GL_UNSIGNED_INT, NULL);
        } else {
            lotus_draw_arrays(internal_renderer_state.mode, 0, buf.vertex_count);
        }

        lotus_bind_vertex_array(0);
        internal_renderer_state.passes++;
    }

    // Clear draw queue
    internal_renderer_state.draws = 0;
}

void lotus_renderer_destroy() {
    internal_renderer_state.draws = 0;
    internal_renderer_state.passes = 0;
    internal_renderer_state.mode = -1;

    lotus_destroy_array(internal_renderer_state.queue);

    lotus_disable(GL_DEPTH_TEST);
    lotus_disable(GL_BLEND);
}


// lotus_draw_buffer lotus_make_draw_buffer(lotus_buffer_attr attrs, lotus_memory vertices, const lotus_memory* indices) {
//     lotus_draw_buffer buf = {
//         .attrs = attrs,
//         .vertex_data = vertices,
//         .handle[LOTUS_BUFFER_VBO] = -1,
//         .handle[LOTUS_BUFFER_EBO] = -1,
//         .handle[LOTUS_BUFFER_VAO] = -1
//     };

//     if ((attrs & ~((1 << SO_VERTEX_ATTRIBS) - 1)) != 0 || !vertices.ptr) {
//         return buf;
//     }

//     calculate stride and offsets dynamically
//     ubyte4 stride = 0;
//     ubyte4 offsets[SO_VERTEX_ATTRIBS] = {0};
//     const ubyte4 attr_sizes[SO_VERTEX_ATTRIBS] = {
//         3, LOTUS_LOCATION_ATTR: vec3
//         3, LOTUS_COLOR_ATTR: vec3
//         2, LOTUS_TCOORD_ATTR: vec2
//         2  LOTUS_NORMAL_ATTR: vec2
//     };
    
//     for (int i = 0; i < SO_VERTEX_ATTRIBS; i++) {
//         if ((attrs & (1 << i)) != 0) {
//             accumulate stride for enabled attributes
//             offsets[i] = stride;
//             stride += attr_sizes[i];
//         }
//     }

//     lotus_gen_vertex_arrays(1, &buf.handle[LOTUS_BUFFER_VAO]);
//     lotus_gen_buffers(1, &buf.handle[LOTUS_BUFFER_VBO]);
    
//     lotus_bind_vertex_array(buf.handle[LOTUS_BUFFER_VAO]);
//     lotus_bind_buffer(LOTUS_ARRAY_BUFFER, buf.handle[LOTUS_BUFFER_VBO]);
    
//     lotus_buffer_data(LOTUS_ARRAY_BUFFER, vertices.size, vertices.ptr, LOTUS_STATIC_DRAW);

//     generate EBO if indices are provided
//     if (indices && indices->size > 0 && indices->ptr != (void*)0) {
//         buf.index_data = *indices;
//         lotus_gen_buffers(1, &buf.handle[LOTUS_BUFFER_EBO]);
//         lotus_bind_buffer(LOTUS_ELEMENT_ARRAY_BUFFER, buf.handle[LOTUS_BUFFER_EBO]);
//         lotus_buffer_data(LOTUS_ELEMENT_ARRAY_BUFFER, indices->size, indices->ptr, LOTUS_STATIC_DRAW);
//     }

//     configure vertex attributes dynamically
//     for (int i = 0; i < SO_VERTEX_ATTRIBS; i++) {
//         if ((attrs & (1 << i)) != 0) {
//             lotus_vertex_attrib_pointer(
//                 i, 
//                 attr_sizes[i], 
//                 GL_FLOAT, 
//                 GL_FALSE, 
//                 stride * sizeof(f32), 
//                 (void*)(offsets[i] * sizeof(f32))
//             );
//             lotus_enable_vertex_attrib_array(i);
//         }
//     }

//     lotus_bind_buffer(LOTUS_ARRAY_BUFFER, 0);
//     lotus_bind_vertex_array(0);

//     return buf;
// }

