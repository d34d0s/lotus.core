#include "lotus_hashmap.h"

#include "../utility/lotus_array.h"
#include "../platform/lotus_logger.h"

_LOTUS_PRIVATE int lotus_string_hash(const char* buffer) {
    int res = 0;
    size_t size = strlen(buffer);
    for(int i = 0; i < size; i++) {
        res+=(int)buffer[i]*31;
    }; return res;
}

_LOTUS_PRIVATE bool lotus_probe_hashmap_f(lotus_hashmap* m, int* kHash, const char* key) {
    bool match = 0;
    lotus_key_value kvp = m->map[*kHash];
    
    for (int i = *kHash+1; i < m->max; i++) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp.k) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp.k && !strcmp(key, kvp.k)) {
                *kHash = i;
                match = 1;
                break;
            } else continue;
        }
    } return match;
}

_LOTUS_PRIVATE bool lotus_probe_hashmap_r(lotus_hashmap* m, int* kHash, const char* key) {
    bool match = 0;
    lotus_key_value kvp = m->map[*kHash];
    
    for (int i = *kHash-1; i > 0; i--) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp.k) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp.k && !strcmp(key, kvp.k)) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        }
    } return match;
}

lotus_hashmap* lotus_make_hashmap(int max) {
    lotus_hashmap* m = (lotus_hashmap*)malloc(sizeof(lotus_hashmap));
    if (!m) {
        lotus_set_log_level(LOTUS_LOG_ERROR);
        lotus_log_error("failed to allocate hashmap");
        return NULL;
    }

    m->max = max;
    m->count = 0;

    m->map = (lotus_key_value*)lotus_make_array(sizeof(lotus_key_value*), max);
    if (!m->map) {
        free(m);
        lotus_set_log_level(LOTUS_LOG_ERROR);
        lotus_log_error("failed to allocate hashmap array");
        return NULL;
    }

    return m;
}

void lotus_destroy_hashmap(lotus_hashmap* m) {
    lotus_destroy_array(m->map);
    free(m);
}

void* lotus_get_hashmap(lotus_hashmap* m, const char* key) {
    if (!key) { return NULL; }

    int kHash = lotus_string_hash(key) % m->max;
    lotus_key_value kvp = m->map[kHash];

    if (kvp.k && strcmp(key, kvp.k)) {
        bool match = 0;

        // forward probing
        match = lotus_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = lotus_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            lotus_set_log_level(LOTUS_LOG_ERROR);
            lotus_log_error("probing error | key [%s] is not set", key);
            return NULL;
        }
        
        kvp = m->map[kHash];
    }; return kvp.v;
}

lotus_error_type lotus_set_hashmap(lotus_hashmap* m, const char* key, void* value) {
    if (!key || !value || m->count+1 > m->max) { return LOTUS_ERR_TYPE; }

    int kHash = lotus_string_hash(key) % m->max;
    lotus_key_value kvp = m->map[kHash];

    // resolve collisions with open addressing + linear probing
    if (kvp.k) {
        if (!strcmp(key, kvp.k)) {
            m->map[kHash].v = value;
            return LOTUS_ERR_NONE;        
        }

        bool set = 0;
        
        // forward probing
        set = lotus_probe_hashmap_f(m, &kHash, NULL);

        // reverse probing
        if (!set) set = lotus_probe_hashmap_r(m, &kHash, NULL);

        if (!set) {
            lotus_set_log_level(LOTUS_LOG_ERROR);
            lotus_log_error("probing error | key[%s]", key);
            return LOTUS_ERR_FUNC;
        }
    }

    // m->map[kHash] = (lotus_key_value*)malloc(sizeof(lotus_key_value));
    m->map[kHash].k = strdup(key);
    m->map[kHash].v = value;
    LOTUS_ARRAY_SET_HEADER_FIELD(m->map, LOTUS_ARRAY_LENGTH_FIELD, m->count++);
    return LOTUS_ERR_NONE;
}

lotus_error_type lotus_rem_hashmap(lotus_hashmap* m, const char* key) {
    if (!key) { return LOTUS_ERR_TYPE; }

    int kHash = lotus_string_hash(key) % m->max;
    lotus_key_value kvp = m->map[kHash];

    if (kvp.k && strcmp(key, kvp.k)) {
        bool match = 0;

        // forward probing
        match = lotus_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = lotus_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            lotus_set_log_level(LOTUS_LOG_ERROR);
            lotus_log_error("probing error | key [%s] is not set", key);
            return LOTUS_ERR_FUNC;
        }
        
        kvp = m->map[kHash];
    };
    
    m->map[kHash].k = NULL;
    m->map[kHash].v = NULL;
    LOTUS_ARRAY_SET_HEADER_FIELD(m->map, LOTUS_ARRAY_LENGTH_FIELD, m->count--);
    return LOTUS_ERR_NONE;
}
