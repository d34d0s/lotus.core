#pragma once

#if defined(LOTUS_PLATFORM_WINDOWS)
    #include <GL/gl.h>
    #include <GL/glu.h>
#elif defined(LOTUS_PLATFORM_MACOS)
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl.h>
    #include <dlfcn.h>
#elif defined(LOTUS_PLATFORM_LINUX)
    #include <GL/gl.h>
    #include <dlfcn.h>
#endif

#include <stddef.h>
#include <stdbool.h>

#include "../types.h"
#include "../defines.h"

ubyte lotus_load_fptrs(void);
void *lotus_get_fptr(const char *name);

// BUFFER FUNCTIONS

extern void (*lgl_gen_buffers)(sbyte4 count, ubyte4 *buffer);
extern void (*lgl_bind_buffer)(ubyte4 target, ubyte4 buffer);
extern void (*lgl_buffer_data)(ubyte4 target, sbyte4 size, const void *data, ubyte4 usage);

extern void (*lgl_delete_buffers)(sbyte4 count, const ubyte4 *buffer);
extern void (*lgl_map_buffer)(ubyte4 target, ubyte4 access);
extern void (*lgl_unmap_buffer)(ubyte4 target);
extern void (*lgl_buffer_sub_data)(ubyte4 target, ubyte4* offset, sbyte4 size, const void *data);
extern void (*lgl_get_buffer_parameteriv)(ubyte4 target, ubyte4 pname, ubyte4 *params);

// VERTEX ATTRIBUTE FUNCTIONS

extern void (*lgl_enable_vertex_attrib_array)(ubyte4 index);
extern void (*lgl_disable_vertex_attrib_array)(ubyte4 index);
extern void (*lgl_vertex_attrib_pointer)(ubyte4 index, ubyte4 size, ubyte4 type, ubyte normalized, sbyte4 stride, const void *pointer);
extern void (*lgl_bind_vertex_array)(ubyte4 array);
extern void (*lgl_gen_vertex_arrays)(sbyte4 count, ubyte4 *arrays);
extern void (*lgl_delete_vertex_arrays)(sbyte4 count, const ubyte4 *arrays);

// SHADER FUNCTIONS

extern ubyte4 (*lgl_create_program)(void);
extern void (*lgl_use_program)(ubyte4 program);
extern void (*lgl_link_program)(ubyte4 program);
extern void (*lgl_delete_program)(ubyte4 program);

extern ubyte4 (*lgl_create_shader)(ubyte4 type);
extern void (*lgl_compile_shader)(ubyte4 shader);
extern void (*lgl_attach_shader)(ubyte4 program, ubyte4 shader);
extern void (*lgl_detach_shader)(ubyte4 program, ubyte4 shader);
extern void (*lgl_delete_shader)(ubyte4 shader);
extern void (*lgl_shader_source)(ubyte4 shader, sbyte4 count, const char **strings, const int *lengths);
extern void (*lgl_get_shaderiv)(ubyte4 shader, ubyte4 pname, ubyte4 *params);
extern void (*lgl_get_shader_info_log)(ubyte4 shader, sbyte4 maxLength, sbyte4 *length, char *infoLog);
extern void (*lgl_get_programiv)(ubyte4 program, ubyte4 pname, ubyte4 *params);
extern void (*lgl_get_program_info_log)(ubyte4 program, sbyte4 maxLength, sbyte4 *length, char *infoLog);
extern ubyte4 (*lgl_get_uniform_location)(ubyte4 program, const char* name);
extern void (*lgl_uniform2fv)(ubyte4 location, ubyte4 count, const f32* value);
extern void (*lgl_uniform3fv)(ubyte4 location, ubyte4 count, const f32* value);
extern void (*lgl_uniform4fv)(ubyte4 location, ubyte4 count, const f32* value);
extern void (*lgl_uniform_matrix4fv)(ubyte4 location, ubyte4 count, ubyte4 transpose, const f32* value);

// TEXTURE FUNCTIONS

extern void (*lgl_gen_textures)(sbyte4 count, ubyte4 *textures);
extern void (*lgl_bind_texture)(ubyte4 target, ubyte4 texture);
extern void (*lgl_tex_parameteri)(ubyte4 target, ubyte4 pname, ubyte4 param);
extern void (*lgl_tex_image2d)(ubyte4 target, ubyte4 level, ubyte4 internalFormat, sbyte4 width, sbyte4 height, ubyte4 border, ubyte4 format, ubyte4 type, const void *data);
extern void (*lgl_active_texture)(ubyte4 texture);
extern void (*lgl_delete_textures)(sbyte4 count, const ubyte4 *textures);
extern void (*lgl_generate_mipmap)(ubyte4 target);
extern void (*lgl_generate_texture_mipmap)(ubyte4 texture);

// FRAMEBUFFER FUNCTIONS

extern void (*lgl_gen_framebuffers)(sbyte4 count, ubyte4 *framebuffers);
extern void (*lgl_bind_framebuffer)(ubyte4 target, ubyte4 framebuffer);
extern void (*lgl_framebuffer_texture2d)(ubyte4 target, ubyte4 attachment, ubyte4 textarget, ubyte4 texture, ubyte4 level);
extern void (*lgl_delete_framebuffers)(sbyte4 count, const ubyte4 *framebuffers);
extern ubyte4 (*lgl_check_framebuffer_status)(ubyte4 target);

// DRAWING FUNCTIONS

extern void (*lgl_clear)(sbyte4 mask);
extern void (*lgl_draw_arrays)(ubyte4 mode, ubyte4 first, sbyte4 count);
extern void (*lgl_clear_color)(f32 r, f32 g, f32 b, f32 a);
extern void (*lgl_draw_elements)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices);
extern void (*lgl_draw_elements_base_vertex)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices, ubyte4 baseVertex);

// STATE MANAGEMENT FUNCTIONS

extern void (*lgl_enable)(ubyte4 cap);
extern void (*lgl_disable)(ubyte4 cap);
extern void (*lgl_blend_func)(ubyte4 sfactor, ubyte4 dfactor);
extern void (*lgl_cull_face)(ubyte4 mode);
extern void (*lgl_depth_func)(ubyte4 func);
extern void (*lgl_viewport)(ubyte4 x, ubyte4 y, sbyte4 width, sbyte4 height);
extern void (*lgl_polygon_mode)(ubyte4 face, ubyte4 mode);

// UTILITY FUNCTIONS

extern ubyte4 (*lgl_get_error)(void);
extern const ubyte *(*lgl_get_string)(const ubyte4 name);
