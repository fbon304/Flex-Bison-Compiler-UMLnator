#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned int hash(const char * key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31 + *key++) % TABLE_SIZE;
    }
    return hash;
}

HashMap* createHashMap() {
    HashMap* hm = calloc(1, sizeof(HashMap));
    return hm;
}

void put(HashMap* map, const char * key, void * value) {
    unsigned int index = hash(key);
    Entry* current = map->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    Entry* newEntry = malloc(sizeof(Entry));
    newEntry->key = strdup(key);
    newEntry->value = value;
    newEntry->next = map->buckets[index];
    map->buckets[index] = newEntry;
}

void * get(HashMap * map, const char* key) {
    unsigned int index = hash(key);
    Entry* current = map->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return &current->value;
        }
        current = current->next;
    }

    return NULL;
}

void removeKey(HashMap * map, const char * key) {
    unsigned int index = hash(key);
    Entry* current = map->buckets[index];
    Entry* prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev)
                prev->next = current->next;
            else
                map->buckets[index] = current->next;

            free(current->key);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void destroyHashMap(HashMap * map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* current = map->buckets[i];
        while (current) {
            Entry* tmp = current;
            current = current->next;
            free(tmp->value);
            free(tmp);
        }
    }
    free(map);
}
