/*
 *  expr_tree_optimize_const_prod_pow.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

ExpressionObject* ExpressionTree::optimize_const_prod_pow(){
	if (root->operation == '^' && children.size() == 2){
		
		ExpressionTree *base_tree = dynamic_cast<ExpressionTree*> (children[0]);
		if (base_tree == NULL || base_tree->root->operation != '*') return this;
		
		ExpressionVariable *pow_var = dynamic_cast<ExpressionVariable*> (children[1]);
		if (pow_var == NULL || !pow_var->isnumeric) return this;
		
		double k = 1;
		vector<ExpressionObject*> base_vars;
		for (int i = 0; i < base_tree->children.size(); ++i){
			ExpressionVariable *base_var = dynamic_cast<ExpressionVariable*> (base_tree->children[i]);
			if (base_var && base_var->isnumeric){
				k *= pow(base_var->numericvalue, pow_var->numericvalue);
				continue;
			}
			base_vars.push_back(base_tree->children[i]);
		}
		
		if (base_vars.size() ==  1){
			children[0] = base_vars[0];
		}else{
			base_tree->children = base_vars;
		}
		
		if (k == 0){
			return new ExpressionVariable(0);
		}else if (k != 1){
			ExpressionTree *new_tree = new ExpressionTree();
			new_tree->root = new ExpressionOperator('*');
			new_tree->children.push_back(new ExpressionVariable(k));
			new_tree->children.push_back(this);
			return new_tree;
		}
	}
	return this;
}
