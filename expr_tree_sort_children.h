/*
 *  expr_tree_sort_children.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

void ExpressionTree::sort_children(){
	if (root->operation == '+'){
		sort(children.begin(), children.end(), ExpressionObject::rsortcompare);
	}else if(root->operation == '*'){
		sort(children.begin(), children.end(), ExpressionObject::sortcompare);
	}
}