#ifndef __PARSERS_H__
#define __PARSERS_H__ 1

typedef enum {
	OverFlow = 1,
	InvalidInput = 2
} ParsersControls;

/**
 * @brief Converts string to int
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern int parse_int(char *, int *);
/**
 * @brief Converts string to unsigned int
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern unsigned int parse_uint(char *, int *);

/**
 * @brief Converts string to long int
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern long int parse_lint(char *, int *);
/**
 * @brief Converts string to unsigned long int
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern unsigned long int parse_ulint(char *, int *);

/**
 * @brief Converts string to short int
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern short int parse_sint(char *, int *);
/**
 * @brief Converts string to unsigned short int
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern unsigned short int parse_usint(char *, int *);

/**
 * @brief Converts string to float
 *
 * @param char * - the input
 * @param int * - indicates an error. Set to 0 if everything went fine, set to the values of ParsersControls
 */
extern float parse_float(char *, int *);


#endif
