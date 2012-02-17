/*
 *  expr_tree_optimize_consolidate_scalars.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


ExpressionObject* ExpressionTree::optimize_consolidate_scalars(){
	if (root->operation == '^') return this;
	double k = root->getk();
	vector<ExpressionObject*> children_new;
	children_new.push_back(NULL);
	for (int i = 0; i < children.size(); ++i){
		ExpressionVariable *child_var = dynamic_cast<ExpressionVariable*> (children[i]);
		if (child_var != NULL && child_var->isnumeric){
			k = root->evaluate(k, child_var->numericvalue);
		}else{
			children_new.push_back(children[i]);
		}
	}
	if (k != root->getk()){
		children_new[0] = new ExpressionVariable(k);
	}else{
		children_new.erase(children_new.begin());
	}
	children = children_new;
	return this;
}