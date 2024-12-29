#pragma once

#include "../types.h"
#include "../defines.h"

typedef struct lotus_key_value {
    const char* k;
    void* v;
} lotus_key_value;

typedef struct lotus_hashmap {
    int max;
    int count;
    lotus_key_value** map;
} lotus_hashmap;

/**
 * @brief Creates a new hashmap with a specified maximum number of entries.
 * 
 * @param max The maximum number of entries the hashmap can hold.
 * @return A pointer to the newly created hashmap, or NULL on failure.
 */
LOTUS_API_ENTRY lotus_hashmap* lotus_make_hashmap(int max);

/**
 * @brief Destroys a hashmap and frees all associated memory.
 * 
 * @param m A pointer to the hashmap to destroy.
 */
LOTUS_API_ENTRY void lotus_destroy_hashmap(lotus_hashmap* m);

/**
 * @brief Retrieves a value from the hashmap using the specified key.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to look up.
 * @return A pointer to the value associated with the key, or NULL if the key is not found.
 */
LOTUS_API_ENTRY void* lotus_get_hashmap(lotus_hashmap* m, const char* key);

/**
 * @brief Removes a key-value pair from the hashmap.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to remove.
 * @return `LOTUS_SUCCESS` on success, or an appropriate error code if the operation fails.
 */
LOTUS_API_ENTRY lotus_error_type lotus_rem_hashmap(lotus_hashmap* m, const char* key);

/**
 * @brief Sets a key-value pair in the hashmap. If the key already exists, its value is updated.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to set.
 * @param value A pointer to the value to associate with the key.
 * @return `LOTUS_SUCCESS` on success, or an appropriate error code if the operation fails.
 */
LOTUS_API_ENTRY lotus_error_type lotus_set_hashmap(lotus_hashmap* m, const char* key, void* value);
