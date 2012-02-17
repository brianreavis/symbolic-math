#ifndef _EXPR_HELPERS_H
#define _EXPR_HELPERS_H
#include <string>
using std::string;

string str_repeat(string str, int count){
	string output = "";
	for (int i = 0; i < count; i++){
		output += str;
	}
	return output;
} 

// returns the index of the matching parentheis to the parentheis
// located at expr[start]. if the parenthesis are not properly nested,
// -1 is returned.
int advance_matching_paren(string expr, int start, int stopat){
	stack<char> parens;
	parens.push(expr[start]);
	int j = start + 1;
	int n = !stopat ? expr.length() : stopat;
	while (j < n){
		switch (expr[j]){
			case '(': 
			case '[':
				parens.push(expr[j]);
				break;
			case ')':
				if (parens.top() != '(') return -1;
				parens.pop();
				break;
			case ']':
				if (parens.top() != '[') return -1;
				parens.pop();
				break;
			default:
				++j;
				continue;
		}
		if (parens.empty()){
			return j;
		}
		++j;
	}
	if (!parens.empty()) return -1;
		return j;
	}

#endif