// Brian Reavis
// COSC2030
// 11/17/2010
// Symbolic Function Differentiator

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "expr.h"
using namespace std;

int main(){
	ifstream input;
	ofstream output;
	ofstream output_eval;
	ofstream output_eqns;
	ofstream output_large;
	input.open("tests.txt", ifstream::in);
	if (!input.is_open()){
		cerr << "Could not open tests.txt" << endl;
		exit(1);
	}
	output.open("tests_results.tex", ifstream::out);
	if (!output.is_open()){
		cerr << "Could not open tests_results.tex for writing." << endl;
		exit(1);
	}
	output_eval.open("tests_results_eval.tex", ifstream::out);
	if (!output_eval.is_open()){
		cerr << "Could not open tests_results_eval.tex for writing." << endl;
		exit(1);
	}
	output_eqns.open("eqns.tex", ifstream::out);
	if (!output_eqns.is_open()){
		cerr << "Could not open eqns.tex for writing." << endl;
		exit(1);
	}
	output_large.open("test_large.tex", ifstream::out);
	if (!output_large.is_open()){
		cerr << "Could not open test_large.tex for writing." << endl;
		exit(1);
	}

	string line;
	int n = 0;
	while (getline(input, line)){
		ExpressionObject *expr = ExpressionObject::parse(line, false);
		if (!expr){
			cout << "Failed to parse: " << line << endl;
			continue;
		}
		++n;
		
		output << "% --- " << line << " ---" << endl;
		output << "\\item[(" << n << ")]" << endl;
		
		// original expression
		output << "\\parbox[t]{\\treeboxwidth}{" << endl;
		output << "\t\\noindent$f(x)=" << expr->toLaTeX() << "$" << endl;
		output << "\t\\vspace{1em}\\newline" << expr->toLaTeXTree() << endl;
		output << "}" << endl;
		
		output_eqns << "$" << expr->toLaTeX() << "$ & $\\rightarrow$ & \\verb|" << line << "|\\\\" << endl; 
		
		// optimized expression
		expr = expr->optimize();
		output << "\\parbox[t]{\\treeboxwidth}{" << endl;
		output << "\t\\noindent$g(x)=" << expr->toLaTeX() << "$" << endl;
		output << "\t\\vspace{1em}\\newline" << expr->toLaTeXTree() << endl;
		output << "}" << endl;
		
		output_eval << "% --- " << line << " ---" << endl;
		output_eval << "\\begin{align*}" << endl;
		output_eval << "f(x) &= " << expr->toLaTeX() << "\\\\" << endl;
		output_eval << "f(0) &= " << expr->evaluate(0.0, "x")->toLaTeX() << "\\\\ " << endl;
		output_eval << "f(1) &= " << expr->evaluate(1.0, "x")->toLaTeX() << "\\\\ " << endl;
		output_eval << "f(3) &= " << expr->evaluate(3.0, "x")->toLaTeX() << "\\\\ " << endl;
		output_eval << "f(x^2) &= " << expr->evaluate(ExpressionObject::parse("x^2"), "x")->toLaTeX() << "\\\\" << endl;
		output_eval << "\\end{align*}" << endl;
		
		// differentiated expression
	
		ExpressionObject *expr_diff = expr->differentiate("x");
		if (expr_diff){
			output << "\\parbox[t]{\\treeboxwidth}{" << endl;
			output << "\t\\noindent$\\frac{d}{dx}g(x)=" << expr_diff->toLaTeX() << "$" << endl;
			output << "\t\\vspace{1em}\\newline" << expr_diff->toLaTeXTree() << endl;
			output << "}" << endl;

			output_eval << "\\begin{align*}" << endl;
			output_eval << "f'(x) &= " << expr_diff->toLaTeX() << "\\\\" << endl;
			output_eval << "f'(0) &= " << expr_diff->evaluate(0.0, "x")->toLaTeX() << "\\\\ " << endl;
			output_eval << "f'(1) &= " << expr_diff->evaluate(1.0, "x")->toLaTeX() << "\\\\" << endl;
			output_eval << "f'(3) &= " << expr_diff->evaluate(3.0, "x")->toLaTeX() << "\\\\ " << endl;
			output_eval << "f'(x^2) &= " << expr_diff->evaluate(ExpressionObject::parse("x^2"), "x")->toLaTeX() << "\\\\" << endl;
			output_eval << "\\end{align*}" << endl;
		}else{
			output << "\\parbox[t]{\\treeboxwidth}{" << endl;
			output << "\t\\noindent$\\frac{d}{dx}g(x)=\\varnothing$" << endl;
			output << "}" << endl;
			
			output_eval << "\\begin{align*}" << endl;
			output_eval << "f'(x) &= \\varnothing\\\\" << endl;
			output_eval << "\\end{align*}" << endl;
			cout << "Could not differentiate: " << line << endl;
		}
		
		cout << line << endl;
		
	}
	
	ExpressionObject *large = ExpressionObject::parse("5*x+x^2*((x+2)*x^3)^4");
	output_large << "$f(x)=" << large->toLaTeX() << "$" << endl << endl; 
	ExpressionObject *large_diff = large->differentiate("x");
	output_large << "$f'(x)=" << large_diff->toLaTeX() << "$" << endl << endl;
	output_large << large_diff->toLaTeXTree() << endl;
	
	output_large.close();
	output_eqns.close();
	output_eval.close();
	output.close();
	input.close();
	 
	return 0;
}