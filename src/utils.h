#ifndef _GUTILS
#define _GUTILS

#include "dirent.h"
#include "stddef.h"
#include "stdio.h"
#include "raylib.h"

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


// -------------------------------------------------------------------------------------
// Math
// -------------------------------------------------------------------------------------
float min(float a, float b);
float max(float a, float b);



// -------------------------------------------------------------------------------------
// Rendering
// -------------------------------------------------------------------------------------

// Initializes a new window. Wraps a few raylib functions to handle screen scaling.
void InitTextureWindow(int newWindowWidth, int newWindowHeight, int newWorldWidth, int newWorldHeight, char* windowTitle);
void BeginTextureRendering();
void EndTextureRendering();

// changes the current window resolution
// if fullscreen is true changes to monitor resolution instead and switches fullscreen on
void SwitchResolution(int width, int height, bool fullscreen);

#endif
