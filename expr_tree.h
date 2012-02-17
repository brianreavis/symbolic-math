/*
 *  expr_tree.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef _EXPR_H

/**
 * EXPRESSION TREE
 * contains a list of expression objects joined 
 * by an operator.
 */
class ExpressionTree : public ExpressionObject {
public:
	vector<ExpressionObject*> children;
	
	// the operator that joins all the tree's children together
	ExpressionOperator *root;
	
	// default constructor
	ExpressionTree() : root(NULL) {};
	
	// clone
	ExpressionTree* clone() {
		ExpressionTree *obj = new ExpressionTree();
		obj->root = new ExpressionOperator(root->operation);
		for (int i = 0; i < children.size(); ++i){
			obj->children.push_back(children[i]->clone());
		}
		return obj;
	};
	
	// equality operator
	bool operator==(const ExpressionTree &obj) const {
		if (root == NULL && obj.root != NULL) return false;
		if (root != NULL && obj.root == NULL) return false;
		if (root->operation != obj.root->operation) return false;
		if (children.size() != obj.children.size()) return false;
		for (int i = 0; i < children.size(); ++i){
			if (!ExpressionObject::equal(children[i], obj.children[i])) return false;
		}
		return true;
	}
	bool operator!=(const ExpressionTree &obj) const {
		return !(*this == obj);
	}
	
	// calculates the size of the tree
	// (used for comparing trees for sorting purposes)
	int size(){
		int n = 1;
		for (int i = 0; i < children.size(); ++i){
			ExpressionTree *child_tree = dynamic_cast<ExpressionTree*> (children[i]);
			if (child_tree != NULL) n += child_tree->size();
			else ++n;
		}
		return n;
	}
	
	// reduces the complexity of the tree wherever possible.
	// e.g. 2x + 5x -> 7x
	
	ExpressionObject* optimize(){
		for (int i = 0; i < children.size(); ++i){
			children[i] = children[i]->optimize();
		}
		const int MAX_ITERATIONS = 4;
		int iteration = 0;		
		ExpressionObject *optimized = this;
		ExpressionObject *before;
		do {
			
			ExpressionTree *optimized_tree;
			before = optimized;
			
			optimized_tree = dynamic_cast<ExpressionTree*> (optimized); 
			
			// optimize_redundant_operators
			if (optimized_tree){
				optimized = optimized_tree->optimize_redundant_operators();
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized); 
			}
			// distribute constants (2(x+1) = 2x+2)
			if (optimized_tree){
				optimized = optimized_tree->optimize_distribute_constants();   
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized); 
			} 
			// optimize_consolidate_terms (8a + a = 9a)
			if (optimized_tree){
				optimized = optimized_tree->optimize_consolidate_terms();   
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized); 
			}			
			// optimize_consolidate_scalars (8 * 8 * x = 64 * x)
			if (optimized_tree){
				optimized = optimized_tree->optimize_consolidate_scalars(); 
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized); 
			}
			// optimize_const_prod_pow ( (2*x)^2 = 4(x)^2 )
			if (optimized_tree){
				optimized = optimized_tree->optimize_const_prod_pow();
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized);  
			}
			// optimize_consolidate_powers ((x^2)^2 = x^4)
			if (optimized_tree){
				optimized = optimized_tree->optimize_consolidate_powers();
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized);  
			}
			// optimize_consolidate_factors (x^2 * x^2 = x^4)
			if (optimized_tree){
				optimized = optimized_tree->optimize_consolidate_factors();
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized);  
			}
			// optimize_redundant_operators
			if (optimized_tree){
				optimized = optimized_tree->optimize_redundant_operators();
				optimized_tree = dynamic_cast<ExpressionTree*> (optimized); 
			}
			
			// reoptimize the children since they might have been changed 
			if (optimized_tree){
				for (int i = 0; i < optimized_tree->children.size(); ++i){
					optimized_tree->children[i] = optimized_tree->children[i]->optimize();
				}
			}
			
			// neaten up the children
			if (optimized_tree) optimized_tree->sort_children();
			
			++iteration;
		} while (iteration < MAX_ITERATIONS && !ExpressionObject::equal(optimized, before));
		
		return optimized;
	}
	
	ExpressionObject* optimize_redundant_operators();
	ExpressionObject* optimize_consolidate_terms();
	ExpressionObject* optimize_consolidate_scalars();
	ExpressionObject* optimize_consolidate_factors();
	ExpressionObject* optimize_consolidate_powers();
	ExpressionObject* optimize_distribute_constants();
	ExpressionObject* optimize_const_prod_pow();
	void sort_children();
	
	// differentiates the equation represented by the expression tree
	// and returns a tree representing the result.
	ExpressionObject* differentiate(string variable);
	
	// evaluates the expression tree for a given number
	ExpressionObject* evaluate(double value, string variable);
	ExpressionObject* evaluate(ExpressionObject *value, string variable);
	
	void print(ostream &stream){
		print(stream, 0);
	}
	void print(ostream &stream, int level){
		if (root) root->print(stream, level);
		for (int i = 0; i < children.size(); ++i){
			children[i]->print(stream, level + 1);
		}
	}
	string toLaTeX();
	string toLaTeXTree();
	string toLaTeXTree(int level);
};

#include "expr_tree_evaluate.h"
#include "expr_tree_differentiate.h"

#include "expr_tree_optimize_redundant_operators.h"
#include "expr_tree_optimize_consolidate_terms.h"
#include "expr_tree_optimize_consolidate_factors.h"
#include "expr_tree_optimize_consolidate_scalars.h"
#include "expr_tree_optimize_consolidate_powers.h"
#include "expr_tree_optimize_distribute_constants.h"
#include "expr_tree_optimize_const_prod_pow.h"

#include "expr_tree_sort_children.h"

#endif
