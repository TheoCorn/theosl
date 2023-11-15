#include <string.h>
#include "ss.h"
#include <stddef.h>

size_t strptrusize_djb2(char** strptr) {
	char const* __restrict string = *strptr;
	size_t hash = 5381;
	for (size_t i = 0; string[i] != '\0'; ++i) {
		hash = ((hash << 5) + hash) + string[i];
	}

	return hash;
}
