#pragma once

#include "../defines.h"
#include "../memory/lotus_memory.h"

/**
 * @enum lotus_array_attr
 * @brief Attributes used to access array metadata in `lotus_array`.
 */
typedef enum lotus_array_attr {
    LOTUS_ARRAY_SIZE = 0,    /**< Number of elements currently in the array. */
    LOTUS_ARRAY_STRIDE,      /**< Size of each element in bytes. */
    LOTUS_ARRAY_LENGTH,      /**< Total allocated capacity of the array. */
    LOTUS_ARRAY_ATTRS        /**< Total number of attributes. */
} lotus_array_attr;

/**
 * @struct lotus_array
 * @brief A dynamic array structure.
 * 
 * Contains allocated memory, header metadata, and content data.
 */
typedef struct lotus_array {
    lotus_memory mem;    /**< Allocated memory for the array. */
    ubyte8* header;     /**< Metadata header containing array attributes. */
    void* content;      /**< Pointer to the array's data. */
} lotus_array;

/**
 * @brief Retrieves a metadata attribute from a `lotus_array`.
 * 
 * @param array The array to query.
 * @param attr The attribute to retrieve (from `lotus_array_attr`).
 * @return The value of the attribute, or -1 if the attribute index is invalid.
 */
_LOTUS_PRIVATE ubyte8 lotus_get_array_attr(lotus_array array, lotus_array_attr attr) {
    return (attr < LOTUS_ARRAY_ATTRS) ? (ubyte8)array.header[attr] : -1;
}

/**
 * @brief Checks whether an index is valid for the given array.
 * 
 * @param array The array to query.
 * @param index The index to validate.
 * @return 1 if the index is valid, 0 otherwise.
 */
_LOTUS_PRIVATE ubyte lotus_index_array(lotus_array array, ubyte8 index) {
    return (index <= lotus_get_array_attr(array, LOTUS_ARRAY_SIZE) && index < lotus_get_array_attr(array, LOTUS_ARRAY_LENGTH));
}

/**
 * @brief Safely retrieves an element from a `lotus_array`.
 * 
 * @param array The array to retrieve the element from.
 * @param index The index of the element.
 * @param type The type of the element.
 * @return The element at the given index, or -1 if the index is invalid.
 */
#define lotus_get_array(array, index, type) \
    (lotus_index_array(array, index)) ? ((type*)array.content)[index] : -1

/**
 * @brief Safely sets an element in a `lotus_array`.
 * 
 * @param array The array to modify.
 * @param index The index of the element.
 * @param type The type of the element.
 * @param value The value to set at the given index.
 */
#define lotus_set_array(array, index, type, value) \
    array.header[LOTUS_ARRAY_SIZE]++;    \
    ((type*)array.content)[index] = (lotus_index_array(array, index)) ? value : -1

/**
 * @brief Creates a new `lotus_array` with the specified length and stride.
 * 
 * @param alloc The allocator to use.
 * @param length The initial capacity of the array.
 * @param stride The size of each element in bytes.
 * @return The created `lotus_array`.
 */
LOTUS_API_ENTRY lotus_array lotus_make_array(lotus_allocator* alloc, ubyte8 length, ubyte8 stride);

/**
 * @brief Destroys a `lotus_array`, freeing its memory.
 * 
 * @param alloc The allocator to use.
 * @param array The array to destroy.
 */
LOTUS_API_ENTRY void lotus_destroy_array(lotus_allocator* alloc, lotus_array array);

/**
 * @brief Resizes a `lotus_array` to a new length.
 * 
 * @param alloc The allocator to use.
 * @param array The array to resize.
 * @param new_length The new capacity of the array.
 */
LOTUS_API_ENTRY void lotus_resize_array(lotus_allocator* alloc, lotus_array* array, ubyte8 new_length);

/**
 * @brief Pushes a new element to the end of a `lotus_array` as a blob.
 * 
 * @param alloc The allocator to use.
 * @param array The array to modify.
 * @param element A pointer to the element to push.
 */
LOTUS_API_ENTRY void lotus_push_array(lotus_allocator* alloc, lotus_array* array, const lotus_memory* element);

/**
 * @brief Pops the last element from a `lotus_array` as a blob.
 * 
 * @param array The array to modify.
 * @return The popped element as a `lotus_memory` structure.
 */
LOTUS_API_ENTRY lotus_memory _lotus_pop_array(lotus_array* array);

/**
 * @brief Pops the last element from a `lotus_array` as a typed value.
 * 
 * @param array The array to modify.
 * @param type The type of the element.
 * @return The popped element as the specified type.
 */
#define lotus_pop_array(array, type) *(type*)_lotus_pop_array(&array).ptr
