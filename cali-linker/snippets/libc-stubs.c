#include <string.h>
#include <stdlib.h>

// from glibc
/*
char *strdup(const char *s) {
	size_t len = strlen(s) + 1;
	void *new = malloc(len);

	if (new == NULL)
		return NULL;

	return (char *) memcpy(new, s, len);
}
*/
