/*
 *  expr_tree_optimize_distribute_constants.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// 2(x+1)(x^2+1) -> (2x+2)(x^2+1)
ExpressionObject* ExpressionTree::optimize_distribute_constants(){
	if (root->operation == '*'){
		int k = 1;
		ExpressionTree *add_tree = NULL;
		
		// determine the scalar being distributed
		// along with the location of the (+) tree
		vector<ExpressionObject*> factors;
		for (int i = 0; i < children.size(); ++i){
			ExpressionVariable *child_var = dynamic_cast<ExpressionVariable*> (children[i]);
			if (child_var != NULL && child_var->isnumeric){
				k *= child_var->numericvalue;
				continue;
			}
			if (add_tree == NULL){
				ExpressionTree *child_tree = dynamic_cast<ExpressionTree*> (children[i]);
				if (child_tree && (child_tree->root->operation == '+' || child_tree->root->operation == '-')){
					add_tree = child_tree;
				}
			}
			factors.push_back(children[i]);
		}
		
		// distribute k
		if (k == 0) return new ExpressionVariable(0);
		if (k != 1 && add_tree != NULL){
			for (int i = 0; i < add_tree->children.size(); ++i){
				ExpressionObject *child = add_tree->children[i];
				ExpressionTree *child_tree = dynamic_cast<ExpressionTree*> (child);
				if (child_tree != NULL && child_tree->root->operation == '*'){
					child_tree->children.push_back(new ExpressionVariable(k));
				}else{
					ExpressionTree *prod_tree = new ExpressionTree();
					prod_tree->root = new ExpressionOperator('*');
					prod_tree->children.push_back(new ExpressionVariable(k));
					prod_tree->children.push_back(child);
					add_tree->children[i] = prod_tree;
				}
			}
			children = factors;
		}
		
	}
	return this;
}
