#pragma once

#include "../defines.h"
#include "../memory/somemory.h"

/**
 * @enum sogl_array_attr
 * @brief Attributes used to access array metadata in `sogl_array`.
 */
typedef enum sogl_array_attr {
    SOGL_ARRAY_SIZE = 0,    /**< Number of elements currently in the array. */
    SOGL_ARRAY_STRIDE,      /**< Size of each element in bytes. */
    SOGL_ARRAY_LENGTH,      /**< Total allocated capacity of the array. */
    SOGL_ARRAY_ATTRS        /**< Total number of attributes. */
} sogl_array_attr;

/**
 * @struct sogl_array
 * @brief A dynamic array structure.
 * 
 * Contains allocated memory, header metadata, and content data.
 */
typedef struct sogl_array {
    sogl_memory mem;    /**< Allocated memory for the array. */
    ubyte8* header;     /**< Metadata header containing array attributes. */
    void* content;      /**< Pointer to the array's data. */
} sogl_array;

/**
 * @brief Retrieves a metadata attribute from a `sogl_array`.
 * 
 * @param array The array to query.
 * @param attr The attribute to retrieve (from `sogl_array_attr`).
 * @return The value of the attribute, or -1 if the attribute index is invalid.
 */
_SOGL_PRIVATE ubyte8 sogl_get_array_attr(sogl_array array, sogl_array_attr attr) {
    return (attr < SOGL_ARRAY_ATTRS) ? (ubyte8)array.header[attr] : -1;
}

/**
 * @brief Checks whether an index is valid for the given array.
 * 
 * @param array The array to query.
 * @param index The index to validate.
 * @return 1 if the index is valid, 0 otherwise.
 */
_SOGL_PRIVATE ubyte sogl_index_array(sogl_array array, ubyte8 index) {
    return (index <= sogl_get_array_attr(array, SOGL_ARRAY_SIZE) && index < sogl_get_array_attr(array, SOGL_ARRAY_LENGTH));
}

/**
 * @brief Safely retrieves an element from a `sogl_array`.
 * 
 * @param array The array to retrieve the element from.
 * @param index The index of the element.
 * @param type The type of the element.
 * @return The element at the given index, or -1 if the index is invalid.
 */
#define sogl_get_array(array, index, type) \
    (sogl_index_array(array, index)) ? ((type*)array.content)[index] : -1

/**
 * @brief Safely sets an element in a `sogl_array`.
 * 
 * @param array The array to modify.
 * @param index The index of the element.
 * @param type The type of the element.
 * @param value The value to set at the given index.
 */
#define sogl_set_array(array, index, type, value) \
    array.header[SOGL_ARRAY_SIZE]++;    \
    ((type*)array.content)[index] = (sogl_index_array(array, index)) ? value : -1

/**
 * @brief Creates a new `sogl_array` with the specified length and stride.
 * 
 * @param alloc The allocator to use.
 * @param length The initial capacity of the array.
 * @param stride The size of each element in bytes.
 * @return The created `sogl_array`.
 */
SOGL_API_ENTRY sogl_array sogl_make_array(sogl_allocator* alloc, ubyte8 length, ubyte8 stride);

/**
 * @brief Destroys a `sogl_array`, freeing its memory.
 * 
 * @param alloc The allocator to use.
 * @param array The array to destroy.
 */
SOGL_API_ENTRY void sogl_destroy_array(sogl_allocator* alloc, sogl_array array);

/**
 * @brief Resizes a `sogl_array` to a new length.
 * 
 * @param alloc The allocator to use.
 * @param array The array to resize.
 * @param new_length The new capacity of the array.
 */
SOGL_API_ENTRY void sogl_resize_array(sogl_allocator* alloc, sogl_array* array, ubyte8 new_length);

/**
 * @brief Pushes a new element to the end of a `sogl_array` as a blob.
 * 
 * @param alloc The allocator to use.
 * @param array The array to modify.
 * @param element A pointer to the element to push.
 */
SOGL_API_ENTRY void sogl_push_array(sogl_allocator* alloc, sogl_array* array, const sogl_memory* element);

/**
 * @brief Pops the last element from a `sogl_array` as a blob.
 * 
 * @param array The array to modify.
 * @return The popped element as a `sogl_memory` structure.
 */
SOGL_API_ENTRY sogl_memory _sogl_pop_array(sogl_array* array);

/**
 * @brief Pops the last element from a `sogl_array` as a typed value.
 * 
 * @param array The array to modify.
 * @param type The type of the element.
 * @return The popped element as the specified type.
 */
#define sogl_pop_array(array, type) *(type*)_sogl_pop_array(&array).ptr
