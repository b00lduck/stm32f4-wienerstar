#include "main.h"
#include "expression.h"
#include "tools/strtof.h"

/**
 *
 */
float expressionParseAtom(char **expr) {
    char* end_ptr;
    float res;
    my_strtof(*expr, &end_ptr, &res);
    *expr = end_ptr;
    return res;
}

/**
 *
 */
float expressionParseFactors(char **expr) {
	float a = expressionParseAtom(expr);
	 while(1) {
		uint8_t op = **expr;
		if(op != '/' && op != '*') return a;
		(*expr)++;
		float b = expressionParseAtom(expr);
		if(op == '/') {
			a /= b;
		} else {
			a *= b;
		}
	}
}

/**
 *
 */
float expressionParseSummands(char **expr) {
	float a = expressionParseFactors(expr);
	while(1) {
		uint8_t op = **expr;
		if(op != '-' && op != '+') return a;
		(*expr)++;
		float b = expressionParseFactors(expr);
		if(op == '-') {
			a -= b;
		} else {
			a += b;
		}
	}
}


/**
 *
 */
float expressionEvaluateExpression(char *expr) {

	float ret = expressionParseSummands(&expr);

	return ret;

}
