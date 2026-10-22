#include "parsers/args.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VARIABLES_COUNT 26

struct Node {
	char value;
	struct Node * left;
	struct Node * right;
};
typedef struct Node * tNode;

tNode create_node(char value) {
	tNode node;
	if ((node = malloc(sizeof(struct Node))) == NULL) return NULL;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}
void destroy_node(tNode * p) {
	if ((*p)->left != NULL) destroy_node(&(*p)->left);
	if ((*p)->right != NULL) destroy_node(&(*p)->right);

	free(*p);
	*p=NULL;
}

void display_infix(tNode node, int * count) {
	if (node->left != NULL) display_infix(node->left, count);
	printf("%c", node->value);
	if (count != NULL) (*count)++;
	if (node->right != NULL) display_infix(node->right, count);
}
void display_full(tNode node) {
	display_infix(node, NULL);
	printf("\n");
}

int is_variable(char v) {
	return 97 <= v && v <= 122;
}
int is_operator(char v) {
	return v == '&' || v == '|' || v == '!' || v == '-';
}
int is_parenthesis(char v) {
	return v == '(' || v == ')';
}
int is_valid(char v) {
	return is_variable(v) || is_operator(v) || is_parenthesis(v);
}

int is_tree_valid(tNode tree) {
	if (tree->right == NULL && tree->left == NULL && !is_variable(tree->value)) {
		return 0;
	}
	if (tree->left != NULL && tree->right == NULL && !is_operator(tree->value)) {
		return 0;
	}
	if (tree->left != NULL && tree->right == NULL && tree->value != '!') return 0;
	int left = tree->left == NULL ? 1 : is_tree_valid(tree->left);
	int right = tree->right == NULL ? 1 : is_tree_valid(tree->right);

	return left && right;	
}

int strsize(char * input) {
	int i = 0;
	while (input[i++] != 0) {};
	return i - 1;
}

int valid_expression = 1;
tNode parse_implication(char *, int *);

tNode parse_primary(char * expression, int * pos) {
	char c = expression[*pos];
	if (!c) {
		valid_expression = 0;
		return NULL;
	}

	if (is_variable(c)) {
		(*pos)++;
		return create_node(c);
	} else if (c == '(') {
		(*pos)++;
		tNode res = parse_implication(expression, pos);
		if (expression[*pos] != ')') {
			valid_expression = 0;
			return NULL;
		}
		(*pos)++;
		return res;
	} else {
		valid_expression = 0;
		return create_node(0);
	}
}

tNode parse_not(char * expression, int * pos) {
	while (expression[*pos] == '!') {
		(*pos)++;
		tNode operand = parse_primary(expression, pos);
		tNode node = create_node('!');
		node->right = operand;
		return node;
	}

	return parse_primary(expression, pos);
}

tNode parse_and(char * expression, int * pos) {
	tNode left = parse_not(expression, pos);
	while (expression[*pos] == '&') {
		char op = expression[*pos];
		(*pos)++;
		tNode right = parse_not(expression, pos);
		tNode node = create_node(op);
		node->left = left;
		node->right = right;
		left = node;
	}

	return left;
}

tNode parse_or(char * expression, int * pos) {
	tNode left = parse_and(expression, pos);
	while (expression[*pos] == '|') {
		char op = expression[*pos];
		(*pos)++;
		tNode right = parse_and(expression, pos);
		tNode node = create_node(op);
		node->left = left;
		node->right = right;
		left = node;
	}

	return left;
}

tNode parse_implication(char * expression, int * pos) {
	tNode left = parse_or(expression, pos);
	while (expression[*pos] == '-') {
		char op = expression[*pos];
		(*pos)++;
		tNode right = parse_or(expression, pos);
		tNode node = create_node(op);
		node->left = left;
		node->right = right;
		left = node;
	}

	return left;
}

tNode parse_expression(char * expression, int size) {
	int start = 0;
	tNode result = parse_implication(expression, &start);

	if (start < size) {
		valid_expression = 0;
	}
	return result;
}
int valid_input(char * expr) {
	int p = 0;
	while (expr[p++] != 0) {
		if (!is_valid(expr[p - 1])) return 0;
	}

	return 1;
}

char * get_used_variables(char * expression, int size, int * found) {
	char * arr;
	if ((arr = calloc(VARIABLES_COUNT, sizeof(char))) == NULL) {
		perror("Cannot allocate variables array");
		return NULL;
	}
	*found = 0;

	int i = 0;
	while (i++ < size) {
		char c = expression[i - 1];
		if (is_variable(c = expression[i - 1]) && arr[c - 97] == 0) {
			arr[c - 97] = 1;
			(*found)++;
		}
	}

	return arr;
}

int evaluate_induction(tNode tree, char * model) {
	if (is_variable(tree->value)) {
		return (model[tree->value - 97] == 1);
	} else {
		if (tree->value == '!') return !evaluate_induction(tree->right, model);
		int left = evaluate_induction(tree->left, model);

		if (tree->value == '|') {
			if (left) return 1;
			return evaluate_induction(tree->right, model);
		}
		if (tree->value == '&') {
			if (!left) return 0;
			return evaluate_induction(tree->right, model);
		}
		if (tree->value == '-') {
			if (!left) return 1;
			return evaluate_induction(tree->right, model);
		}
	}
	return 0; // Should never reach this
}

char * create_model(char * variables, int base) {
	char * modelisation;
	if ((modelisation = calloc(VARIABLES_COUNT, sizeof(char))) == NULL) return NULL;

	int i = 0;
	while (i++ < VARIABLES_COUNT) {
		if (variables[i - 1]) {
			modelisation[i - 1] = base & 1;
			base >>=1;
		}
	}

	return modelisation;
}

#define GET_PRINT_STATEMENT(_1, _2, _3, _4, _5, NAME, ...) NAME
#define PRINT_STATEMENT(...) GET_PRINT_STATEMENT(__VA_ARGS__, PRINT_STATEMENT_ARGS, PRINT_STATEMENT_NOARGS)(__VA_ARGS__)
#define PRINT_STATEMENT_ARGS(fd, pre_col, buf, post_col, args) { \
  printf(pre_col buf post_col, args); \
  if (fd != 0) {\
    char buffer[BUFSIZ] = {0}; \
    snprintf(buffer, BUFSIZ - 1, buf, args); \
    ssize_t s = strlen(buf); \
    write(fd, buffer, s); \
  }\
}
#define PRINT_STATEMENT_NOARGS(fd, pre_col, buf, post_col) {\
  printf(pre_col buf post_col); \
  if (fd != 0) {\
    ssize_t s = strlen(buf); \
    write(fd, buf, s); \
  } \
}

void display_infix_with_fd(tNode node, int * count, int fd) {
	if (node->left != NULL) display_infix_with_fd(node->left, count, fd);
	PRINT_STATEMENT(fd, "", "%c", "", node->value);
	if (count != NULL) (*count)++;
	if (node->right != NULL) display_infix_with_fd(node->right, count, fd);
}
void print_header(tNode tree, int vars, char * varmodel, int fd) {
	int i = 0;
	while (i < VARIABLES_COUNT) {
		if (varmodel[i]) {
      PRINT_STATEMENT(fd, "\x1b[34m", " %c", "\x1b[0m", 97 + i);
      PRINT_STATEMENT(fd, "", " │", "");
    }
		i++;
	}

  PRINT_STATEMENT(fd, "", " ", "\x1b[34m");
	int length = 0;
	display_infix_with_fd(tree, &length, fd);
  PRINT_STATEMENT(fd, "\x1b[0m", "\n", "");

	i = 0;
	while (i++ < vars) PRINT_STATEMENT(fd, "", "───┼", "");
	i = 0;
	while (i++ < length + 2) PRINT_STATEMENT(fd, "", "─", "");

  PRINT_STATEMENT(fd, "", "\n", "");
	fflush(stdout);
}

void start_process(tNode tree, int vars, char * varmodel, int fd) {
	int count = 1 << vars;
	int i = 0;

	print_header(tree, vars, varmodel, fd);

	while (i < count) {
		char * buffer = create_model(varmodel, i);

		int j = 0;
		while (j < VARIABLES_COUNT) {
			if (varmodel[j]) {
				if (buffer[j]) PRINT_STATEMENT(fd, "\x1b[32m", " 1", "\x1b[0m")
				else PRINT_STATEMENT(fd, "\x1b[31m", " 0", "\x1b[0m");

        PRINT_STATEMENT(fd, "", " │", "");
			}
			j++;
		}

		int result = evaluate_induction(tree, buffer);
    if (result) PRINT_STATEMENT(fd, "\x1b[32m", " 1\n", "\x1b[0m")
    else PRINT_STATEMENT(fd, "\x1b[31m", " 0\n", "\x1b[0m");

		i++;

		free(buffer);
	}
}
#undef PRINT_STATEMENT
#undef PRINT_STATEMENT_ARGS
#undef PRINT_STATEMENT_NOARGS
#undef GET_PRINT_STATEMENT

void remove_double_char(char * expression, char target, int size, int * new_size) {
	if (size < 2) {
		*new_size = size;
		return;
	}

	int i = 0;
	while (i < size - 1) {
		if (expression[i] == target && expression[i + 1] == target) {
			int a = 0;
			while (a < 2) {
				int j = i;
				while (j < size - 1) {
					char n = expression[j + 1];
					expression[j] = n;
					j++;
				}
			
				size--;
				a++;
			}
		}
		i++;
	}

	*new_size = size;
}

static void help_page(struct arg_input args[], int size, char * bin_name) {
	printf("Greensky's assert checker CLI.\n  Basic usage : \x1b[90m%s [OPTIONS]\x1b[0m\n  Options are :\n", bin_name);

	int i = 0;
	while (i < size) {
		printf("    \x1b[90m%-19s\x1b[0m : %s. Of type \x1b[91m", args[i].name, args[i].description);
		switch (args[i].type) {
			case String:
				printf("string\x1b[0m");
				if (args[i].str_result == NULL) printf(", no default value is provided\n");
				else printf(", default value set to \x1b[33m%s\x1b[0m", args[i].str_result);
				break;
			case Int:
				printf("integer\x1b[0m, default value is set to \x1b[33m%d\x1b[0m\n", args[i].int_res);
				break;
			case Presence:
				printf("toggle\x1b[0m\n");
				break;
		}
		i++;
	}
}
int main(int argc, char * argv[]) {
  struct arg_input arguments[] = {
    { "-h", "Displays help page", Presence, 0, 0, NULL },
    { "-o", "Sends result output to a file as well", String, 0, 0, NULL }
  };
  int size = sizeof(arguments) / sizeof(struct arg_input);

  find_all(argc, argv, arguments, size);

  if (arguments[0].found) {
    help_page(arguments, size, argv[0]);
    return 0;
  }

	printf("Welcome to \x1b[94mGreensky's assert checker\x1b[0m\n");

  int out_fd = 0;
  if (arguments[1].found) {
    if ((out_fd = open(arguments[1].str_result, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP)) == -1) {
      perror("open");
      fprintf(stderr, "Impossible to open output file. Continuing without.\n");
      out_fd = 0;
    }
  }

	char * buffer = malloc(1024);
	while (1) {
		valid_expression = 1;
		printf("Enter expression: ");
		fflush(stdout);

		if (fgets(buffer, sizeof(char) * 1023, stdin) == NULL) {
			continue;
		}
		char * input = buffer;
		input[strcspn(buffer, "\n")] = 0;

		if (strcmp(input, "exit") == 0) {
			printf("bye\n");
			free(buffer);
			return 0;
		}

		if (!valid_input(input)) {
			printf("\x1b[31mYou entered a wrong caracter. Try again\x1b[0m\n");
			continue;
		}

		int ssize = strsize(input);
		int size;
		int old = ssize;
		remove_double_char(input, '!', ssize, &size);
		while (size != old) {
			int temp;
			remove_double_char(input, '!', size, &temp);
			old = size;
			size = temp;
		}

		tNode tree = parse_expression(input, size);

		if (tree == NULL) continue;

		if (!valid_expression) {
			printf("\x1b[31mYou entered an invalid expression\x1b[0m\n");
			destroy_node(&tree);
			continue;
		}

		int found;
		char * vars = get_used_variables(input, size, &found);

		if (found == 0) {
			printf("\x1b[31mImpossible\x1b[31m\n");
			free(vars);
			destroy_node(&tree);
			continue;
		}

		start_process(tree, found, vars, out_fd);
		
		free(vars);
		destroy_node(&tree);
	}

	free(buffer);
}
