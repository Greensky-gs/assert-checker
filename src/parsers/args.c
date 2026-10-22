#include "stringlib.h"
#include "parsers.h"
#include "args.h"
#include <stdio.h>

extern int streq(char *, char *);
extern int parse_int(char *, int *);

int in_args(int argc, char * argv[], char * target) {
	int i = 1;
	while (i < argc) {
		if (streq(argv[i], target)) return 1;
		i++;
	}
	
	return 0;
}
char * arg_value(int argc, char * argv[], char * target) {
	int i = 1;
	while (i < argc - 1) {
		if (streq(argv[i], target)) return argv[i + 1];
		i++;
	}
	return NULL;
}

int get_int(int argc, char * argv[], char * target, int * res) {
	int i = 1;
	while (i < argc - 1) {
		if (streq(argv[i], target)) {
			int control;
			*res = parse_int(argv[i + 1], &control);
			return control == 0;
		}
		i++;
	}
	return 2;
}

int find_all(int argc, char * argv[], struct arg_input args[], int size) {
	if (size == 0) return 0;

	int found = 0;
	int i = 1;
	while (i < argc) {
		if (argv[i][0] != '-') {
			i++;
			continue;
		}
		int j = 0;
		while (j < size) {
			if (args[j].found || !streq(argv[i], args[j].name)) {
				j++;
				continue;
			}
			
			switch (args[j].type) {
				case Presence:
					found += args[j].found = 1;
					break;
				case Int:
					if (i == argc - 2) break;
					args[j].found = parse_int(argv[i + 1], &(args[j].int_res));
					found += args[j].found;
					break;
				case String:
					if (i == argc - 1) break;
					found++;
					args[j].found = 1;
					args[j].str_result = argv[i + 1];
					break;
			}
			j++;
		}
		i++;
	}

	return found;
}
