/*
 *  expr_function.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/** 
 * EXPRESSION FUNCTION
 * represents fundamental mathematical functions 
 * (ln, log, sin, cos, tan, etc)
 */ 
class ExpressionFunction : public ExpressionObject {
public:
	// function classes
	static const char EXP = 1;
	static const char LOG = 2;
	static const char SIN = 3;
	static const char COS = 4;
	static const char TAN = 5;
};
