#include "soarray.h"

sogl_array sogl_make_array(sogl_allocator* alloc, ubyte8 length, ubyte8 stride) {
    sogl_array arr;

    size_t header_size = 3 * sizeof(ubyte8);

    arr.mem = sogl_alloc(alloc, header_size + (length * stride), SOGL_ALIGNOF(void*));

    arr.header = (ubyte8*)arr.mem.ptr;
    
    arr.header[0] = 0;          // current size
    arr.header[1] = stride;     // stride of each element
    arr.header[2] = length;     // length of array

    arr.content = ((char*)arr.mem.ptr) + header_size;

    return arr;
}

void sogl_destroy_array(sogl_allocator* alloc, sogl_array array) {
    sogl_free(alloc, array.mem);
    array.header = NULL;
    array.content = NULL;
}

void sogl_resize_array(sogl_allocator* alloc, sogl_array* array, ubyte8 new_length) {
    size_t header_size = 3 * sizeof(ubyte8);
    size_t new_size = header_size + (new_length * sogl_get_array_attr(*array, SOGL_ARRAY_STRIDE));
    
    // allocate new memory
    sogl_memory new_mem = sogl_alloc(alloc, new_size, SOGL_ALIGNOF(void*));
    ubyte8* new_header = (ubyte8*)new_mem.ptr;
    void* new_content = ((char*)new_mem.ptr) + header_size;
    
    // copy old data
    memcpy(new_header, array->header, header_size);
    memcpy(new_content, array->content, array->header[SOGL_ARRAY_SIZE] * array->header[SOGL_ARRAY_STRIDE]);

    // free old memory
    sogl_free(alloc, array->mem);

    // update array
    array->mem = new_mem;
    array->header = new_header;
    array->content = new_content;
    array->header[SOGL_ARRAY_LENGTH] = new_length;
}

void sogl_push_array(sogl_allocator* alloc, sogl_array* array, const sogl_memory* element) {
    ubyte8 size = sogl_get_array_attr(*array, SOGL_ARRAY_SIZE);
    ubyte8 length = sogl_get_array_attr(*array, SOGL_ARRAY_LENGTH);
    ubyte8 stride = sogl_get_array_attr(*array, SOGL_ARRAY_STRIDE);

    // resize if necessary
    if (size >= length) {
        sogl_resize_array(alloc, array, length * 2);
    }

    // add element
    memcpy((char*)array->content + (size * stride), element->ptr, stride);
    array->header[SOGL_ARRAY_SIZE]++;
}

sogl_memory _sogl_pop_array(sogl_array* array) {
    sogl_memory element = { .ptr = NULL, .size = 0 };

    // check if the array has elements to pop
    if (array->header[SOGL_ARRAY_SIZE] > 0) {
        ubyte8 size = array->header[SOGL_ARRAY_SIZE];
        ubyte8 stride = array->header[SOGL_ARRAY_STRIDE];

        // point to the last element in the array
        element.ptr = (char*)array->content + ((size - 1) * stride);
        element.size = stride;

        // decrease the size of the array
        array->header[SOGL_ARRAY_SIZE]--;
    }

    return element;
}
