/*
 *  expr_tree_optimize_consolidate_powers.h
 *  CS-Final-Project
 *
 *  Created by Brian Reavis on 12/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


ExpressionObject* ExpressionTree::optimize_consolidate_powers(){
	if (root->operation == '^' && children.size() == 2){
		double k = 1;
		vector<ExpressionObject*> powers;
		
		ExpressionObject *power, *base;
		ExpressionVariable *power_var;
		ExpressionTree *base_tree = this;
		do {
			base = base_tree->children[0];
			power = base_tree->children[1];
			base_tree = dynamic_cast<ExpressionTree*> (base);
			power_var = dynamic_cast<ExpressionVariable*> (power);
			if (power_var && power_var->isnumeric){
				k *= power_var->numericvalue;
			}else{
				ExpressionTree *power_tree = dynamic_cast<ExpressionTree*> (power);
				if (power_tree != NULL && power_tree->root->operation == '*'){
					for (int i = 0; i < power_tree->children.size(); ++i){
						powers.push_back(power_tree->children[i]);
					}
				}else{
					powers.push_back(power);
				}
			}
		} while(base_tree && base_tree->root->operation == '^' && base_tree->children.size() == 2);
		
		ExpressionTree *reduced = new ExpressionTree();
		reduced->root = new ExpressionOperator('^');
		reduced->children.push_back(base);
		if (k == 0) return new ExpressionVariable(0);
		if (powers.size() == 0 && k == 1){
			return base;
		}
		if (powers.size() == 1 && k == 1){
			reduced->children.push_back(powers[0]);
			return reduced;
		}
		
		ExpressionTree *new_power = new ExpressionTree();
		new_power->root = new ExpressionOperator('*');
		if (k != 1) new_power->children.push_back(new ExpressionVariable(k));
		for (int i = 0; i < powers.size(); ++i){
			new_power->children.push_back(powers[i]);
		}
		reduced->children.push_back(new_power);
		return reduced;
	}
	return this;
}