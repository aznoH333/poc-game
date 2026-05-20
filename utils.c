#include "utils.h"

// -------------------------------------------------------------------------------------
// Hashmap
// -------------------------------------------------------------------------------------
unsigned int hashString(char* name) {
	unsigned int output = 1;

	int i = 0;

	while (name[i]) {
		output *= name[i] + name[i-1 + (i == 0)];
		++i;
	}

	return output % MAX_HASHMAP_SIZE;
}


// -------------------------------------------------------------------------------------
// File utils
// -------------------------------------------------------------------------------------
void doForEachFileInFolder(char* directoryPath, void (*func)(int, struct dirent*)) {
	struct dirent* directoryEntry;

	DIR* dir = opendir(directoryPath);

	if (dir == NULL) {
		printf("Could not open requested directory [%s]", directoryPath);
		return;
	}

	

	for (int i = 0; (directoryEntry = readdir(dir)) != NULL; i++) {
		func(i, directoryEntry);
	}

	closedir(dir);
}
