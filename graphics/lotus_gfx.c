#include "lotus_gfx.h"

#include "lotus_glapi.h"

lotus_buffer lotus_make_buffer(lotus_buffer_attr attrs, lotus_memory mem, const lotus_memory* indices) {
    lotus_buffer buf = {
        .attrs = attrs,
        .mem = mem,
        .handle[LOTUS_BUFFER_VBO] = -1,
        .handle[LOTUS_BUFFER_EBO] = -1,
        .handle[LOTUS_BUFFER_VAO] = -1
    };

    if ((attrs & ~((1 << SO_VERTEX_ATTRIBS) - 1)) != 0 || !mem.ptr) {
        return buf;
    }

    // calculate stride and offsets dynamically
    ubyte4 stride = 0;
    ubyte4 offsets[SO_VERTEX_ATTRIBS] = {0};
    const ubyte4 attr_sizes[SO_VERTEX_ATTRIBS] = {
        3, // LOTUS_LOCATION_ATTR: vec3
        3, // LOTUS_COLOR_ATTR: vec3
        2, // LOTUS_TCOORD_ATTR: vec2
        2  // LOTUS_NORMAL_ATTR: vec2
    };
    
    for (int i = 0; i < SO_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            // accumulate stride for enabled attributes
            offsets[i] = stride;
            stride += attr_sizes[i];
        }
    }

    lotus_gen_vertex_arrays(1, &buf.handle[LOTUS_BUFFER_VAO]);
    lotus_gen_buffers(1, &buf.handle[LOTUS_BUFFER_VBO]);
    
    lotus_bind_vertex_array(buf.handle[LOTUS_BUFFER_VAO]);
    lotus_bind_buffer(LOTUS_ARRAY_BUFFER, buf.handle[LOTUS_BUFFER_VBO]);
    
    lotus_buffer_data(LOTUS_ARRAY_BUFFER, mem.size, mem.ptr, LOTUS_STATIC_DRAW);

    // generate EBO if indices are provided
    if (indices && indices->size > 0 && indices->ptr != (void*)0) {
        lotus_gen_buffers(1, &buf.handle[LOTUS_BUFFER_EBO]);
        lotus_bind_buffer(LOTUS_ELEMENT_ARRAY_BUFFER, buf.handle[LOTUS_BUFFER_EBO]);
        lotus_buffer_data(LOTUS_ELEMENT_ARRAY_BUFFER, indices->size, indices->ptr, LOTUS_STATIC_DRAW);
    }

    // configure vertex attributes dynamically
    for (int i = 0; i < SO_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            lotus_vertex_attrib_pointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            lotus_enable_vertex_attrib_array(i);
        }
    }

    lotus_bind_buffer(LOTUS_ARRAY_BUFFER, 0);
    lotus_bind_vertex_array(0);

    return buf;
}

