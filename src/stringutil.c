#include <stdlib.h>
#include <string.h>

#include "stringutil.h"

bool string_cptrlist_search(void *a, void *b) {
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	if (str_a == str_b) {
		return true;
	}
	if (str_a == NULL || str_b == NULL) {
		return false;
	}

	return strcmp(str_a, str_b);
}

char *strdup(const char *in) {
	if (in == NULL) {
		return NULL;
	}

	char *out = malloc(strlen(in) + 1);
	if (out == NULL) {
		return NULL;
	}

	memcpy(out, in, strlen(in) + 1);
	return out;
}
