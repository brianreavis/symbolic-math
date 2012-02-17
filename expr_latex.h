/*
 *  expr_latex.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

string ExpressionVariable::toLaTeX(){
	if (isnumeric){
		int epos = var.find('e');
		if (epos == string::npos){
			return var;
		}else{
			// fix the formatting of the power up a bit
			stringstream power_str(var.substr(epos + 1));
			double power = 1;
			power_str >> power;
			
			ostringstream power_strstream;
			power_strstream.precision(3);
			power_strstream << power;
			
			string sci_notation = var.substr(0, epos);
			sci_notation.append("\\times 10^{");
			sci_notation.append(power_strstream.str());
			sci_notation.append("}");
			return sci_notation;
		}
	}
	return var;
}

string ExpressionTree::toLaTeX(){
	string str;
	string op(1, root->operation);
	for (int i = 0; i < children.size(); ++i){
		if (i > 0 && root->operation != '*') str.append(op);
		ExpressionTree *child_tree = dynamic_cast<ExpressionTree*> (children[i]);
		if (child_tree != NULL){
			bool wrap = false;
			bool wrap_bracket = false;
			if (child_tree->root->operation == '+' || child_tree->root->operation == '-'){
				wrap = root->operation == '*' || root->operation == '/' || root->operation == '^';
				wrap_bracket = root->operation == '^';
			}
			if (root->operation == '^'){
				wrap = true;
				wrap_bracket = true;
			}
			if (wrap) str.append(wrap_bracket ? "{\\left(" : "(");
			str.append(children[i]->toLaTeX());
			if (wrap) str.append(wrap_bracket ? "\\right)}" : ")");
		}else{
			bool implicit_bracket = root->operation == '^' || root->operation == '/';
			if (implicit_bracket) str.append("{");
			str.append(children[i]->toLaTeX());
			if (implicit_bracket) str.append("}");
		}
	}
	
	bool implicit_bracket = root->operation == '^';
	return (implicit_bracket ? "{" : "") + str + (implicit_bracket ? "}" : "");
}


string ExpressionVariable::toLaTeXTree(){
	return ExpressionVariable::toLaTeXTree(0);
}
string ExpressionVariable::toLaTeXTree(int level){
	if (level == 0){
		return "\\Tree [." + toLaTeX() + " ]"; 
	}else{
		return toLaTeX();
	}
}


string ExpressionTree::toLaTeXTree(){
	return ExpressionTree::toLaTeXTree(0);
}
string ExpressionTree::toLaTeXTree(int level){
	string str;
	string op = root->toLaTeX();
	if (level == 0){
		str.append("\\Tree ");
	}
	str.append("[." + op + " ");
	for (int i = 0; i < children.size(); ++i){
		str.append(children[i]->toLaTeXTree(level + 1));
		str.append(" ");
	}
	str.append("]");
	return str;
}