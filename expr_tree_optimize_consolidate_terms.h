/*
 *  expr_tree_optimize_factor.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

ExpressionObject* ExpressionTree::optimize_consolidate_terms(){
	if (root->operation == '+'){
		ExpressionTree *child_tree;
		vector<ExpressionObject*> final_terms;
		final_terms.reserve(children.size());
		
		set<int> indices;
		for (int i = 0; i < children.size(); ++i){
			indices.insert(i);
		}
		
		while (!indices.empty()){
			set<int>::iterator i_iter = indices.begin(); 
			int i = *i_iter;
			
			double k = 1;
			
			// accumulate the variables / coefficients of this term 
			vector<ExpressionObject*> vars;
			child_tree = dynamic_cast<ExpressionTree*>(children[i]);
			if (child_tree != NULL){
				if (child_tree->root->operation == '*'){
					int j = 0;
					ExpressionVariable *expr_var = dynamic_cast<ExpressionVariable*>(child_tree->children[0]); 
					if (expr_var != NULL && expr_var->isnumeric){
						k = expr_var->numericvalue;
						j = 1;
					}
					while (j < child_tree->children.size()){
						vars.push_back(child_tree->children[j]);
						++j;
					}
				}else{
					vars.push_back(child_tree);
				}
			}else{
				vars.push_back(children[i]);
			}
			
			// check the other terms for identical variables
			// (variables existing in the vars vector)
			for (int j = 0; j < children.size(); ++j){
				if (j == i) continue;
				
				// accumulate the variables / coefficients of this sibling term
				double c = 1;
				vector<ExpressionObject*> vars_sibling;
				child_tree = dynamic_cast<ExpressionTree*>(children[j]);
				if (child_tree != NULL){
					if (child_tree->root->operation == '*'){
						int j = 0;
						ExpressionVariable *expr_var = dynamic_cast<ExpressionVariable*>(child_tree->children[0]); 
						if (expr_var != NULL && expr_var->isnumeric){
							c = expr_var->numericvalue;
							j = 1;
						}
						while (j < child_tree->children.size()){
							vars_sibling.push_back(child_tree->children[j]);
							++j;
						}
					}else{
						vars_sibling.push_back(child_tree);
					}
				}else{
					vars_sibling.push_back(children[j]);
				}
				
				// check to see if the variables are identical to those 
				// of the variable we're looking at currently
				if (vars_sibling.size() == vars.size()){
					for (int m = 0; m < vars.size(); ++m){
						bool sibling_found = false;
						for (int n = 0; n < vars_sibling.size(); ++n){
							if (ExpressionObject::equal(vars_sibling[n], vars[m])){
								sibling_found = true;
								vars_sibling.erase(vars_sibling.begin() + n);
								break;
							}
						}
						if (!sibling_found) break;
					}
					if (vars_sibling.size() == 0){
						indices.erase(indices.find(j));
						k += c;
					}
				}
			}
			
			// add the term to the final terms list
			if (k == 0){ 
				// do nothing
			}else if (vars.size() == 1 && k == 1){
				final_terms.push_back(vars[0]);
			}else{
				ExpressionTree *new_tree = new ExpressionTree();
				new_tree->root = new ExpressionOperator('*');
				if (k != 1){
					new_tree->children.push_back(new ExpressionVariable(k));
				}
				for (int m = 0; m < vars.size(); ++m){
					new_tree->children.push_back(vars[m]);
				}
				final_terms.push_back(new_tree);
			}
			indices.erase(indices.find(i));
		}
		
		// reassemble the children
		children.clear();
		if (final_terms.size() == 1 && (child_tree = dynamic_cast<ExpressionTree*>(final_terms[0]))){
			root->operation = child_tree->root->operation;
			children = child_tree->children;
		}else{
			for (int i = 0; i < final_terms.size(); ++i){
				children.push_back(final_terms[i]);
			}
		}
	}
	return this;
}
