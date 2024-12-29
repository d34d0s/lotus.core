#include "../include/graphics/lotus_gl.h"
#include "../include/graphics/lotus_glapi.h"

#include "../include/utility/lotus_array.h"

static Lotus_Renderer internal_renderer_state = {0};

void lotus_init_renderer(void) {
    memset(&internal_renderer_state, 0, sizeof(Lotus_Renderer));

    internal_renderer_state.draws = 0;
    internal_renderer_state.passes = 0;
    internal_renderer_state.shader = NULL;
    internal_renderer_state.mode = GL_TRIANGLES;
    internal_renderer_state.projection = lotus_identity();
    internal_renderer_state.clear_color = LOTUS_COLOR_V4(133, 161, 172, 255);

    internal_renderer_state.queue = lotus_make_array(sizeof(Lotus_Draw_Buffer), LOTUS_MAX_DRAW_BUFFERS);

    lgl_enable(GL_DEPTH_TEST);
    lgl_enable(GL_BLEND);
    lgl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void lotus_draw_begin(Lotus_Draw_Mode mode, f32 r, f32 g, f32 b, f32 a, Lotus_Mat4 projection) {
    internal_renderer_state.draws = 0;
    internal_renderer_state.mode = mode;
    internal_renderer_state.projection = projection;
    internal_renderer_state.clear_color = LOTUS_COLOR_V4(r, g, b, a);
}

void lotus_set_renderer_clear_color(f32 r, f32 g, f32 b, f32 a) {
    internal_renderer_state.clear_color = LOTUS_COLOR_V4(r, g, b, a);
}

void lotus_set_renderer_shader(Lotus_Shader* shader) {
    internal_renderer_state.shader = (shader == NULL || shader->uniforms == NULL || shader->program < 0) ? NULL : shader;
    lotus_set_shader_uniform(*internal_renderer_state.shader, "u_projection", &internal_renderer_state.projection);
}

void lotus_draw_submit(ubyte4 vbo, ubyte4 ebo, ubyte4 vao, Lotus_Mat4* matrix, ubyte4 index_count, ubyte4 vertex_count) {
    if (internal_renderer_state.draws >= LOTUS_MAX_DRAW_BUFFERS) {
        return; // error: exceeded max draw buffers
    }
    
    ubyte8 index = internal_renderer_state.draws;
    lotus_insert_array(internal_renderer_state.queue, index, &(Lotus_Draw_Buffer){
        .handle[LOTUS_BUFFER_VBO] = vbo,
        .handle[LOTUS_BUFFER_VAO] = vao,
        .handle[LOTUS_BUFFER_EBO] = ebo,
        .matrix = matrix,
        .index_count = index_count,
        .vertex_count = vertex_count
    });

    internal_renderer_state.draws++;
}

void lotus_draw_flush() {
    lgl_clear_color(
        internal_renderer_state.clear_color.x,
        internal_renderer_state.clear_color.y,
        internal_renderer_state.clear_color.z,
        internal_renderer_state.clear_color.w
    ); lgl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (ubyte8 i = 0; i < internal_renderer_state.draws; ++i) {
        Lotus_Draw_Buffer buf;
        lotus_pop_array(internal_renderer_state.queue, &buf);

        if (internal_renderer_state.shader) {
            lgl_use_program(internal_renderer_state.shader->program);
            
            lotus_send_shader_uniform(*internal_renderer_state.shader, LOTUS_UNIFORM_MAT4, "u_model");
            lotus_send_shader_uniform(*internal_renderer_state.shader, LOTUS_UNIFORM_MAT4, "u_projection");
        }

        lgl_bind_vertex_array(buf.handle[LOTUS_BUFFER_VAO]);

        if (buf.index_count > 0) {
            lgl_draw_elements(internal_renderer_state.mode, buf.index_count, GL_UNSIGNED_INT, NULL);
        } else {
            lgl_draw_arrays(internal_renderer_state.mode, 0, buf.vertex_count);
        }

        lgl_bind_vertex_array(0);
        internal_renderer_state.passes++;
    }
    internal_renderer_state.draws = 0;
}

void lotus_destroy_renderer() {
    internal_renderer_state.draws = 0;
    internal_renderer_state.passes = 0;
    internal_renderer_state.mode = -1;

    lotus_destroy_array(internal_renderer_state.queue);

    lgl_disable(GL_DEPTH_TEST);
    lgl_disable(GL_BLEND);
}

Lotus_Shader lotus_make_shader(const char* vertex_shader, const char* fragment_shader) {
    sbyte4 link = 0;
    sbyte4 compile = 0;

    Lotus_Shader program = {.uniforms=lotus_make_hashmap(16)};
    if (!program.uniforms) {
        printf("failed to allocate uniform hashmap!\n");
        return (Lotus_Shader){0};
    }

    program.program = lgl_create_program();
    sbyte4 v_Shader = lgl_create_shader(LOTUS_VERTEX_SHADER);
    sbyte4 f_Shader = lgl_create_shader(LOTUS_FRAGMENT_SHADER);

    lgl_shader_source(v_Shader, 1, &vertex_shader, NULL);
    lgl_compile_shader(v_Shader);
    lgl_get_shaderiv(v_Shader, LOTUS_COMPILE_STATUS, &compile);
    if (!compile) {
        printf("failed to compile vertex-shader!\n");
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }
    
    lgl_shader_source(f_Shader, 1, &fragment_shader, NULL);
    lgl_compile_shader(f_Shader);
    lgl_get_shaderiv(f_Shader, LOTUS_COMPILE_STATUS, &compile);
    if (!compile) {
        printf("failed to compile fragment-shader!\n");
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }

    lgl_attach_shader(program.program, v_Shader);
    lgl_attach_shader(program.program, f_Shader);
    lgl_link_program(program.program);
    lgl_get_programiv(program.program, LOTUS_LINK_STATUS, &link);
    if (!link) {
        printf("failed to link shader program!\n");
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }

    lgl_delete_shader(v_Shader);
    lgl_delete_shader(f_Shader);
    return program;

}

void lotus_destroy_shader(Lotus_Shader* shader) {
    lotus_destroy_hashmap(shader->uniforms);
    lgl_delete_program(shader->program);
}

void lotus_set_shader_uniform(Lotus_Shader program, const char* name, void* value) { lotus_set_hashmap(program.uniforms, name, value); }

void lotus_send_shader_uniform(Lotus_Shader program, Lotus_Uniform_Type type, const char* name) {
    Lotus_Uniform uniform = lotus_get_shader_uniform(program, name);
    if (uniform.location < 0) return;
    
    lgl_use_program(program.program);
    switch (type) {
        case LOTUS_UNIFORM_NONE: break;
        case LOTUS_UNIFORM_TYPES: break;
        case LOTUS_UNIFORM_VEC2: lgl_uniform2fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_VEC3: lgl_uniform3fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_VEC4: lgl_uniform4fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_MAT4: lgl_uniform_matrix4fv(uniform.location, 1, 0, uniform.value); break;
        default: break;
    }
}

Lotus_Uniform lotus_get_shader_uniform(Lotus_Shader program, const char* name) {
    sbyte4 location = lgl_get_uniform_location(program.program, name);
    if (location < 0) return (Lotus_Uniform){0};
    
    void* value = lotus_get_hashmap(program.uniforms, name);
    if (!value) return (Lotus_Uniform){0};
    
    return (Lotus_Uniform){.location=location, .name=name, .value=value};
}
