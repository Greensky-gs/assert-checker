#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNXPECTED_INPUT 180
#define UNXPECTED_END_INPUT 181

#define VARIABLE_COUNT 26

enum Operator {
    NOT = 3,
    AND = 2,
    OR = 1,
    IMPLIES = 0
};

struct Node {
    struct Node* left;
    struct Node* right;
    char value;
};

// Tools
void error_message(char * message) {
    printf("\x1b[31mERREUR : %s\x1b[0m\n", message);
}

int precedance(char c) {
    switch (c) {
        case '!':
            return NOT;
        case '&':
            return AND;
        case '|':
            return OR;
        case '>':
            return IMPLIES;
        default:
            return -1;
    }
}
int isOperator(char c) {
    return c == '!' || c == '&' || c == '|' || c == '-';
}
int isParenthesis(char c) {
    return c == '(' || c == ')';
}
int isVariable(char c) {
    //TODO fix this to allow uppercase letters
    return (c >= 'a' && c <= 'z');
}

struct Node * parse_implication(char * expression, int * pos);


// Main
// Variables
int valid_expresssion = 1;
// Main tools

struct Node* create_node() {
    struct Node* node = malloc(sizeof(struct Node));

    node->left = NULL;
    node->right = NULL;
    node->value = '\0';

    return node;
}
void free_tree(struct Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

struct Node * parse_primary(char * expression, int * pos) {
    char c = expression[*pos];
    if (!c) {
        valid_expresssion = 0;
        raise(UNXPECTED_END_INPUT);
    }

    if (isVariable(c)) {
        (*pos)++;
        struct Node* node = create_node();
        node->value = c;
        node->left = NULL;
        node->right = NULL;
        return node;
    } else if (c == '(') {
        (*pos)++;
        struct Node * result = parse_implication(expression, pos);
        if (expression[*pos] != ')') {
            valid_expresssion = 0;
            raise(UNXPECTED_INPUT);
        }
        (*pos)++;
        return result;
    } else {
        raise(UNXPECTED_INPUT);
        valid_expresssion = 0;
        return &(struct Node){.left = NULL, .right = NULL, .value = '\0'};
    }
}
struct Node* parse_not(char * expression, int * pos) {
    if (expression[*pos] == '!') {
        (*pos)++;
        struct Node * operand = parse_not(expression, pos);
        struct Node * node = create_node();
        node->left = NULL;
        node->right = operand;
        node->value = '!';
        return node;
    }
    return parse_primary(expression, pos);
}
struct Node * parse_and(char * expression, int * pos) {
    struct Node * left = parse_not(expression, pos);
    while (expression[*pos] == '&') {
        char op = expression[*pos];
        (*pos)++;
        struct Node * right = parse_not(expression, pos);
        struct Node * node = create_node();
        node->left = left;
        node->right = right;
        node->value = op;
        return node;
    }
    return left;
}
struct Node * parse_or(char * expression, int * pos) {
    struct Node * left = parse_and(expression, pos);
    while (expression[*pos] == '|') {
        char op = expression[*pos];
        (*pos)++;
        struct Node * right = parse_and(expression, pos);
        struct Node * node = create_node();
        node->left = left;
        node->right = right;
        node->value = op;

        return node;
    }

    return left;
}

struct Node * parse_implication(char * expression, int * pos) {
    struct Node * left = parse_or(expression, pos);
    while (expression[*pos] == '-') {
        char op = expression[*pos];
        (*pos)++;
        struct Node * right = parse_or(expression, pos);
        struct Node * node = create_node();
        node->left = left;
        node->right = right;
        node->value = op;
        return node;
    }

    return left;
}
struct Node * parse(char * expression, int size, int * pos) {
    struct Node * result = parse_implication(expression, pos);
    if (*pos < size) {
        raise(UNXPECTED_INPUT);
        valid_expresssion = 0;
    }
    return result;
}

void decimalToBinary(int num, char binary[]) {
    if (num == 0) {
        for (int i = 0; i < VARIABLE_COUNT; i++) {
            binary[i] = 'f';
        }
        return;
    }

    int i = 0;

    while (num > 0) {
        binary[i] = (num & 1) == 1 ? 'v' : 'f';
        num = num >> 1;
        i++;
    }
}


// Main functions
int build(char * expression, struct Node * node, int size) {
    int pos = 0;
    struct Node * res = parse(expression, size, &pos);

    *node = *res;
    return 0;
}

int findVariables(const char * expression, int size, char * variables, int * count) {
    int i = 0;
    *count = 0;
    while (i < size && *count < VARIABLE_COUNT) {
        char c = expression[i];
        if (isVariable(c)) {
            if (variables[c - 'a'] == '-') {
                variables[c - 'a'] = c;
                (*count)++;
            }
        }
        i++;
    }

    return 0;
}

void removeSpaces(const char * expression, const int * size, char * newExpression, int * newSize) {
    int i = 0;
    int j = 0;
    while (i < *size) {
        if (expression[i] != ' ') {
            newExpression[j] = expression[i];
            j++;
        }
        i++;
    }

    *newSize = j;
}
int Cvalue(const char var, const char * variables, const char * values) {
    int valuesBefore = 0;
    for (int i = 0; i < var - 'a'; i++) {
        if (variables[i] != '-') {
            valuesBefore++;
        }
    }

    return values[valuesBefore] == 'v';
}
char * colorateChar(char c) {
    return c == 'v' ? "\033[0;32m1\033[0m" : "\033[0;31m0\033[0m";
}
char * colorateInt(int c) {
    return c ? "\033[0;32m1\033[0m" : "\033[0;31m0\033[0m";
}

int evaluateInduction(struct Node * node, char * variables, char * model) {
    if (isVariable(node->value)) {
        return Cvalue(node->value, variables, model);
    } else {
        if (node->value == '!') {
            return !evaluateInduction(node->right, variables, model);
        }
        int left = evaluateInduction(node->left, variables, model);
        int right = evaluateInduction(node->right, variables, model);

        switch (node->value) {
            case '&':
                return left && right;
            case '|':
                return left || right;
            case '-':
                return !left || right;
            default:
                return -1;
        }
    }
}

int evaluate(struct Node * node, char * variables, int varCount, char * model) {
    for (int i = 0; i < varCount; i++) {
        printf(" %s |", colorateChar(model[i]));
        // printf(" %c |", model[i]);
    }

    int result = evaluateInduction(node, variables, model);
    printf(" %s\n", colorateInt(result));

    return result;
}

void truth_table(struct Node * node, char expression[], int size, char * variables, int varCount) {
    for (int i = 0; i < VARIABLE_COUNT; i++) {
        if (variables[i] != '-') {
            printf(" \x1b[94m%c\x1b[0m |", variables[i]);
        }
    }
    printf("  %s\n", expression);
    for (int i = 0; i < varCount * 4 + size + 2; i++) {
        if (i > 0 && i <= varCount * 4 && i % 4 == 3) {
            printf("|");
        } else {
            printf("-");
        }
    }
    printf("\n");

    int total = pow(2, varCount);
    int valid_models = 0;

    for (int i = 0; i < total; i++) {
        char binary[VARIABLE_COUNT + 1];
        decimalToBinary(i, binary);

        int value = evaluate(node, variables, varCount, binary);
        if (value) {
            valid_models++;
        }
    }

    int satisfiable = valid_models > 0;
    int valid = valid_models == total;

    printf("Votre assertion est \x1b[%dm%s\x1b[0m et \x1b[%dm%s\x1b[0m.\n", satisfiable ? 32 : 31, satisfiable ? "satisfiable" : "insatifiable", valid ? 32 : 31, valid ? "valide" : "invalide");
}

int check_parenthesis(const char * expression, const int size) {
    int count = 0;
    int i = 0;

    while (i < size && count >= 0) {
        if (expression[i] == '(') {
            count++;
        }
        if (expression[i] == ')') {
            count--;
        }
        i++;
    }

    return count == 0;
}


int main() {
    printf("Entrez une assertion logique : \x1b[94m");
    fflush(stdout);

    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\x1b[0m");
        error_message("Erreur de lecture de l'entree.");
        return 1;
    }
    printf("\x1b[0m");
    buffer[strcspn(buffer, "\n")] = 0;
    char *input = buffer;

    int size = (int)strlen(input);

    int valid_parenthesis = check_parenthesis(input, size);
    if (!valid_parenthesis) {
        error_message("Parenthesage incorrect");
        return 1;
    }

    char expression[size];
    int expressionSize = 0;

    removeSpaces(input, &size, expression, &expressionSize);

    int varCount = 0;
    char variables[VARIABLE_COUNT + 1];
    for (int i = 0; i < VARIABLE_COUNT; i++) {
        variables[i] = '-';
    }

    findVariables(expression, expressionSize, variables, &varCount);

    if (varCount == 0) {
        error_message("Aucune variable trouvee");
        return 1;
    }

    struct Node *tree = &(struct Node){ .left = NULL, .right = NULL, .value = '\0' };
    build(expression, tree, expressionSize);

    if (!valid_expresssion) {
        error_message("Expression non valide");
        return 1;
    }

    truth_table(tree, expression, expressionSize, variables, varCount);
    free_tree(tree);

    return 0;
}