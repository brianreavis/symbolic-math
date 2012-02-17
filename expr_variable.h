/*
 *  expr_variable.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef _EXPR_H

class ExpressionVariable : public ExpressionObject {
public:
	string var;
	bool isnumeric;
	double numericvalue;
	
	// constructors
	ExpressionVariable(string variable) : var(variable) {
		stringstream strstream(variable);
		isnumeric = !((strstream >> numericvalue).fail());
	}
	ExpressionVariable(double value) : isnumeric(true), numericvalue(value) {
		ostringstream strstream;
		strstream << value;
		var = strstream.str();
	}
	
	ExpressionVariable* clone(){
		ExpressionVariable *obj = new ExpressionVariable(var);
		return obj;
	}
	
	//equality operators
	bool operator==(const ExpressionVariable &obj) const {
		if (isnumeric && !obj.isnumeric) return false;
		if (!isnumeric && obj.isnumeric) return false;
		if (isnumeric) return numericvalue == obj.numericvalue;
		return var == obj.var;
	}
	bool operator!=(const ExpressionVariable &obj) const {
		return !(*this == obj);
	}
	
	void print(ostream &stream){
		stream << var << endl;
	}
	void print(ostream &stream, int level){
		stream << str_repeat(TAB, level);
		print(stream);
	}
	string toLaTeX();
	string toLaTeXTree();
	string toLaTeXTree(int level);
	
	bool isNumeric(){
		return isnumeric;
	}
	ExpressionVariable* differentiate(string variable){
		if (isnumeric) return new ExpressionVariable(0);
		return new ExpressionVariable(var == variable ? 1 : 0);
	}
	ExpressionVariable* evaluate(double value, string variable) {
		if (isnumeric) this->clone();
		if (variable == var) return new ExpressionVariable(value);
		return this->clone();
	}
	ExpressionObject* evaluate(ExpressionObject *value, string variable) {
		if (isnumeric) return this->clone();
		if (variable == var) return value->clone();
		return this->clone();
	}
	ExpressionObject* optimize(){ return this; };
};

#endif
