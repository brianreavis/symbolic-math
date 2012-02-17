/*
 *  expr_tree_evaluate.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


ExpressionObject* ExpressionTree::evaluate(ExpressionObject *value, string variable) {
	
	// evaluation if a power operation
	if (root->operation == '^'){
		ExpressionObject *base = children[0]->evaluate(value, variable);
		ExpressionVariable *base_var = dynamic_cast<ExpressionVariable*> (base);
		ExpressionObject *power = children[1]->evaluate(value, variable);
		ExpressionVariable *power_var = dynamic_cast<ExpressionVariable*> (power);
		if (power_var && power_var->isnumeric && power_var->numericvalue == 0){
			return new ExpressionVariable(1);
		}
		if (base_var && base_var->isnumeric && power_var && power_var->isnumeric){
			return new ExpressionVariable(pow(base_var->numericvalue, power_var->numericvalue));
		}
		ExpressionTree *ans = new ExpressionTree();
		ans->root = new ExpressionOperator('^');
		ans->children.push_back(base);
		ans->children.push_back(power);
		return ans->optimize();
	}
	
	double k = root->getk();
	vector<ExpressionObject*> nonscalars;
	int start = (root->operation == '-' || root->operation == '/') ? 1 : 0;
	
	// determine the object being divided / subtracted-from, if appropriate
	ExpressionObject *first;
	if (start == 1){
		first = children[0]->evaluate(value, variable);
		ExpressionVariable *first_var = dynamic_cast<ExpressionVariable*> (first);
		if (first_var && first_var->isnumeric){
			k = first_var->numericvalue;
			first = NULL;
		}
	}
	
	// evaluate the children
	for (int i = start; i < children.size(); ++i){
		ExpressionObject *child_result = children[i]->evaluate(value, variable);
		ExpressionVariable *child_result_var = dynamic_cast<ExpressionVariable*> (child_result);
		if (child_result_var != NULL && child_result_var->isnumeric){
			k = root->evaluate(k, child_result_var->numericvalue);
		}else{
			nonscalars.push_back(child_result);
		}
	}
	
	// assemble the result
	if (start == 1){
		if (k == root->getk() && nonscalars.size() == 0){
			return first == NULL ? new ExpressionVariable(k) : first;
		}
		ExpressionTree *ans = new ExpressionTree();
		ans->root = new ExpressionOperator(root->operation);
		ans->children.push_back(first == NULL ? new ExpressionVariable(k) : first);
		for (int i = 0; i < nonscalars.size(); ++i){
			ans->children.push_back(nonscalars[i]);
		}
		return ans->optimize();
	}else{
		if (k == root->getk()){
			if (nonscalars.size() == 0) return new ExpressionVariable(k);
			if (nonscalars.size() == 1) return nonscalars[0];
		}
		ExpressionTree *ans = new ExpressionTree();
		ans->root = new ExpressionOperator(root->operation);
		if (k != root->getk()){
			ans->children.push_back(new ExpressionVariable(k));
		}
		for (int i = 0; i < nonscalars.size(); ++i){
			ans->children.push_back(nonscalars[i]);
		}
		return ans->optimize();
	}
}


ExpressionObject* ExpressionTree::evaluate(double value, string variable) {
	return evaluate(new ExpressionVariable(value), variable);
}