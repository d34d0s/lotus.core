// soalloc.h

#pragma once

#include "../defines.h"

/**
 * @brief A structure used to represent a memory blob with its pointer and size.
 * 
 * The `lotus_memory` structure is used to pass around memory blobs.
 * It contains a pointer to the memory and its size in bytes.
 */
typedef struct lotus_memory {
    void* ptr;      /**< Pointer to the allocated memory block. */
    size_t size;    /**< Size of the allocated memory block in bytes. */
} lotus_memory;

/**
 * @brief Metadata stored at the start of each memory block in the allocator's free list.
 * 
 * The `lotus_memory_itf` structure holds metadata about a memory block, such as its size, 
 * whether it's free, and a pointer to the next block in the free list.
 */
typedef struct lotus_memory_itf {
    size_t size;                    /**< Size of the memory block (excluding metadata). */
    ubyte is_free;                  /**< Whether the block is free (1 = free, 0 = allocated). */
    struct lotus_memory_itf* next;   /**< Pointer to the next block in the free list. */
} lotus_memory_itf;

/**
 * @brief The allocator state, managing memory allocation and free lists.
 * 
 * The `lotus_allocator` structure represents the allocator's state, including the 
 * start of the heap, total heap size, and the linked list of free blocks.
 */
typedef struct lotus_allocator {
    void* heap_start;               /**< Pointer to the start of the allocated heap. */
    size_t heap_size;               /**< Total size of the heap. */
    lotus_memory_itf* free_list;     /**< Linked list of free memory blocks. */
} lotus_allocator;

/**
 * @brief Helper function to align memory addresses to a specified alignment.
 * 
 * This function calculates the aligned size for memory allocation.
 * 
 * @param size The size of the memory to align.
 * @param alignment The alignment value (must be a power of 2).
 * @return The aligned memory size.
 */
static inline size_t lotus_align(size_t size, size_t alignment) {
    return (size + (alignment - 1)) & ~(alignment - 1);
}

/**
 * @brief Macro to create a `lotus_memory` structure from a variable.
 * 
 * This macro initializes a `lotus_memory` struct from a variable (e.g., an array, struct, 
 * or primitive type). It is commonly used to pass known data types as memory blobs.
 * 
 * @param x The variable to wrap in a `lotus_memory` struct.
 * @return A `lotus_memory` structure containing a pointer to `x` and its size.
 */
#define LOTUS_MEMORY(x) (lotus_memory){ &x, sizeof(x) }

/**
 * @brief Macro to create a reference to a `lotus_memory` structure from a variable.
 * 
 * This macro returns a pointer to a temporary `lotus_memory` struct created from a variable 
 * (e.g., an array, struct, or primitive type). It is useful when passing `lotus_memory` by reference.
 * 
 * @param x The variable to wrap in a `lotus_memory` struct.
 * @return A pointer to a temporary `lotus_memory` structure containing a pointer to `x` and its size.
 */
#define LOTUS_MEMORY_REF(x) &(lotus_memory){ &x, sizeof(x) }

/**
 * @brief Macro to get the size of a type using `sizeof`.
 * 
 * @param type The type whose size is required.
 * @return The size of the type in bytes.
 */
#define LOTUS_SIZEOF(type) sizeof(type)

/**
 * @brief Macro to get the alignment of a type using `offsetof`.
 * 
 * This macro calculates the alignment of a type using a technique involving `offsetof`.
 * It ensures that memory allocations align properly.
 * 
 * @param type The type whose alignment is required.
 * @return The alignment of the type in bytes.
 */
#define LOTUS_ALIGNOF(type) offsetof(struct { char c; type t; }, t)

/**
 * @brief Creates and initializes a memory allocator with a specified heap size.
 * 
 * This function sets up an allocator that manages a heap of the specified size.
 * It prepares the allocator to manage memory blocks using the heap.
 * 
 * @param heap_size The size of the heap to allocate.
 * @return A `lotus_allocator` struct containing the initialized allocator state.
 */
LOTUS_API_ENTRY lotus_allocator lotus_make_allocator(size_t heap_size);

/**
 * @brief Destroys the allocator and frees the heap memory.
 * 
 * This function destroys the given allocator and frees any memory used by it.
 * It is important to call this function when the allocator is no longer needed.
 * 
 * @param alloc A pointer to the `lotus_allocator` to destroy.
 */
LOTUS_API_ENTRY void lotus_destroy_allocator(lotus_allocator* alloc);

/**
 * @brief Allocates a memory block from the allocator with specified size and alignment.
 * 
 * This function allocates a memory block of the given size from the allocator,
 * ensuring that the memory is aligned to the specified alignment boundary.
 * 
 * @param alloc A pointer to the `lotus_allocator` managing the allocation.
 * @param size The size of the memory block to allocate in bytes.
 * @param alignment The alignment of the memory block in bytes (must be a power of 2).
 * @return A `lotus_memory` struct containing the pointer to the allocated memory and its size.
 */
LOTUS_API_ENTRY lotus_memory lotus_alloc(lotus_allocator* alloc, size_t size, size_t alignment);

/**
 * @brief Frees a previously allocated memory block and returns it to the allocator.
 * 
 * This function marks the memory block as free and potentially coalesces it with adjacent free blocks
 * in the free list to reduce fragmentation.
 * 
 * @param alloc A pointer to the `lotus_allocator` managing the memory block.
 * @param mem The `lotus_memory` struct representing the memory block to free.
 */
LOTUS_API_ENTRY void lotus_free(lotus_allocator* alloc, lotus_memory mem);