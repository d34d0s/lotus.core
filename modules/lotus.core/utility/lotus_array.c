#include "lotus_array.h"

lotus_array lotus_make_array(lotus_allocator* alloc, ubyte8 length, ubyte8 stride) {
    lotus_array arr;

    size_t header_size = 3 * sizeof(ubyte8);

    arr.mem = lotus_alloc(alloc, header_size + (length * stride), LOTUS_ALIGNOF(void*));

    arr.header = (ubyte8*)arr.mem.ptr;
    
    arr.header[0] = 0;          // current size
    arr.header[1] = stride;     // stride of each element
    arr.header[2] = length;     // length of array

    arr.content = ((char*)arr.mem.ptr) + header_size;

    return arr;
}

void lotus_destroy_array(lotus_allocator* alloc, lotus_array array) {
    lotus_free(alloc, array.mem);
    array.header = NULL;
    array.content = NULL;
}

void lotus_resize_array(lotus_allocator* alloc, lotus_array* array, ubyte8 new_length) {
    size_t header_size = 3 * sizeof(ubyte8);
    size_t new_size = header_size + (new_length * lotus_get_array_attr(*array, LOTUS_ARRAY_STRIDE));
    
    // allocate new memory
    lotus_memory new_mem = lotus_alloc(alloc, new_size, LOTUS_ALIGNOF(void*));
    ubyte8* new_header = (ubyte8*)new_mem.ptr;
    void* new_content = ((char*)new_mem.ptr) + header_size;
    
    // copy old data
    memcpy(new_header, array->header, header_size);
    memcpy(new_content, array->content, array->header[LOTUS_ARRAY_SIZE] * array->header[LOTUS_ARRAY_STRIDE]);

    // free old memory
    lotus_free(alloc, array->mem);

    // update array
    array->mem = new_mem;
    array->header = new_header;
    array->content = new_content;
    array->header[LOTUS_ARRAY_LENGTH] = new_length;
}

void lotus_push_array(lotus_allocator* alloc, lotus_array* array, const lotus_memory* element) {
    ubyte8 size = lotus_get_array_attr(*array, LOTUS_ARRAY_SIZE);
    ubyte8 length = lotus_get_array_attr(*array, LOTUS_ARRAY_LENGTH);
    ubyte8 stride = lotus_get_array_attr(*array, LOTUS_ARRAY_STRIDE);

    // resize if necessary
    if (size >= length) {
        lotus_resize_array(alloc, array, length * 2);
    }

    // add element
    memcpy((char*)array->content + (size * stride), element->ptr, stride);
    array->header[LOTUS_ARRAY_SIZE]++;
}

lotus_memory _lotus_pop_array(lotus_array* array) {
    lotus_memory element = { .ptr = NULL, .size = 0 };

    // check if the array has elements to pop
    if (array->header[LOTUS_ARRAY_SIZE] > 0) {
        ubyte8 size = array->header[LOTUS_ARRAY_SIZE];
        ubyte8 stride = array->header[LOTUS_ARRAY_STRIDE];

        // point to the last element in the array
        element.ptr = (char*)array->content + ((size - 1) * stride);
        element.size = stride;

        // decrease the size of the array
        array->header[LOTUS_ARRAY_SIZE]--;
    }

    return element;
}
