/*
 *  expr_operator.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef _EXPR_H

/** 
 * EXPRESSION OPERATOR
 * located at the root of an expression tree, expression
 * operators represent the operator that joins the children
 */
class ExpressionOperator {
public:
	static const char MULT = '*';
	static const char DIV  = '/';
	static const char ADD  = '+';
	static const char SUB  = '-';
	char operation;
	ExpressionOperator(char op) : operation(op) {};
	double getk(){
		if (operation == '*' || operation == '/') return 1;
		return 0;
	}
	double evaluate(double a, double b){
		switch (operation){
			case '+': return a + b;
			case '-': return a - b;
			case '*': return a * b;
			case '/': return a / b;
			case '^': return pow(a, b);
		}
		return 0;
	}
	
	//equality operators
	bool operator==(const ExpressionOperator &obj) const {
		return operation == obj.operation;
	}
	bool operator!=(const ExpressionOperator &obj) const {
		return !(*this == obj);
	}
	string toLaTeX(){
		switch (operation){
			case '+': return "$\\varoplus$";
			case '-': return "$\\varominus$";
			case '*': return "$\\varoast$";
			case '/': return "$\\varoslash$";
			case '^': return "$\\varowedge$";
		}	
		return "";
	}
	void print(ostream &stream){
		stream << "(" << operation << ")" << endl;
	}
	void print(ostream &stream, int level){
		cout << str_repeat(TAB, level);
		print(stream);
	}
};

#endif