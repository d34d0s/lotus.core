#include "sogfx.h"

#include "soglapi.h"

sogl_buffer sogl_make_buffer(sogl_buffer_attr attrs, sogl_memory mem, const sogl_memory* indices) {
    sogl_buffer buf = {
        .attrs = attrs,
        .mem = mem,
        .handle[SOGL_BUFFER_VBO] = -1,
        .handle[SOGL_BUFFER_EBO] = -1,
        .handle[SOGL_BUFFER_VAO] = -1
    };

    if ((attrs & ~((1 << SO_VERTEX_ATTRIBS) - 1)) != 0 || !mem.ptr) {
        return buf;
    }

    // calculate stride and offsets dynamically
    ubyte4 stride = 0;
    ubyte4 offsets[SO_VERTEX_ATTRIBS] = {0};
    const ubyte4 attr_sizes[SO_VERTEX_ATTRIBS] = {
        3, // SOGL_LOCATION_ATTR: vec3
        3, // SOGL_COLOR_ATTR: vec3
        2, // SOGL_TCOORD_ATTR: vec2
        2  // SOGL_NORMAL_ATTR: vec2
    };
    
    for (int i = 0; i < SO_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            // accumulate stride for enabled attributes
            offsets[i] = stride;
            stride += attr_sizes[i];
        }
    }

    sogl_gen_vertex_arrays(1, &buf.handle[SOGL_BUFFER_VAO]);
    sogl_gen_buffers(1, &buf.handle[SOGL_BUFFER_VBO]);
    
    sogl_bind_vertex_array(buf.handle[SOGL_BUFFER_VAO]);
    sogl_bind_buffer(SOGL_ARRAY_BUFFER, buf.handle[SOGL_BUFFER_VBO]);
    
    sogl_buffer_data(SOGL_ARRAY_BUFFER, mem.size, mem.ptr, SOGL_STATIC_DRAW);

    // generate EBO if indices are provided
    if (indices && indices->size > 0 && indices->ptr != (void*)0) {
        sogl_gen_buffers(1, &buf.handle[SOGL_BUFFER_EBO]);
        sogl_bind_buffer(SOGL_ELEMENT_ARRAY_BUFFER, buf.handle[SOGL_BUFFER_EBO]);
        sogl_buffer_data(SOGL_ELEMENT_ARRAY_BUFFER, indices->size, indices->ptr, SOGL_STATIC_DRAW);
    }

    // configure vertex attributes dynamically
    for (int i = 0; i < SO_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            sogl_vertex_attrib_pointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            sogl_enable_vertex_attrib_array(i);
        }
    }

    sogl_bind_buffer(SOGL_ARRAY_BUFFER, 0);
    sogl_bind_vertex_array(0);

    return buf;
}

