#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_CAPACITY 128

typedef struct {
    int value;
    char *name;
} Variable;

typedef struct {
    Variable *entries;
    int capacity;
} Map;

int hash(char *key) {
    int hash = 0;
    int c = 0;
    for(int i=0;i<strlen(key)-1;i++){
        hash = (hash << 2) + hash + key[i];
    }
    return hash;
}

Map *construct() {
    Map *map = malloc(sizeof(Map));
    map->capacity = MAP_CAPACITY;
    map->entries = calloc(MAP_CAPACITY, sizeof(Variable));
    return map;
}
void put(Map *map, char *key, int value) {
    int index = hash(key) % map->capacity;
    while (map->entries[index].name != NULL) {
        if (strcmp(map->entries[index].name, key) == 0) {
            map->entries[index].value = value;
            return;
        }
        index = (index + 1) % map->capacity;
    }
    map->entries[index].value = value;
    map->entries[index].name = key;
}
int get(Map *map, char *key) {
    int index = hash(key) % map->capacity;
    while (map->entries[index].name != NULL) {
        if (strcmp(map->entries[index].name, key) == 0) {
            return map->entries[index].value;
        }
        index = (index + 1) % map->capacity;
    }
    return 0;
}

int main() {
    Map *map = construct();

    put(map, "one", 1);
    put(map, "two", 2);
    put(map, "three", 3);

    printf("Value of 'one': %d\n", get(map, "one"));
    printf("Value of 'two': %d\n", get(map, "two"));
    printf("Value of 'three': %d\n", get(map, "three"));
    printf("Value of 'four': %d\n", get(map, "four"));
    return 0;
}