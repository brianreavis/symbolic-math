/*
 *  expr.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// Features: 
// - terms automatically sorted by complexity 
// - 

#ifndef _EXPR_H
#define _EXPR_H

#include <string>
#include <stack>
#include <list>
#include <vector>
#include <sstream>
#include <math.h>
#include <set>
#include <algorithm>
using namespace std;

#define TAB "   "

/**
 * EXPRESSION OBJECT 
 * the abstract class lining out the methods
 * that ExpressionVariable and ExpressionTree share.
 */
class ExpressionObject {
public:
	virtual ExpressionObject* clone() = 0;
	
	// parses a mathematical expression given as a string. if the 
	// expression is cannot be parsed, the tree that is returned will
	// have tree.valid set to false.
	static ExpressionObject* parse(string expr, bool optimizeResult);
	static ExpressionObject* parse(string expr);
		
	virtual void print(ostream &stream) = 0;
	virtual void print(ostream &stream, int level) = 0;
	virtual string toLaTeX() = 0;
	virtual string toLaTeXTree() = 0;
	virtual string toLaTeXTree(int level) = 0;
	
	virtual ExpressionObject* evaluate(double value, string variable) = 0;
	virtual ExpressionObject* evaluate(ExpressionObject *value, string variable) = 0;
	virtual ExpressionObject* differentiate(string variable) = 0;
	virtual ExpressionObject* optimize() = 0;
	
	static int  compare(ExpressionObject *obj1, ExpressionObject *obj2);
	static bool sortcompare(ExpressionObject *obj1, ExpressionObject *obj2);
	static bool rsortcompare(ExpressionObject *obj1, ExpressionObject *obj2);
	static bool equal(ExpressionObject *obj1, ExpressionObject *obj2);
	
	// returns an expression that is marked as 'invalid' 
	// (i.e. it couldn't be parsed successfully)
	static ExpressionObject* invalid(){
		return NULL;
	};
};

#include "expr_helpers.h"

#include "expr_operator.h"
#include "expr_variable.h"
#include "expr_tree.h"
#include "expr_parse.h"
#include "expr_latex.h"


bool operator == (const ExpressionTree &obj1, const ExpressionVariable &obj2) { return false; }
bool operator != (const ExpressionTree &obj1, const ExpressionVariable &obj2) { return true; }
bool operator == (const ExpressionVariable &obj1, const ExpressionTree &obj2) { return false; }
bool operator != (const ExpressionVariable &obj1, const ExpressionTree &obj2) { return true; }

bool ExpressionObject::equal(ExpressionObject *obj1, ExpressionObject *obj2){
	ExpressionTree *obj1_tree = dynamic_cast<ExpressionTree*> (obj1);
	ExpressionTree *obj2_tree = dynamic_cast<ExpressionTree*> (obj2);
	ExpressionVariable *obj1_var = obj1_tree == NULL ? dynamic_cast<ExpressionVariable*> (obj1) : NULL;
	ExpressionVariable *obj2_var = obj2_tree == NULL ? dynamic_cast<ExpressionVariable*> (obj2) : NULL;
	if (obj1_tree && obj2_tree) return (*obj1_tree) == (*obj2_tree);
	if (obj1_var && obj2_var) return (*obj1_var) == (*obj2_var);
	return false;
}

int ExpressionObject::compare(ExpressionObject *obj1, ExpressionObject *obj2){
	ExpressionTree *obj1_tree = dynamic_cast<ExpressionTree*> (obj1);
	ExpressionTree *obj2_tree = dynamic_cast<ExpressionTree*> (obj2);
	ExpressionVariable *obj1_var = obj1_tree == NULL ? dynamic_cast<ExpressionVariable*> (obj1) : NULL;
	ExpressionVariable *obj2_var = obj2_tree == NULL ? dynamic_cast<ExpressionVariable*> (obj2) : NULL;
	
	// differing types
	if (obj1_tree && obj2_var) return 1;
	if (obj2_tree && obj1_var) return -1;
	
	// identical types (tree)
	if (obj1_tree && obj2_tree){
		if ((*obj1_tree) == (*obj2_tree)) return 0;
		int tree1_size = obj1_tree->size();
		int tree2_size = obj2_tree->size();
		if (tree1_size != tree2_size) return tree1_size > tree2_size ? 1 : -1;
		return 0; // <-- why to never, ever use this compare function for equality-checking purposes
	}
	
	// identical types (var)
	if (obj1_var && obj2_var){
		if ((*obj1_var) == (*obj2_var)) return 0;
		if (obj1_var->isnumeric && !obj2_var->isnumeric) return -1;
		if (obj2_var->isnumeric && !obj1_var->isnumeric) return 1;
		if (obj1_var->isnumeric && obj2_var->isnumeric) return obj1_var->numericvalue > obj2_var->numericvalue ? 1 : -1;
		return obj1_var->var.compare(obj2_var->var);
	}
	
	return 0;
}

bool ExpressionObject::sortcompare(ExpressionObject *obj1, ExpressionObject *obj2){
	// true if first goes before second
	int cmp = ExpressionObject::compare(obj1, obj2);
	return cmp < 0;
}

bool ExpressionObject::rsortcompare(ExpressionObject *obj1, ExpressionObject *obj2){
	return !ExpressionObject::sortcompare(obj1, obj2);
}

#endif