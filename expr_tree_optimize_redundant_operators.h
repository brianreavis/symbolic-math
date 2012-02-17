/*
 *  expr_tree_optimize_redundant_operators.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

ExpressionObject* ExpressionTree::optimize_redundant_operators(){
	if (children.size() == 1){
		return children[0];
	}
	
	vector<ExpressionObject*> new_children;
	new_children.reserve(children.size() + 5);
	
	for (int i = 0; i < children.size(); ++i){
		ExpressionTree *child_tree = dynamic_cast<ExpressionTree*> (children[i]);
		if (child_tree != NULL && (child_tree->children.size() == 1 || (child_tree->root->operation == root->operation && root->operation != '^'))){
			for (int j = 0; j < child_tree->children.size(); ++j){
				new_children.push_back(child_tree->children[j]);
			}
		}else{
			new_children.push_back(children[i]);
		}
	}
	
	children = new_children;
	return this;
}
