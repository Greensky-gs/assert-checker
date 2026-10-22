#ifndef __ARGS_H__
#define __ARGS_H__ 1

typedef enum {
	Presence = 0,
	String = 1,
	Int = 2
} ArgType;

struct arg_input {
	char * name; // Starting with - or --
	char * description;
	ArgType type;
	int found;

	int int_res;
	char * str_result;
};

extern int in_args(int, char *[], char *);
extern char * arg_value(int, char *[], char *);
extern int get_int(int, char *[], char *, int *);

extern int find_all(int argc, char *[], struct arg_input [], int size);

#endif
