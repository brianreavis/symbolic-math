/*
 *  expr_parse.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

ExpressionObject* ExpressionObject::parse(string expr, bool optimizeResult){
	int i;
	int start = 0;
	int n = expr.length();
	list<ExpressionObject*> parts;
	list<char> rawbuffer;
	list<int> operators;
	
	// ignore parentheses at the beginning and end 
	// of the expression string whenever possible
	int j = 0;
	stack<char> parens;
	stack<bool> parens_removable;
	bool removable = true;
	int removable_count = 0;
	while (j < n){
		char c = expr[j];
		if (c == '(' || c == '['){
			parens.push(c);
			parens_removable.push(removable);
		}else if (c == ')' || c == ']'){
			char open = parens.top();
			if ((c == ')' && open == '(') || (c == ']' && open == '[')){
				bool open_removable = parens_removable.top();
				if (open_removable){
					++removable_count;
				}else{
					removable_count = 0;
				}
				parens.pop();
				parens_removable.pop();
			}else{
				return NULL;
			}
		}else{
			removable_count = 0;
			removable = false;
		}
		++j;
	}
	
	start = removable_count;
	n = expr.length() - removable_count;
	
	// determine the operator that will serve as the root of this tree.
	// add / subtract take precedence over multiply / divide in this step.
	i = start;
	char op_split = 0;
	while (i < n){
		char c = expr[i];
		switch (c){
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
				if (op_split == 0){
					op_split = c;
					if (c == '+' || c == '-') break;
				}else if(c == '+' || c == '-'){
					op_split = c;
					break;
				}else{
					if (op_split == '^' && (c == '*' || c == '/')){
						op_split = c;
					}
				}
				break;
			case '(':
			case '[':
				i = advance_matching_paren(expr, i, n);
				if (i == -1) return ExpressionObject::invalid();
				break;
		}
		++i;
	}
	
	if (op_split == 0){
		return new ExpressionVariable(expr.substr(start, n));
	}else{
		ExpressionTree *tree = new ExpressionTree();
		tree->root = new ExpressionOperator(op_split);
		// split by the root operator
		i = start;
		int raw_begin = i;
		while (i < n){
			if (expr[i] == '(' || expr[i] == '['){
				i = advance_matching_paren(expr, i, n);
			}else if(expr[i] == op_split){
				if (i - raw_begin > 0){
					tree->children.push_back(ExpressionObject::parse(expr.substr(raw_begin, i-raw_begin), optimizeResult));
				}
				raw_begin = i + 1;
				if (op_split == '^'){ i = n; break; }
			}
			++i;
		}
		if (i - raw_begin > 0){
			tree->children.push_back(ExpressionObject::parse(expr.substr(raw_begin, i-raw_begin), optimizeResult));
		}
		return (optimizeResult) ? tree->optimize() : tree;
	}
}

ExpressionObject* ExpressionObject::parse(string expr){
	return parse(expr, true);
}
