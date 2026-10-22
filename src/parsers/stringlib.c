#include "stringlib.h"
#include <stdlib.h>

int streq(char * a, char * b) {
	int i = 0;
	while (a[i] != 0 && b[i] != 0) {
		if (a[i] != b[i]) return 0;
		i++;
	}

	return a[i] == b[i];
}

char * join(char * a, char * b) {
	unsigned long size_a = 0;
	while (a[size_a] != 0) size_a++;
	unsigned long size_b = 0;
	while (b[size_b] != 0) size_b++;

	unsigned long size = size_a + size_b;

	char * joined;
	if ((joined = malloc(size + 1)) == NULL) return NULL;

	unsigned long i = 0;
	while (a[i] != 0) {
		joined[i] = a[i];
		i++;
	}
	unsigned long j = 0;
	while (b[j] != 0) {
		joined[i + j] = b[j];
		j++;
	}
	joined[size] = 0;
	return joined;
}
