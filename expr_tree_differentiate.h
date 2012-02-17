/*
 *  expr_tree_differentiate.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

ExpressionObject* ExpressionTree::differentiate(string variable){
	ExpressionTree *differentiated = new ExpressionTree();
	switch (root->operation){
		case '+':
		case '-':
			differentiated->root = new ExpressionOperator(root->operation);
			for (int i = 0; i < children.size(); ++i){
				ExpressionObject *diff_child = children[i]->differentiate(variable);
				if (!diff_child) return ExpressionObject::invalid();
				ExpressionVariable *diff_child_var = dynamic_cast<ExpressionVariable*> (diff_child);
				if (diff_child_var != NULL && diff_child_var->isnumeric && diff_child_var->numericvalue == 0) continue;
				differentiated->children.push_back(diff_child);
			}
			break;
		case '*':
			differentiated->root = new ExpressionOperator('+');
			for (int i = 0; i < children.size(); ++i){
				ExpressionObject *diff_child = children[i]->differentiate(variable);
				if (!diff_child) return ExpressionObject::invalid();
				ExpressionVariable *diff_child_var = dynamic_cast<ExpressionVariable*> (diff_child);
				if (diff_child_var != NULL && diff_child_var->isnumeric && diff_child_var->numericvalue == 0) continue;
				bool diff_child_isone = diff_child_var != NULL && diff_child_var->isnumeric && diff_child_var->numericvalue == 1;
				
				if (diff_child_isone && children.size() == 2){
					differentiated->children.push_back(children[(i+1)%2]);					
				}else{
					ExpressionTree *prod = new ExpressionTree();
					prod->root = new ExpressionOperator('*');
					prod->children.push_back(diff_child);
					for (int j = 0; j < children.size(); ++j){
						if (j == i) continue;
						prod->children.push_back(children[j]);
					}
					differentiated->children.push_back(prod);
				}
			}
			break;
		case '/':
			return ExpressionObject::invalid();
			break;
		case '^':
			// at the moment, this will only differentiate constant powers 
			// e.g. (x+2)^3 and NOT (x+2)^x
			// and exponentials
			if (children.size() != 2) return ExpressionObject::invalid();
			differentiated->root = new ExpressionOperator('*');
			ExpressionObject *base = children[0];
			ExpressionVariable *base_var = dynamic_cast<ExpressionVariable*> (base);
			ExpressionObject *power = children[1];
			ExpressionVariable *power_var = dynamic_cast<ExpressionVariable*> (power);
			if (base_var && !base_var->isnumeric && base_var->var == "e"){
				ExpressionObject *power_diff = power->differentiate(variable);
				if (!power_diff) return ExpressionObject::invalid();
				differentiated->children.push_back(power_diff);
				differentiated->children.push_back(this);
			}else if (power_var && power_var->isnumeric){
				double new_power = power_var->numericvalue - 1;
				ExpressionObject *base_diff = base->differentiate(variable);
				if (!base_diff) return ExpressionObject::invalid();
				if (power_var->numericvalue != 1){
					differentiated->children.push_back(new ExpressionVariable(power_var->numericvalue));
				}
				differentiated->children.push_back(base_diff);
				if (new_power != 0 && new_power != 1){
					ExpressionTree *pow_tree = new ExpressionTree();
					pow_tree->root = new ExpressionOperator('^');
					pow_tree->children.push_back(base);
					pow_tree->children.push_back(new ExpressionVariable(new_power));
					differentiated->children.push_back(pow_tree);
				}else if(new_power == 1){
					differentiated->children.push_back(base);
				}
			}else{
				return ExpressionObject::invalid();
			}
			break;
	}
	
	return differentiated->optimize();
}