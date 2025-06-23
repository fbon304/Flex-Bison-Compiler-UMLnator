#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_SIZE 101

typedef struct Entry {
    char * key;
    void * value;
    struct Entry * next;
} Entry;

typedef struct {
    Entry * buckets[TABLE_SIZE];
} HashMap;

HashMap * createHashMap();
void put(HashMap * map, char * key, void * value);
void * get(HashMap * map, const char * key);
void removeKey(HashMap * map, const char * key);
void destroyHashMap(HashMap * map);

#endif
