#include "parsers.h"
#include <stdlib.h>

#define ISC_NMC(x) (48 <= (x) && (x) <= 57)
#define SET_CTR(x, y) if ((x) != NULL) (*x) = y;
#define BAS_OPE(base, value, coef) if (coef == -1) base = (base) * 10 - (value);\
											else base = (base) * 10 + (value);
#define BAS_COM(base, verif, coef) ((coef == -1) ? (base <= verif) : (base >= verif))

int parse_int(char * val, int * control) {
	int base = 0;
	int verif = 0;

	int i = 0;
	int coef = 1;
	if (val[0] == '-') {
		i++;
		coef = -1;
    } else if (val[0] == '+') i++;
	while (val[i] != 0) {
		if (!ISC_NMC(val[i])) {
			SET_CTR(control, InvalidInput)
			return 0;
		}

		BAS_OPE(base, val[i] - 48, coef)

		if (!BAS_COM(base, verif, coef)) {
			SET_CTR(control, OverFlow)
			return 0;
		}
		i++;
		verif = base;
	}

	SET_CTR(control, 0)
	return base;
}

unsigned int parse_uint(char * val, int * control) {
	// The number can only be positive, so we don't need to check the sign and act accordingly. Thus the sign will be ignored.
	unsigned int base = 0;
	unsigned int verif = 0;

	int i = 0;
	if (val[0] == '-' || val[0] == '+') i++;

	while (val[i] != 0) {
		if (!ISC_NMC(val[i])) {
			SET_CTR(control, InvalidInput)
			return 0;
		}

		base = base * 10 + val[i] - 48;
		if (base < verif) {
			SET_CTR(control, OverFlow)
			return 0;
		}
		verif = base;
		i++;
	}

	SET_CTR(control, 0)
	return base;
}

long int parse_lint(char * val, int * control) {
	long int base = 0;
	long int verif = 0;

	int i = 0;
	int coef = 1;
	if (val[0] == '-') {
		i++;
		coef = -1;
	} else if (val[0] == '+') i++;
	while (val[i] != 0) {
		if (!ISC_NMC(val[i])) {
			SET_CTR(control, InvalidInput);
			return 0;
		}

		BAS_OPE(base, val[i] - 48, coef);

		if (!BAS_COM(base, verif, coef)) {
			SET_CTR(control, OverFlow)
				return 0;
		}
		i++;
		verif = base;
	}

    SET_CTR(control, 0)
	return base;
}

unsigned long int parse_ulint(char * val, int * control) {
	// The number can only be positive, so we don't need to check the sign and
	// act accordingly. Thus the sign will be ignored.
	unsigned long int base = 0;
	unsigned long int verif = 0;

	int i = 0;
	if (val[0] == '-' || val[0] == '+') i++;

	while (val[i] != 0) {
		if (!ISC_NMC(val[i])) {
			SET_CTR(control, InvalidInput)
				return 0;
		}

		base = base * 10 + val[i] - 48;
		if (base < verif) {
			SET_CTR(control, OverFlow)
				return 0;
		}
		verif = base;
		i++;
	}

	SET_CTR(control, 0)
	return base;
}

short int parse_sint(char * val, int * control) {
	short int base = 0;
	short int verif = 0;

	int i = 0;
	int coef = 1;
	if (val[0] == '-') {
		i++;
		coef = -1;
    } else if (val[0] == '+') i++;
	while (val[i] != 0) {
		if (!ISC_NMC(val[i])) {
			SET_CTR(control, InvalidInput)
			return 0;
		}

		BAS_OPE(base, val[i] - 48, coef)

		if (!BAS_COM(base, verif, coef)) {
			SET_CTR(control, OverFlow)
			return 0;
		}
		i++;
		verif = base;
	}

	SET_CTR(control, 0)
	return base;
}

unsigned short int parse_usint(char * val, int * control) {
	// The number can only be positive, so we don't need to check the sign and act accordingly. Thus the sign will be ignored.
	unsigned short int base = 0;
	unsigned short int verif = 0;

	int i = 0;
	if (val[0] == '-' || val[0] == '+') i++;

	while (val[i] != 0) {
		if (!ISC_NMC(val[i])) {
			SET_CTR(control, InvalidInput)
			return 0;
		}

		base = base * 10 + val[i] - 48;
		if (base < verif) {
			SET_CTR(control, OverFlow)
			return 0;
		}
		verif = base;
		i++;
	}

	SET_CTR(control, 0)
	return base;
}

float parse_float(char * val, int * control) {
	float base = 0;
	float verif = 0;

	int i = 0;
	int coef = 1;
	int floating = 0;
	long div_power = 10;

	if (val[0] == '-') {
		coef = -1;
		i++;
	} else if (val[0] == '+') i++;
	while (val[i] != 0) {
		if (!ISC_NMC(val[i]) && val[i] != '.') {
			SET_CTR(control, InvalidInput)
			return 0;
		}
		if (val[i] == '.') {
			if (floating) {
				SET_CTR(control, InvalidInput)
				return 0;
			}
			floating = 1;
			i++;
			continue;
		}

		if (floating) {
			if (coef == 1) base += (float)(val[i] - 48) / div_power;
			else base -= (float)(val[i] - 48) / div_power;
			div_power *= 10;
		} else BAS_OPE(base, val[i] - 48, coef);
		
		if (!BAS_COM(base, verif, coef)) {
			SET_CTR(control, InvalidInput)
			return 0;
		}
		verif = base;

		i++;
	}

	SET_CTR(control, 0)
	return base;
}
