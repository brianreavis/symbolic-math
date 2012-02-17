/*
 *  expr_tree_optimize_consolidate_factors.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


ExpressionObject* ExpressionTree::optimize_consolidate_factors(){
	if (root->operation == '*'){
		// are there any zero-coefficients?
		for (int i = 0; i < children.size(); ++i){
			ExpressionVariable *child_var = dynamic_cast<ExpressionVariable*> (children[i]);
			if (child_var != NULL && child_var->isnumeric && child_var->numericvalue == 0){
				return new ExpressionVariable(0);
			}
		}
		
		// join terms with identical factors
		vector<ExpressionObject*> final_terms;
		final_terms.reserve(children.size());
		
		set<int> indices;
		for (int i = 0; i < children.size(); ++i){
			indices.insert(i);
		}
		
		while (!indices.empty()){
			set<int>::iterator i_iter = indices.begin(); 
			int i = *i_iter;
			
			double pow_scalar;
			vector<ExpressionObject*> pow_vars;
			
			// determine the base of the current child (along with the power it's raised to)
			ExpressionObject *base;
			ExpressionTree *child_tree = dynamic_cast<ExpressionTree*> (children[i]);
			if (child_tree != NULL && child_tree->root->operation == '^'){
				base = child_tree->children[0];
				pow_scalar = 0;
				for (int p = 1; p < child_tree->children.size(); ++p){
					ExpressionVariable *power_var = dynamic_cast<ExpressionVariable*> (child_tree->children[p]);
					if (power_var != NULL && power_var->isnumeric){
						pow_scalar += power_var->numericvalue;
					}else{
						pow_vars.push_back(child_tree->children[p]);
					}
				}
			}else{
				base = children[i];
				pow_scalar = 1;
			}
			
			// look for identical siblings
			for (int j = 0; j < children.size(); ++j){
				if (j == i) continue;
				set<int>::iterator j_iter = indices.find(j);
				if (j_iter == indices.end()) continue;
				
				// is the term identical?
				if (ExpressionObject::equal(children[j], base)){
					++pow_scalar;
					indices.erase(indices.find(j));
					continue;
				}
				// is the term an identical term raised to a power?
				ExpressionTree *tree = dynamic_cast<ExpressionTree*> (children[j]);
				if (tree != NULL && tree->root->operation == '^'){
					ExpressionObject *base_sibling = tree->children[0];
					if (ExpressionObject::equal(base_sibling, base)){
						indices.erase(j_iter);
						for (int p = 1; p < tree->children.size(); ++p){
							ExpressionVariable *power_var = dynamic_cast<ExpressionVariable*> (tree->children[p]);
							if (power_var != NULL && power_var->isnumeric){
								pow_scalar += power_var->numericvalue;
							}else{
								pow_vars.push_back(tree->children[p]);
							}
						}
					}
				}
			}
			
			if (!pow_vars.empty()){
				ExpressionObject *pow_sum;
				if (pow_scalar != 0 || pow_vars.size() > 1){
					ExpressionTree *pow_sum_tree = new ExpressionTree();
					pow_sum_tree->root = new ExpressionOperator('+');
					if (pow_scalar != 0){
						pow_sum_tree->children.push_back(new ExpressionVariable(pow_scalar));
					}
					for (int j = 0; j < pow_vars.size(); ++j){
						pow_sum_tree->children.push_back(pow_vars[j]);
					}
					pow_sum = pow_sum_tree;
				}else{
					if (pow_scalar != 0) pow_sum = new ExpressionVariable(pow_scalar);
					else pow_sum = pow_vars[0];
				}
				ExpressionTree *pow = new ExpressionTree();
				pow->root = new ExpressionOperator('^');
				pow->children.push_back(base);
				pow->children.push_back(pow_sum);
				final_terms.push_back(pow);
			}else if(pow_scalar != 1){
				ExpressionTree *pow = new ExpressionTree();
				pow->root = new ExpressionOperator('^');
				pow->children.push_back(base);
				pow->children.push_back(new ExpressionVariable(pow_scalar));
				final_terms.push_back(pow);				
			}else{
				final_terms.push_back(base);
			}
			
			indices.erase(indices.find(i));
		}
		
		children = final_terms;
	}
	return this;
}
