#ifndef _GUTILS
#define _GUTILS

#include "dirent.h"
#include "stddef.h"
#include "stdio.h"

// -------------------------------------------------------------------------------------
// ArrayHandle
// -------------------------------------------------------------------------------------
#define GenArrayDefinition(type, maxElements, arrayStructName) typedef struct { \
	type elements[maxElements]; \
	unsigned int count; \
} arrayStructName;

#define ArrayNew(arrayStructName) (arrayStructName) {.elements = {0}, .count = 0 }
#define ArrayPush(array, element) array.elements[array.count++] = element


// -------------------------------------------------------------------------------------
// Hashmap
// -------------------------------------------------------------------------------------
unsigned int hashString(char* name);

#define HASH_MAP_SIZE 512
#define GenHashmapDefinition(type, hashMapStructName) typedef struct { 	\
	type elements[HASH_MAP_SIZE];										\
} hashMapStructName;

#define HashMapPut(hashmap, key, value) hashmap.elements[hashString(key)] = value
#define HashMapGet(hashmap, key) 		hashmap.elements[hashString(key)]


// -------------------------------------------------------------------------------------
// File utils
// -------------------------------------------------------------------------------------
void doForEachFileInFolder(char* directoryPath, void (*func)(int, struct dirent*)); 

#endif
