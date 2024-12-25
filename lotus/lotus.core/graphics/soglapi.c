#include "soglapi.h"

#include "../platform/sologger.h"

// BUFFER FUNCTIONS

void (*sogl_gen_buffers)(sbyte4 count, ubyte4 *buffer);
void (*sogl_bind_buffer)(ubyte4 target, ubyte4 buffer);
void (*sogl_buffer_data)(ubyte4 target, sbyte4 size, const void *data, ubyte4 usage);
void (*sogl_delete_buffers)(sbyte4 count, const ubyte4 *buffer);
void (*sogl_map_buffer)(ubyte4 target, ubyte4 access);
void (*sogl_unmap_buffer)(ubyte4 target);
void (*sogl_buffer_sub_data)(ubyte4 target, ubyte4* offset, sbyte4 size, const void *data);
void (*sogl_get_buffer_parameteriv)(ubyte4 target, ubyte4 pname, ubyte4 *params);

// VERTEX ATTRIBUTE FUNCTIONS

void (*sogl_enable_vertex_attrib_array)(ubyte4 index);
void (*sogl_disable_vertex_attrib_array)(ubyte4 index);
void (*sogl_vertex_attrib_pointer)(ubyte4 index, ubyte4 size, ubyte4 type, ubyte normalized, sbyte4 stride, const void *pointer);
void (*sogl_bind_vertex_array)(ubyte4 array);
void (*sogl_gen_vertex_arrays)(sbyte4 count, ubyte4 *arrays);
void (*sogl_delete_vertex_arrays)(sbyte4 count, const ubyte4 *arrays);

// SHADER FUNCTIONS

ubyte4 (*sogl_create_program)(void);
void (*sogl_use_program)(ubyte4 program);
void (*sogl_link_program)(ubyte4 program);
void (*sogl_delete_program)(ubyte4 program);

ubyte4 (*sogl_create_shader)(ubyte4 type);
void (*sogl_compile_shader)(ubyte4 shader);
void (*sogl_attach_shader)(ubyte4 program, ubyte4 shader);
void (*sogl_detach_shader)(ubyte4 program, ubyte4 shader);
void (*sogl_delete_shader)(ubyte4 shader);
void (*sogl_shader_source)(ubyte4 shader, sbyte4 count, const char **strings, const int *lengths);
void (*sogl_get_shaderiv)(ubyte4 shader, ubyte4 pname, ubyte4 *params);
void (*sogl_get_shader_info_log)(ubyte4 shader, sbyte4 maxLength, sbyte4 *length, char *infoLog);
void (*sogl_get_programiv)(ubyte4 program, ubyte4 pname, ubyte4 *params);
void (*sogl_get_program_info_log)(ubyte4 program, sbyte4 maxLength, sbyte4 *length, char *infoLog);
ubyte4 (*sogl_get_uniform_location)(ubyte4 program, const char* name);
void (*sogl_uniform2fv)(ubyte4 location, ubyte4 count, const f32* value);
void (*sogl_uniform3fv)(ubyte4 location, ubyte4 count, const f32* value);
void (*sogl_uniform4fv)(ubyte4 location, ubyte4 count, const f32* value);
void (*sogl_uniform_matrix4fv)(ubyte4 location, ubyte4 count, ubyte4 transpose, const f32* value);

// TEXTURE FUNCTIONS

void (*sogl_gen_textures)(sbyte4 count, ubyte4 *textures);
void (*sogl_bind_texture)(ubyte4 target, ubyte4 texture);
void (*sogl_tex_parameteri)(ubyte4 target, ubyte4 pname, ubyte4 param);
void (*sogl_tex_image2d)(ubyte4 target, ubyte4 level, ubyte4 internalFormat, sbyte4 width, sbyte4 height, ubyte4 border, ubyte4 format, ubyte4 type, const void *data);
void (*sogl_active_texture)(ubyte4 texture);
void (*sogl_delete_textures)(sbyte4 count, const ubyte4 *textures);
void (*sogl_generate_mipmap)(ubyte4 target);
void (*sogl_generate_texture_mipmap)(ubyte4 texture);

// FRAMEBUFFER FUNCTIONS

void (*sogl_gen_framebuffers)(sbyte4 count, ubyte4 *framebuffers);
void (*sogl_bind_framebuffer)(ubyte4 target, ubyte4 framebuffer);
void (*sogl_framebuffer_texture2d)(ubyte4 target, ubyte4 attachment, ubyte4 textarget, ubyte4 texture, ubyte4 level);
void (*sogl_delete_framebuffers)(sbyte4 count, const ubyte4 *framebuffers);
ubyte4 (*sogl_check_framebuffer_status)(ubyte4 target);

// DRAWING FUNCTIONS

void (*sogl_clear)(sbyte4 mask);
void (*sogl_draw_arrays)(ubyte4 mode, ubyte4 first, sbyte4 count);
void (*sogl_clear_color)(f32 r, f32 g, f32 b, f32 a);
void (*sogl_draw_elements)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices);
void (*sogl_draw_elements_base_vertex)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices, ubyte4 baseVertex);

// STATE MANAGEMENT FUNCTIONS

void (*sogl_enable)(ubyte4 cap);
void (*sogl_disable)(ubyte4 cap);
void (*sogl_blend_func)(ubyte4 sfactor, ubyte4 dfactor);
void (*sogl_cull_face)(ubyte4 mode);
void (*sogl_depth_func)(ubyte4 func);
void (*sogl_viewport)(ubyte4 x, ubyte4 y, sbyte4 width, sbyte4 height);
void (*sogl_polygon_mode)(ubyte4 face, ubyte4 mode);

// UTILITY FUNCTIONS

ubyte4 (*sogl_get_error)(void);
const ubyte *(*sogl_get_string)(const ubyte4 name);

void *sogl_get_fptr(const char *name) {
    void* proc = (void*)wglGetProcAddress(name);
    if (
        proc == NULL        ||
        (proc ==(void*)0x1) || 
        (proc ==(void*)0x2) || 
        (proc ==(void*)0x3) || 
        (proc ==(void*)-1)) {
        proc = (void*)GetProcAddress(LoadLibrary("opengl32.dll"), name);
    }; return proc;
}

ubyte sogl_load_fptrs(void) {
    sogl_set_log_level(SOGL_LOG_ERROR);
    ubyte result = SOGL_TRUE;

    struct {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&sogl_gen_buffers, "glGenBuffers"},
        {(void**)&sogl_bind_buffer, "glBindBuffer"},
        {(void**)&sogl_buffer_data, "glBufferData"},
        {(void**)&sogl_map_buffer, "glMapBuffer"},
        {(void**)&sogl_unmap_buffer, "glUnmapBuffer"},
        {(void**)&sogl_buffer_sub_data, "glBufferSubData"},
        {(void**)&sogl_get_buffer_parameteriv, "glGetBufferParameteriv"},
        {(void**)&sogl_delete_buffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&sogl_enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {(void**)&sogl_disable_vertex_attrib_array, "glDisableVertexAttribArray"},
        {(void**)&sogl_vertex_attrib_pointer, "glVertexAttribPointer"},
        {(void**)&sogl_bind_vertex_array, "glBindVertexArray"},
        {(void**)&sogl_gen_vertex_arrays, "glGenVertexArrays"},
        {(void**)&sogl_delete_vertex_arrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&sogl_create_program, "glCreateProgram"},
        {(void**)&sogl_use_program, "glUseProgram"},
        {(void**)&sogl_link_program, "glLinkProgram"},
        {(void**)&sogl_delete_program, "glDeleteProgram"},
        {(void**)&sogl_create_shader, "glCreateShader"},
        {(void**)&sogl_compile_shader, "glCompileShader"},
        {(void**)&sogl_attach_shader, "glAttachShader"},
        {(void**)&sogl_detach_shader, "glDetachShader"},
        {(void**)&sogl_delete_shader, "glDeleteShader"},
        {(void**)&sogl_shader_source, "glShaderSource"},
        {(void**)&sogl_get_shaderiv, "glGetShaderiv"},
        {(void**)&sogl_get_shader_info_log, "glGetShaderInfoLog"},
        {(void**)&sogl_get_programiv, "glGetProgramiv"},
        {(void**)&sogl_get_program_info_log, "glGetProgramInfoLog"},
        {(void**)&sogl_get_uniform_location, "glGetUniformLocation"},
        {(void**)&sogl_uniform2fv, "glUniform2fv"},
        {(void**)&sogl_uniform3fv, "glUniform3fv"},
        {(void**)&sogl_uniform4fv, "glUniform4fv"},
        {(void**)&sogl_uniform_matrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&sogl_gen_textures, "glGenTextures"},
        {(void**)&sogl_bind_texture, "glBindTexture"},
        {(void**)&sogl_tex_parameteri, "glTexParameteri"},
        {(void**)&sogl_tex_image2d, "glTexImage2D"},
        {(void**)&sogl_active_texture, "glActiveTexture"},
        {(void**)&sogl_delete_textures, "glDeleteTextures"},
        {(void**)&sogl_generate_mipmap, "glGenerateMipmap"},
        {(void**)&sogl_generate_texture_mipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&sogl_gen_framebuffers, "glGenFramebuffers"},
        {(void**)&sogl_bind_framebuffer, "glBindFramebuffer"},
        {(void**)&sogl_framebuffer_texture2d, "glFramebufferTexture2D"},
        {(void**)&sogl_delete_framebuffers, "glDeleteFramebuffers"},
        {(void**)&sogl_check_framebuffer_status, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&sogl_clear, "glClear"},
        {(void**)&sogl_clear_color, "glClearColor"},
        {(void**)&sogl_draw_arrays, "glDrawArrays"},
        {(void**)&sogl_draw_elements, "glDrawElements"},
        {(void**)&sogl_draw_elements_base_vertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&sogl_enable, "glEnable"},
        {(void**)&sogl_disable, "glDisable"},
        {(void**)&sogl_blend_func, "glBlendFunc"},
        {(void**)&sogl_cull_face, "glCullFace"},
        {(void**)&sogl_depth_func, "glDepthFunc"},
        {(void**)&sogl_viewport, "glViewport"},
        {(void**)&sogl_polygon_mode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&sogl_get_error, "glGetError"},
        {(void**)&sogl_get_string, "glGetString"}
    };

    // Load all functions
    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = sogl_get_fptr(functions[i].name);
        if (!*functions[i].function) {
            sogl_log_error("Failed to load GL function: %s\n", functions[i].name);
            result = SOGL_FALSE;
        }
    }

    return result;
}
