project(symbolic-math CXX)
set(HDS
expr.h
expr_function.h
expr_helpers.h
expr_latex.h
expr_operator.h
expr_parse.h
expr_tree.h
expr_tree_differentiate.h
expr_tree_evaluate.h
expr_tree_optimize_consolidate_factors.h
expr_tree_optimize_consolidate_powers.h
expr_tree_optimize_consolidate_scalars.h
expr_tree_optimize_consolidate_terms.h
expr_tree_optimize_const_prod_pow.h
expr_tree_optimize_distribute_constants.h
expr_tree_optimize_redundant_operators.h
expr_tree_sort_children.h
expr_variable.h
)

add_library(symbolic-math ${HDS} expr.cpp)

add_executable(main main.cpp)
target_link_libraries(main symbolic-math)
