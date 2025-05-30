#include <stdint.h>
#include <stdio.h>

uint32_t calc_hash(const char *key) {
    uint32_t hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = (hash << 5) + hash + (uint32_t)c;
    }
    return hash;
}
