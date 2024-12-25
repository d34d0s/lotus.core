# **soalloc.h** - Memory Abstraction

This header provides a memory abstraction with an allocator and custom memory blocks. It uses the `sogl_memory` structure to represent memory blocks and offers an allocator with functionality for allocating, freeing, and managing memory. It supports memory alignment and provides macros for ease of use.

## **Structures**

### `sogl_memory`
A structure used to pass memory blobs. It contains a pointer to the memory block and the size of the block.

```c
typedef struct sogl_memory {
    void* ptr;      // Pointer to the allocated memory block
    size_t size;    // Size of the allocated memory block
} sogl_memory;
```

- **ptr**: A pointer to the allocated memory.
- **size**: The size of the allocated memory block in bytes.

### `sogl_memory_itf`
Metadata stored at the start of each memory block in the allocator's free list. It helps track the size, allocation status, and the next block in the free list.

```c
typedef struct sogl_memory_itf {
    size_t size;                    // Size of the memory block (excluding metadata)
    ubyte is_free;                  // Whether the block is free (1 = free, 0 = allocated)
    struct sogl_memory_itf* next;   // Pointer to the next block in the free list
} sogl_memory_itf;
```

- **size**: The size of the memory block (excluding the `sogl_memory_itf` metadata).
- **is_free**: Indicates whether the block is free (1) or allocated (0).
- **next**: Pointer to the next block in the free list. NULL if there is no next block.

### `sogl_allocator`
The allocator state, managing memory allocation and free lists.

```c
typedef struct sogl_allocator {
    void* heap_start;               // Pointer to the start of the allocated heap
    size_t heap_size;               // Total size of the heap
    sogl_memory_itf* free_list;     // Linked list of free memory blocks
} sogl_allocator;
```

- **heap_start**: The beginning of the heap memory.
- **heap_size**: The total size of the allocated heap memory.
- **free_list**: A linked list of free memory blocks, managed by the allocator.

---

## **Macros**

### `SOGL_MEMORY(x)`
Creates a `sogl_memory` struct from a value `x` (e.g., an array or struct). This macro is useful for initializing memory blobs from variables.

```c
#define SOGL_MEMORY(x) (sogl_memory){ &x, sizeof(x) }
```

- **x**: A variable (array, struct, or primitive type).
- **Returns**: A `sogl_memory` struct containing a pointer to `x` and its size.

### `SOGL_MEMORY_REF(x)`
Creates a reference to a `sogl_memory` struct for a variable `x`. Useful when passing `sogl_memory` structs by reference to functions.

```c
#define SOGL_MEMORY_REF(x) &(sogl_memory){ &x, sizeof(x) }
```

- **x**: A variable (array, struct, or primitive type).
- **Returns**: A pointer to a temporary `sogl_memory` struct containing a pointer to `x` and its size.

### `SOGL_SIZEOF(type)`
Returns the size of a type, using `sizeof`.

```c
#define SOGL_SIZEOF(type) sizeof(type)
```

- **type**: The type whose size is required.
- **Returns**: The size of the type in bytes.

### `SOGL_ALIGNOF(type)`
Returns the alignment of a type using the `offsetof` technique.

```c
#define SOGL_ALIGNOF(type) offsetof(struct { char c; type t; }, t)
```

- **type**: The type whose alignment is required.
- **Returns**: The alignment of the type in bytes.

---

## **Functions**

### `sogl_make_allocator`
Creates and initializes a memory allocator with a specified heap size.

```c
SOGL_API_ENTRY sogl_allocator sogl_make_allocator(size_t heap_size);
```

- **heap_size**: The size of the heap to allocate.
- **Returns**: A `sogl_allocator` struct containing the allocator state.

### `sogl_destroy_allocator`
Destroys the allocator, freeing the allocated heap memory.

```c
SOGL_API_ENTRY void sogl_destroy_allocator(sogl_allocator* alloc);
```

- **alloc**: A pointer to the `sogl_allocator` to be destroyed.

### `sogl_alloc`
Allocates a memory block from the allocator with the specified size and alignment.

```c
SOGL_API_ENTRY sogl_memory sogl_alloc(sogl_allocator* alloc, size_t size, size_t alignment);
```

- **alloc**: A pointer to the `sogl_allocator` from which to allocate memory.
- **size**: The size of the memory block to allocate in bytes.
- **alignment**: The alignment of the memory block in bytes.
- **Returns**: A `sogl_memory` struct containing the pointer to the allocated memory and its size.

### `sogl_free`
Frees a previously allocated memory block, returning it to the allocator.

```c
SOGL_API_ENTRY void sogl_free(sogl_allocator* alloc, sogl_memory mem);
```

- **alloc**: A pointer to the `sogl_allocator` managing the memory block.
- **mem**: The `sogl_memory` struct representing the memory block to be freed.

---

## **Memory Alignment**

The `sogl_alloc` function supports alignment of memory blocks. You can calculate alignment using the `SOGL_ALIGNOF` macro, which calculates the alignment of a type. It ensures that the allocated memory blocks adhere to platform-specific alignment requirements.

For example, to allocate memory for a struct with proper alignment:

```c
struct MyStruct {
    int a;
    float b;
};

sogl_allocator alloc = sogl_make_allocator(1024);
sogl_memory mem = sogl_alloc(&alloc, sizeof(struct MyStruct), SOGL_ALIGNOF(struct MyStruct));
```

In this case, the `SOGL_ALIGNOF` macro is used to get the proper alignment for `MyStruct`, ensuring that `sogl_alloc` aligns the memory accordingly.

---

## **Example Usage**

```c
#include "soalloc.h"

int main() {
    sogl_allocator alloc = sogl_make_allocator(1024);

    // Create a memory block for an integer
    int x = 42;
    sogl_memory mem = SOGL_MEMORY(x);
    printf("Pointer: %p, Size: %zu\n", mem.ptr, mem.size);

    // Allocate memory for a struct
    struct MyStruct {
        int a;
        float b;
    };
    sogl_memory struct_mem = sogl_alloc(&alloc, sizeof(struct MyStruct), SOGL_ALIGNOF(struct MyStruct));
    struct MyStruct* my_struct = (struct MyStruct*)struct_mem.ptr;
    my_struct->a = 10;
    my_struct->b = 3.14f;
    printf("Struct - a: %d, b: %.2f\n", my_struct->a, my_struct->b);

    // Free memory
    sogl_free(&alloc, struct_mem);

    // Destroy the allocator
    sogl_destroy_allocator(&alloc);

    return 0;
}
```

---

## **Notes**
- The allocator is simple and is intended for managing memory within a specific heap. It does not use a system-wide allocator like `malloc` or `free`.
- Memory blocks are split when needed during allocation, and adjacent free blocks are coalesced during freeing to reduce fragmentation.
- Alignment is supported and calculated using the `SOGL_ALIGNOF` macro, which ensures that allocated blocks adhere to platform-specific alignment requirements.
