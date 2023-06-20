/* Helpers for the autogenerated gimple-match.cc file.
   Copyright (C) 2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "ssa.h"
#include "cgraph.h"
#include "vec-perm-indices.h"
#include "fold-const.h"
#include "fold-const-call.h"
#include "stor-layout.h"
#include "gimple-iterator.h"
#include "gimple-fold.h"
#include "calls.h"
#include "tree-dfa.h"
#include "builtins.h"
#include "gimple-match.h"
#include "tree-pass.h"
#include "internal-fn.h"
#include "case-cfn-macros.h"
#include "gimplify.h"
#include "optabs-tree.h"
#include "tree-eh.h"
#include "dbgcnt.h"
#include "tm.h"
#include "gimple-range.h"
#include "langhooks.h"

tree (*mprts_hook) (gimple_match_op *);

extern bool gimple_simplify (gimple_match_op *, gimple_seq *, tree (*)(tree),
			     code_helper, tree, tree);
extern bool gimple_simplify (gimple_match_op *, gimple_seq *, tree (*)(tree),
			     code_helper, tree, tree, tree);
extern bool gimple_simplify (gimple_match_op *, gimple_seq *, tree (*)(tree),
			     code_helper, tree, tree, tree, tree);
extern bool gimple_simplify (gimple_match_op *, gimple_seq *, tree (*)(tree),
			     code_helper, tree, tree, tree, tree, tree);
extern bool gimple_simplify (gimple_match_op *, gimple_seq *, tree (*)(tree),
			     code_helper, tree, tree, tree, tree, tree, tree);

/* Functions that are needed by gimple-match but that are exported and used in
   other places in the compiler.  */

tree gimple_simplify (enum tree_code, tree, tree, gimple_seq *,
		     tree (*)(tree));
tree gimple_simplify (enum tree_code, tree, tree, tree, gimple_seq *,
		     tree (*)(tree));
tree gimple_simplify (enum tree_code, tree, tree, tree, tree, gimple_seq *,
		     tree (*)(tree));
tree gimple_simplify (combined_fn, tree, tree, gimple_seq *,
		     tree (*)(tree));
tree gimple_simplify (combined_fn, tree, tree, tree, gimple_seq *,
		     tree (*)(tree));
tree gimple_simplify (combined_fn, tree, tree, tree, tree, gimple_seq *,
		     tree (*)(tree));

tree do_valueize (tree, tree (*)(tree), bool &);
tree do_valueize (tree (*)(tree), tree);

/* Forward declarations of the private auto-generated matchers.
   They expect valueized operands in canonical order and do not
   perform simplification of all-constant operands.  */

static bool gimple_resimplify1 (gimple_seq *, gimple_match_op *, tree (*)(tree));
static bool gimple_resimplify2 (gimple_seq *, gimple_match_op *, tree (*)(tree));
static bool gimple_resimplify3 (gimple_seq *, gimple_match_op *, tree (*)(tree));
static bool gimple_resimplify4 (gimple_seq *, gimple_match_op *, tree (*)(tree));
static bool gimple_resimplify5 (gimple_seq *, gimple_match_op *, tree (*)(tree));

/* Match and simplify the toplevel valueized operation THIS.
   Replaces THIS with a simplified and/or canonicalized result and
   returns whether any change was made.  */

bool
gimple_match_op::resimplify (gimple_seq *seq, tree (*valueize)(tree))
{
  switch (num_ops)
    {
    case 1:
      return gimple_resimplify1 (seq, this, valueize);
    case 2:
      return gimple_resimplify2 (seq, this, valueize);
    case 3:
      return gimple_resimplify3 (seq, this, valueize);
    case 4:
      return gimple_resimplify4 (seq, this, valueize);
    case 5:
      return gimple_resimplify5 (seq, this, valueize);
    default:
      gcc_unreachable ();
    }
}

/* Return whether T is a constant that we'll dispatch to fold to
   evaluate fully constant expressions.  */

static inline bool
constant_for_folding (tree t)
{
  return (CONSTANT_CLASS_P (t)
	  /* The following is only interesting to string builtins.  */
	  || (TREE_CODE (t) == ADDR_EXPR
	      && TREE_CODE (TREE_OPERAND (t, 0)) == STRING_CST));
}

/* Try to convert conditional operation ORIG_OP into an IFN_COND_*
   operation.  Return true on success, storing the new operation in NEW_OP.  */

static bool
convert_conditional_op (gimple_match_op *orig_op,
			gimple_match_op *new_op)
{
  internal_fn ifn;
  if (orig_op->code.is_tree_code ())
    ifn = get_conditional_internal_fn ((tree_code) orig_op->code);
  else
    {
      auto cfn = combined_fn (orig_op->code);
      if (!internal_fn_p (cfn))
	return false;
      ifn = get_conditional_internal_fn (as_internal_fn (cfn));
    }
  if (ifn == IFN_LAST)
    return false;
  unsigned int num_ops = orig_op->num_ops;
  new_op->set_op (as_combined_fn (ifn), orig_op->type, num_ops + 2);
  new_op->ops[0] = orig_op->cond.cond;
  for (unsigned int i = 0; i < num_ops; ++i)
    new_op->ops[i + 1] = orig_op->ops[i];
  tree else_value = orig_op->cond.else_value;
  if (!else_value)
    else_value = targetm.preferred_else_value (ifn, orig_op->type,
					       num_ops, orig_op->ops);
  new_op->ops[num_ops + 1] = else_value;
  return true;
}
/* Helper for gimple_simplify valueizing OP using VALUEIZE and setting
   VALUEIZED to true if valueization changed OP.  */

inline tree
do_valueize (tree op, tree (*valueize)(tree), bool &valueized)
{
  if (valueize && TREE_CODE (op) == SSA_NAME)
    {
      tree tem = valueize (op);
      if (tem && tem != op)
	{
	  op = tem;
	  valueized = true;
	}
    }
  return op;
}

/* If in GIMPLE the operation described by RES_OP should be single-rhs,
   build a GENERIC tree for that expression and update RES_OP accordingly.  */

void
maybe_build_generic_op (gimple_match_op *res_op)
{
  tree_code code = (tree_code) res_op->code;
  tree val;
  switch (code)
    {
    case REALPART_EXPR:
    case IMAGPART_EXPR:
    case VIEW_CONVERT_EXPR:
      val = build1 (code, res_op->type, res_op->ops[0]);
      res_op->set_value (val);
      break;
    case BIT_FIELD_REF:
      val = build3 (code, res_op->type, res_op->ops[0], res_op->ops[1],
		    res_op->ops[2]);
      REF_REVERSE_STORAGE_ORDER (val) = res_op->reverse;
      res_op->set_value (val);
      break;
    default:;
    }
}

/* Try to build RES_OP, which is known to be a call to FN.  Return null
   if the target doesn't support the function.  */

static gcall *
build_call_internal (internal_fn fn, gimple_match_op *res_op)
{
  if (direct_internal_fn_p (fn))
    {
      tree_pair types = direct_internal_fn_types (fn, res_op->type,
						  res_op->ops);
      if (!direct_internal_fn_supported_p (fn, types, OPTIMIZE_FOR_BOTH))
	return NULL;
    }
  return gimple_build_call_internal (fn, res_op->num_ops,
				     res_op->op_or_null (0),
				     res_op->op_or_null (1),
				     res_op->op_or_null (2),
				     res_op->op_or_null (3),
				     res_op->op_or_null (4));
}

/* RES_OP is the result of a simplification.  If it is conditional,
   try to replace it with the equivalent UNCOND form, such as an
   IFN_COND_* call or a VEC_COND_EXPR.  Also try to resimplify the
   result of the replacement if appropriate, adding any new statements to
   SEQ and using VALUEIZE as the valueization function.  Return true if
   this resimplification occurred and resulted in at least one change.  */

static bool
maybe_resimplify_conditional_op (gimple_seq *seq, gimple_match_op *res_op,
				 tree (*valueize) (tree))
{
  if (!res_op->cond.cond)
    return false;

  if (!res_op->cond.else_value
      && res_op->code.is_tree_code ())
    {
      /* The "else" value doesn't matter.  If the "then" value is a
	 gimple value, just use it unconditionally.  This isn't a
	 simplification in itself, since there was no operation to
	 build in the first place.  */
      if (gimple_simplified_result_is_gimple_val (res_op))
	{
	  res_op->cond.cond = NULL_TREE;
	  return false;
	}

      /* Likewise if the operation would not trap.  */
      bool honor_trapv = (INTEGRAL_TYPE_P (res_op->type)
			  && TYPE_OVERFLOW_TRAPS (res_op->type));
      tree_code op_code = (tree_code) res_op->code;
      bool op_could_trap;

      /* COND_EXPR will trap if, and only if, the condition
	 traps and hence we have to check this.  For all other operations, we
	 don't need to consider the operands.  */
      if (op_code == COND_EXPR)
	op_could_trap = generic_expr_could_trap_p (res_op->ops[0]);
      else
	op_could_trap = operation_could_trap_p ((tree_code) res_op->code,
						FLOAT_TYPE_P (res_op->type),
						honor_trapv,
						res_op->op_or_null (1));

      if (!op_could_trap)
	{
	  res_op->cond.cond = NULL_TREE;
	  return false;
	}
    }

  /* If the "then" value is a gimple value and the "else" value matters,
     create a VEC_COND_EXPR between them, then see if it can be further
     simplified.  */
  gimple_match_op new_op;
  if (res_op->cond.else_value
      && VECTOR_TYPE_P (res_op->type)
      && gimple_simplified_result_is_gimple_val (res_op))
    {
      new_op.set_op (VEC_COND_EXPR, res_op->type,
		     res_op->cond.cond, res_op->ops[0],
		     res_op->cond.else_value);
      *res_op = new_op;
      return gimple_resimplify3 (seq, res_op, valueize);
    }

  /* Otherwise try rewriting the operation as an IFN_COND_* call.
     Again, this isn't a simplification in itself, since it's what
     RES_OP already described.  */
  if (convert_conditional_op (res_op, &new_op))
    *res_op = new_op;

  return false;
}

/* If RES_OP is a call to a conditional internal function, try simplifying
   the associated unconditional operation and using the result to build
   a new conditional operation.  For example, if RES_OP is:

     IFN_COND_ADD (COND, A, B, ELSE)

   try simplifying (plus A B) and using the result to build a replacement
   for the whole IFN_COND_ADD.

   Return true if this approach led to a simplification, otherwise leave
   RES_OP unchanged (and so suitable for other simplifications).  When
   returning true, add any new statements to SEQ and use VALUEIZE as the
   valueization function.

   RES_OP is known to be a call to IFN.  */

static bool
try_conditional_simplification (internal_fn ifn, gimple_match_op *res_op,
				gimple_seq *seq, tree (*valueize) (tree))
{
  code_helper op;
  tree_code code = conditional_internal_fn_code (ifn);
  if (code != ERROR_MARK)
    op = code;
  else
    {
      ifn = get_unconditional_internal_fn (ifn);
      if (ifn == IFN_LAST)
	return false;
      op = as_combined_fn (ifn);
    }

  unsigned int num_ops = res_op->num_ops;
  gimple_match_op cond_op (gimple_match_cond (res_op->ops[0],
					      res_op->ops[num_ops - 1]),
			   op, res_op->type, num_ops - 2);

  memcpy (cond_op.ops, res_op->ops + 1, (num_ops - 1) * sizeof *cond_op.ops);
  switch (num_ops - 2)
    {
    case 1:
      if (!gimple_resimplify1 (seq, &cond_op, valueize))
	return false;
      break;
    case 2:
      if (!gimple_resimplify2 (seq, &cond_op, valueize))
	return false;
      break;
    case 3:
      if (!gimple_resimplify3 (seq, &cond_op, valueize))
	return false;
      break;
    default:
      gcc_unreachable ();
    }
  *res_op = cond_op;
  maybe_resimplify_conditional_op (seq, res_op, valueize);
  return true;
}

/* Helper for the autogenerated code, valueize OP.  */

tree
do_valueize (tree (*valueize)(tree), tree op)
{
  if (valueize && TREE_CODE (op) == SSA_NAME)
    {
      tree tem = valueize (op);
      if (tem)
	return tem;
    }
  return op;
}

/* Push the exploded expression described by RES_OP as a statement to
   SEQ if necessary and return a gimple value denoting the value of the
   expression.  If RES is not NULL then the result will be always RES
   and even gimple values are pushed to SEQ.  */

tree
maybe_push_res_to_seq (gimple_match_op *res_op, gimple_seq *seq, tree res)
{
  tree *ops = res_op->ops;
  unsigned num_ops = res_op->num_ops;

  /* The caller should have converted conditional operations into an UNCOND
     form and resimplified as appropriate.  The conditional form only
     survives this far if that conversion failed.  */
  if (res_op->cond.cond)
    return NULL_TREE;

  if (res_op->code.is_tree_code ())
    {
      if (!res
	  && gimple_simplified_result_is_gimple_val (res_op))
	return ops[0];
      if (mprts_hook)
	{
	  tree tem = mprts_hook (res_op);
	  if (tem)
	    return tem;
	}
    }

  if (!seq)
    return NULL_TREE;

  /* Play safe and do not allow abnormals to be mentioned in
     newly created statements.  */
  for (unsigned int i = 0; i < num_ops; ++i)
    if (TREE_CODE (ops[i]) == SSA_NAME
	&& SSA_NAME_OCCURS_IN_ABNORMAL_PHI (ops[i]))
      return NULL_TREE;

  if (num_ops > 0 && COMPARISON_CLASS_P (ops[0]))
    for (unsigned int i = 0; i < 2; ++i)
      if (TREE_CODE (TREE_OPERAND (ops[0], i)) == SSA_NAME
	  && SSA_NAME_OCCURS_IN_ABNORMAL_PHI (TREE_OPERAND (ops[0], i)))
	return NULL_TREE;

  if (res_op->code.is_tree_code ())
    {
      auto code = tree_code (res_op->code);
      if (!res)
	{
	  if (gimple_in_ssa_p (cfun))
	    res = make_ssa_name (res_op->type);
	  else
	    res = create_tmp_reg (res_op->type);
	}
      maybe_build_generic_op (res_op);
      gimple *new_stmt = gimple_build_assign (res, code,
					      res_op->op_or_null (0),
					      res_op->op_or_null (1),
					      res_op->op_or_null (2));
      gimple_seq_add_stmt_without_update (seq, new_stmt);
      return res;
    }
  else
    {
      gcc_assert (num_ops != 0);
      auto fn = combined_fn (res_op->code);
      gcall *new_stmt = NULL;
      if (internal_fn_p (fn))
	{
	  /* Generate the given function if we can.  */
	  internal_fn ifn = as_internal_fn (fn);
	  new_stmt = build_call_internal (ifn, res_op);
	  if (!new_stmt)
	    return NULL_TREE;
	}
      else
	{
	  /* Find the function we want to call.  */
	  tree decl = builtin_decl_implicit (as_builtin_fn (fn));
	  if (!decl)
	    return NULL;

	  /* We can't and should not emit calls to non-const functions.  */
	  if (!(flags_from_decl_or_type (decl) & ECF_CONST))
	    return NULL;

	  new_stmt = gimple_build_call (decl, num_ops,
					res_op->op_or_null (0),
					res_op->op_or_null (1),
					res_op->op_or_null (2),
					res_op->op_or_null (3),
					res_op->op_or_null (4));
	}
      if (!res)
	{
	  if (gimple_in_ssa_p (cfun))
	    res = make_ssa_name (res_op->type);
	  else
	    res = create_tmp_reg (res_op->type);
	}
      gimple_call_set_lhs (new_stmt, res);
      gimple_seq_add_stmt_without_update (seq, new_stmt);
      return res;
    }
}


/* Public API overloads follow for operation being tree_code or
   built_in_function and for one to three operands or arguments.
   They return NULL_TREE if nothing could be simplified or
   the resulting simplified value with parts pushed to SEQ.
   If SEQ is NULL then if the simplification needs to create
   new stmts it will fail.  If VALUEIZE is non-NULL then all
   SSA names will be valueized using that hook prior to
   applying simplifications.  */

/* Unary ops.  */

tree
gimple_simplify (enum tree_code code, tree type,
		 tree op0,
		 gimple_seq *seq, tree (*valueize)(tree))
{
  if (constant_for_folding (op0))
    {
      tree res = const_unop (code, type, op0);
      if (res != NULL_TREE
	  && CONSTANT_CLASS_P (res))
	return res;
    }

  gimple_match_op res_op;
  if (!gimple_simplify (&res_op, seq, valueize, code, type, op0))
    return NULL_TREE;
  return maybe_push_res_to_seq (&res_op, seq);
}

/* Binary ops.  */

tree
gimple_simplify (enum tree_code code, tree type,
		 tree op0, tree op1,
		 gimple_seq *seq, tree (*valueize)(tree))
{
  if (constant_for_folding (op0) && constant_for_folding (op1))
    {
      tree res = const_binop (code, type, op0, op1);
      if (res != NULL_TREE
	  && CONSTANT_CLASS_P (res))
	return res;
    }

  /* Canonicalize operand order both for matching and fallback stmt
     generation.  */
  if ((commutative_tree_code (code)
       || TREE_CODE_CLASS (code) == tcc_comparison)
      && tree_swap_operands_p (op0, op1))
    {
      std::swap (op0, op1);
      if (TREE_CODE_CLASS (code) == tcc_comparison)
	code = swap_tree_comparison (code);
    }

  gimple_match_op res_op;
  if (!gimple_simplify (&res_op, seq, valueize, code, type, op0, op1))
    return NULL_TREE;
  return maybe_push_res_to_seq (&res_op, seq);
}

/* Ternary ops.  */

tree
gimple_simplify (enum tree_code code, tree type,
		 tree op0, tree op1, tree op2,
		 gimple_seq *seq, tree (*valueize)(tree))
{
  if (constant_for_folding (op0) && constant_for_folding (op1)
      && constant_for_folding (op2))
    {
      tree res = fold_ternary/*_to_constant */ (code, type, op0, op1, op2);
      if (res != NULL_TREE
	  && CONSTANT_CLASS_P (res))
	return res;
    }

  /* Canonicalize operand order both for matching and fallback stmt
     generation.  */
  if (commutative_ternary_tree_code (code)
      && tree_swap_operands_p (op0, op1))
    std::swap (op0, op1);

  gimple_match_op res_op;
  if (!gimple_simplify (&res_op, seq, valueize, code, type, op0, op1, op2))
    return NULL_TREE;
  return maybe_push_res_to_seq (&res_op, seq);
}

/* Builtin or internal function with one argument.  */

tree
gimple_simplify (combined_fn fn, tree type,
		 tree arg0,
		 gimple_seq *seq, tree (*valueize)(tree))
{
  if (constant_for_folding (arg0))
    {
      tree res = fold_const_call (fn, type, arg0);
      if (res && CONSTANT_CLASS_P (res))
	return res;
    }

  gimple_match_op res_op;
  if (!gimple_simplify (&res_op, seq, valueize, fn, type, arg0))
    return NULL_TREE;
  return maybe_push_res_to_seq (&res_op, seq);
}

/* Builtin or internal function with two arguments.  */

tree
gimple_simplify (combined_fn fn, tree type,
		 tree arg0, tree arg1,
		 gimple_seq *seq, tree (*valueize)(tree))
{
  if (constant_for_folding (arg0)
      && constant_for_folding (arg1))
    {
      tree res = fold_const_call (fn, type, arg0, arg1);
      if (res && CONSTANT_CLASS_P (res))
	return res;
    }

  gimple_match_op res_op;
  if (!gimple_simplify (&res_op, seq, valueize, fn, type, arg0, arg1))
    return NULL_TREE;
  return maybe_push_res_to_seq (&res_op, seq);
}

/* Builtin or internal function with three arguments.  */

tree
gimple_simplify (combined_fn fn, tree type,
		 tree arg0, tree arg1, tree arg2,
		 gimple_seq *seq, tree (*valueize)(tree))
{
  if (constant_for_folding (arg0)
      && constant_for_folding (arg1)
      && constant_for_folding (arg2))
    {
      tree res = fold_const_call (fn, type, arg0, arg1, arg2);
      if (res && CONSTANT_CLASS_P (res))
	return res;
    }

  gimple_match_op res_op;
  if (!gimple_simplify (&res_op, seq, valueize, fn, type, arg0, arg1, arg2))
    return NULL_TREE;
  return maybe_push_res_to_seq (&res_op, seq);
}

/* Common subroutine of gimple_extract_op and gimple_simplify.  Try to
   describe STMT in RES_OP, returning true on success.  Before recording
   an operand, call:

   - VALUEIZE_CONDITION for a COND_EXPR condition
   - VALUEIZE_OP for every other top-level operand

   Both routines take a tree argument and returns a tree.  */

template<typename ValueizeOp, typename ValueizeCondition>
inline bool
gimple_extract (gimple *stmt, gimple_match_op *res_op,
		ValueizeOp valueize_op,
		ValueizeCondition valueize_condition)
{
  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      {
	enum tree_code code = gimple_assign_rhs_code (stmt);
	tree type = TREE_TYPE (gimple_assign_lhs (stmt));
	switch (gimple_assign_rhs_class (stmt))
	  {
	  case GIMPLE_SINGLE_RHS:
	    if (code == REALPART_EXPR
		|| code == IMAGPART_EXPR
		|| code == VIEW_CONVERT_EXPR)
	      {
		tree op0 = TREE_OPERAND (gimple_assign_rhs1 (stmt), 0);
		res_op->set_op (code, type, valueize_op (op0));
		return true;
	      }
	    else if (code == BIT_FIELD_REF)
	      {
		tree rhs1 = gimple_assign_rhs1 (stmt);
		tree op0 = valueize_op (TREE_OPERAND (rhs1, 0));
		res_op->set_op (code, type, op0,
				TREE_OPERAND (rhs1, 1),
				TREE_OPERAND (rhs1, 2),
				REF_REVERSE_STORAGE_ORDER (rhs1));
		return true;
	      }
	    else if (code == SSA_NAME)
	      {
		tree op0 = gimple_assign_rhs1 (stmt);
		res_op->set_op (TREE_CODE (op0), type, valueize_op (op0));
		return true;
	      }
	    break;
	  case GIMPLE_UNARY_RHS:
	    {
	      tree rhs1 = gimple_assign_rhs1 (stmt);
	      res_op->set_op (code, type, valueize_op (rhs1));
	      return true;
	    }
	  case GIMPLE_BINARY_RHS:
	    {
	      tree rhs1 = valueize_op (gimple_assign_rhs1 (stmt));
	      tree rhs2 = valueize_op (gimple_assign_rhs2 (stmt));
	      res_op->set_op (code, type, rhs1, rhs2);
	      return true;
	    }
	  case GIMPLE_TERNARY_RHS:
	    {
	      tree rhs1 = gimple_assign_rhs1 (stmt);
	      if (code == COND_EXPR && COMPARISON_CLASS_P (rhs1))
		rhs1 = valueize_condition (rhs1);
	      else
		rhs1 = valueize_op (rhs1);
	      tree rhs2 = valueize_op (gimple_assign_rhs2 (stmt));
	      tree rhs3 = valueize_op (gimple_assign_rhs3 (stmt));
	      res_op->set_op (code, type, rhs1, rhs2, rhs3);
	      return true;
	    }
	  default:
	    gcc_unreachable ();
	  }
	break;
      }

    case GIMPLE_CALL:
      /* ???  This way we can't simplify calls with side-effects.  */
      if (gimple_call_lhs (stmt) != NULL_TREE
	  && gimple_call_num_args (stmt) >= 1
	  && gimple_call_num_args (stmt) <= 5)
	{
	  combined_fn cfn;
	  if (gimple_call_internal_p (stmt))
	    cfn = as_combined_fn (gimple_call_internal_fn (stmt));
	  else
	    {
	      tree fn = gimple_call_fn (stmt);
	      if (!fn)
		return false;

	      fn = valueize_op (fn);
	      if (TREE_CODE (fn) != ADDR_EXPR
		  || TREE_CODE (TREE_OPERAND (fn, 0)) != FUNCTION_DECL)
		return false;

	      tree decl = TREE_OPERAND (fn, 0);
	      if (DECL_BUILT_IN_CLASS (decl) != BUILT_IN_NORMAL
		  || !gimple_builtin_call_types_compatible_p (stmt, decl))
		return false;

	      cfn = as_combined_fn (DECL_FUNCTION_CODE (decl));
	    }

	  unsigned int num_args = gimple_call_num_args (stmt);
	  res_op->set_op (cfn, TREE_TYPE (gimple_call_lhs (stmt)), num_args);
	  for (unsigned i = 0; i < num_args; ++i)
	    res_op->ops[i] = valueize_op (gimple_call_arg (stmt, i));
	  return true;
	}
      break;

    case GIMPLE_COND:
      {
	tree lhs = valueize_op (gimple_cond_lhs (stmt));
	tree rhs = valueize_op (gimple_cond_rhs (stmt));
	res_op->set_op (gimple_cond_code (stmt), boolean_type_node, lhs, rhs);
	return true;
      }

    default:
      break;
    }

  return false;
}

/* Try to describe STMT in RES_OP, returning true on success.
   For GIMPLE_CONDs, describe the condition that is being tested.
   For GIMPLE_ASSIGNs, describe the rhs of the assignment.
   For GIMPLE_CALLs, describe the call.  */

bool
gimple_extract_op (gimple *stmt, gimple_match_op *res_op)
{
  auto nop = [](tree op) { return op; };
  return gimple_extract (stmt, res_op, nop, nop);
}

/* The main STMT based simplification entry.  It is used by the fold_stmt
   and the fold_stmt_to_constant APIs.  */

bool
gimple_simplify (gimple *stmt, gimple_match_op *res_op, gimple_seq *seq,
		 tree (*valueize)(tree), tree (*top_valueize)(tree))
{
  bool valueized = false;
  auto valueize_op = [&](tree op)
    {
      return do_valueize (op, top_valueize, valueized);
    };
  auto valueize_condition = [&](tree op) -> tree
    {
      bool cond_valueized = false;
      tree lhs = do_valueize (TREE_OPERAND (op, 0), top_valueize,
			      cond_valueized);
      tree rhs = do_valueize (TREE_OPERAND (op, 1), top_valueize,
			      cond_valueized);
      gimple_match_op res_op2 (res_op->cond, TREE_CODE (op),
			       TREE_TYPE (op), lhs, rhs);
      if ((gimple_resimplify2 (seq, &res_op2, valueize)
	   || cond_valueized)
	  && res_op2.code.is_tree_code ())
	{
	  auto code = tree_code (res_op2.code);
	  if (TREE_CODE_CLASS (code) == tcc_comparison)
	    {
	      valueized = true;
	      return build2 (code, TREE_TYPE (op),
			     res_op2.ops[0], res_op2.ops[1]);
	    }
	  else if (code == SSA_NAME
		   || code == INTEGER_CST
		   || code == VECTOR_CST)
	    {
	      valueized = true;
	      return res_op2.ops[0];
	    }
	}
      return valueize_op (op);
    };

  if (!gimple_extract (stmt, res_op, valueize_op, valueize_condition))
    return false;

  if (res_op->code.is_internal_fn ())
    {
      internal_fn ifn = internal_fn (res_op->code);
      if (try_conditional_simplification (ifn, res_op, seq, valueize))
	return true;
    }

  if (!res_op->reverse
      && res_op->num_ops
      && res_op->resimplify (seq, valueize))
    return true;

  return valueized;
}

/* Helper that matches and simplifies the toplevel result from
   a gimple_simplify run (where we don't want to build
   a stmt in case it's used in in-place folding).  Replaces
   RES_OP with a simplified and/or canonicalized result and
   returns whether any change was made.  */

static bool
gimple_resimplify1 (gimple_seq *seq, gimple_match_op *res_op,
		    tree (*valueize)(tree))
{
  if (constant_for_folding (res_op->ops[0]))
    {
      tree tem = NULL_TREE;
      if (res_op->code.is_tree_code ())
	{
	  auto code = tree_code (res_op->code);
	  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code))
	      && TREE_CODE_LENGTH (code) == 1)
	    tem = const_unop (code, res_op->type, res_op->ops[0]);
	}
      else
	tem = fold_const_call (combined_fn (res_op->code), res_op->type,
			       res_op->ops[0]);
      if (tem != NULL_TREE
	  && CONSTANT_CLASS_P (tem))
	{
	  if (TREE_OVERFLOW_P (tem))
	    tem = drop_tree_overflow (tem);
	  res_op->set_value (tem);
	  maybe_resimplify_conditional_op (seq, res_op, valueize);
	  return true;
	}
    }

  /* Limit recursion, there are cases like PR80887 and others, for
     example when value-numbering presents us with unfolded expressions
     that we are really not prepared to handle without eventual
     oscillation like ((_50 + 0) + 8) where _50 gets mapped to _50
     itself as available expression.  */
  static unsigned depth;
  if (depth > 10)
    {
      if (dump_file && (dump_flags & TDF_FOLDING))
	fprintf (dump_file, "Aborting expression simplification due to "
		 "deep recursion\n");
      return false;
    }

  ++depth;
  gimple_match_op res_op2 (*res_op);
  if (gimple_simplify (&res_op2, seq, valueize,
		       res_op->code, res_op->type, res_op->ops[0]))
    {
      --depth;
      *res_op = res_op2;
      return true;
    }
  --depth;

  if (maybe_resimplify_conditional_op (seq, res_op, valueize))
    return true;

  return false;
}

/* Helper that matches and simplifies the toplevel result from
   a gimple_simplify run (where we don't want to build
   a stmt in case it's used in in-place folding).  Replaces
   RES_OP with a simplified and/or canonicalized result and
   returns whether any change was made.  */

static bool
gimple_resimplify2 (gimple_seq *seq, gimple_match_op *res_op,
		    tree (*valueize)(tree))
{
  if (constant_for_folding (res_op->ops[0])
      && constant_for_folding (res_op->ops[1]))
    {
      tree tem = NULL_TREE;
      if (res_op->code.is_tree_code ())
	{
	  auto code = tree_code (res_op->code);
	  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code))
	      && TREE_CODE_LENGTH (code) == 2)
	    tem = const_binop (code, res_op->type,
			       res_op->ops[0], res_op->ops[1]);
	}
      else
	tem = fold_const_call (combined_fn (res_op->code), res_op->type,
			       res_op->ops[0], res_op->ops[1]);
      if (tem != NULL_TREE
	  && CONSTANT_CLASS_P (tem))
	{
	  if (TREE_OVERFLOW_P (tem))
	    tem = drop_tree_overflow (tem);
	  res_op->set_value (tem);
	  maybe_resimplify_conditional_op (seq, res_op, valueize);
	  return true;
	}
    }

  /* Canonicalize operand order.  */
  bool canonicalized = false;
  bool is_comparison
    = (res_op->code.is_tree_code ()
       && TREE_CODE_CLASS (tree_code (res_op->code)) == tcc_comparison);
  if ((is_comparison || commutative_binary_op_p (res_op->code, res_op->type))
      && tree_swap_operands_p (res_op->ops[0], res_op->ops[1]))
    {
      std::swap (res_op->ops[0], res_op->ops[1]);
      if (is_comparison)
	res_op->code = swap_tree_comparison (tree_code (res_op->code));
      canonicalized = true;
    }

  /* Limit recursion, see gimple_resimplify1.  */
  static unsigned depth;
  if (depth > 10)
    {
      if (dump_file && (dump_flags & TDF_FOLDING))
	fprintf (dump_file, "Aborting expression simplification due to "
		 "deep recursion\n");
      return false;
    }

  ++depth;
  gimple_match_op res_op2 (*res_op);
  if (gimple_simplify (&res_op2, seq, valueize,
		       res_op->code, res_op->type,
		       res_op->ops[0], res_op->ops[1]))
    {
      --depth;
      *res_op = res_op2;
      return true;
    }
  --depth;

  if (maybe_resimplify_conditional_op (seq, res_op, valueize))
    return true;

  return canonicalized;
}

/* Helper that matches and simplifies the toplevel result from
   a gimple_simplify run (where we don't want to build
   a stmt in case it's used in in-place folding).  Replaces
   RES_OP with a simplified and/or canonicalized result and
   returns whether any change was made.  */

static bool
gimple_resimplify3 (gimple_seq *seq, gimple_match_op *res_op,
		    tree (*valueize)(tree))
{
  if (constant_for_folding (res_op->ops[0])
      && constant_for_folding (res_op->ops[1])
      && constant_for_folding (res_op->ops[2]))
    {
      tree tem = NULL_TREE;
      if (res_op->code.is_tree_code ())
	{
	  auto code = tree_code (res_op->code);
	  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code))
	      && TREE_CODE_LENGTH (code) == 3)
	    tem = fold_ternary/*_to_constant*/ (code, res_op->type,
						res_op->ops[0], res_op->ops[1],
						res_op->ops[2]);
	}
      else
	tem = fold_const_call (combined_fn (res_op->code), res_op->type,
			       res_op->ops[0], res_op->ops[1], res_op->ops[2]);
      if (tem != NULL_TREE
	  && CONSTANT_CLASS_P (tem))
	{
	  if (TREE_OVERFLOW_P (tem))
	    tem = drop_tree_overflow (tem);
	  res_op->set_value (tem);
	  maybe_resimplify_conditional_op (seq, res_op, valueize);
	  return true;
	}
    }

  /* Canonicalize operand order.  */
  bool canonicalized = false;
  int argno = first_commutative_argument (res_op->code, res_op->type);
  if (argno >= 0
      && tree_swap_operands_p (res_op->ops[argno], res_op->ops[argno + 1]))
    {
      std::swap (res_op->ops[argno], res_op->ops[argno + 1]);
      canonicalized = true;
    }

  /* Limit recursion, see gimple_resimplify1.  */
  static unsigned depth;
  if (depth > 10)
    {
      if (dump_file && (dump_flags & TDF_FOLDING))
	fprintf (dump_file, "Aborting expression simplification due to "
		 "deep recursion\n");
      return false;
    }

  ++depth;
  gimple_match_op res_op2 (*res_op);
  if (gimple_simplify (&res_op2, seq, valueize,
		       res_op->code, res_op->type,
		       res_op->ops[0], res_op->ops[1], res_op->ops[2]))
    {
      --depth;
      *res_op = res_op2;
      return true;
    }
  --depth;

  if (maybe_resimplify_conditional_op (seq, res_op, valueize))
    return true;

  return canonicalized;
}

/* Helper that matches and simplifies the toplevel result from
   a gimple_simplify run (where we don't want to build
   a stmt in case it's used in in-place folding).  Replaces
   RES_OP with a simplified and/or canonicalized result and
   returns whether any change was made.  */

static bool
gimple_resimplify4 (gimple_seq *seq, gimple_match_op *res_op,
		    tree (*valueize)(tree))
{
  /* No constant folding is defined for four-operand functions.  */

  /* Canonicalize operand order.  */
  bool canonicalized = false;
  int argno = first_commutative_argument (res_op->code, res_op->type);
  if (argno >= 0
      && tree_swap_operands_p (res_op->ops[argno], res_op->ops[argno + 1]))
    {
      std::swap (res_op->ops[argno], res_op->ops[argno + 1]);
      canonicalized = true;
    }

  /* Limit recursion, see gimple_resimplify1.  */
  static unsigned depth;
  if (depth > 10)
    {
      if (dump_file && (dump_flags & TDF_FOLDING))
	fprintf (dump_file, "Aborting expression simplification due to "
		 "deep recursion\n");
      return false;
    }

  ++depth;
  gimple_match_op res_op2 (*res_op);
  if (gimple_simplify (&res_op2, seq, valueize,
		       res_op->code, res_op->type,
		       res_op->ops[0], res_op->ops[1], res_op->ops[2],
		       res_op->ops[3]))
    {
      --depth;
      *res_op = res_op2;
      return true;
    }
  --depth;

  if (maybe_resimplify_conditional_op (seq, res_op, valueize))
    return true;

  return canonicalized;
}

/* Helper that matches and simplifies the toplevel result from
   a gimple_simplify run (where we don't want to build
   a stmt in case it's used in in-place folding).  Replaces
   RES_OP with a simplified and/or canonicalized result and
   returns whether any change was made.  */

static bool
gimple_resimplify5 (gimple_seq *seq, gimple_match_op *res_op,
		    tree (*valueize)(tree))
{
  /* No constant folding is defined for five-operand functions.  */

  /* Canonicalize operand order.  */
  bool canonicalized = false;
  int argno = first_commutative_argument (res_op->code, res_op->type);
  if (argno >= 0
      && tree_swap_operands_p (res_op->ops[argno], res_op->ops[argno + 1]))
    {
      std::swap (res_op->ops[argno], res_op->ops[argno + 1]);
      canonicalized = true;
    }

  gimple_match_op res_op2 (*res_op);
  if (gimple_simplify (&res_op2, seq, valueize,
		       res_op->code, res_op->type,
		       res_op->ops[0], res_op->ops[1], res_op->ops[2],
		       res_op->ops[3], res_op->ops[4]))
    {
      *res_op = res_op2;
      return true;
    }

  if (maybe_resimplify_conditional_op (seq, res_op, valueize))
    return true;

  return canonicalized;
}

/* Return a canonical form for CODE when operating on TYPE.  The idea
   is to remove redundant ways of representing the same operation so
   that code_helpers can be hashed and compared for equality.

   The only current canonicalization is to replace built-in functions
   with internal functions, in cases where internal-fn.def defines
   such an internal function.

   Note that the new code_helper cannot necessarily be used in place of
   the original code_helper.  For example, the new code_helper might be
   an internal function that the target does not support.  */

code_helper
canonicalize_code (code_helper code, tree type)
{
  if (code.is_fn_code ())
    return associated_internal_fn (combined_fn (code), type);
  return code;
}

/* Return true if CODE is a binary operation and if CODE is commutative when
   operating on type TYPE.  */

bool
commutative_binary_op_p (code_helper code, tree type)
{
  if (code.is_tree_code ())
    return commutative_tree_code (tree_code (code));
  auto cfn = combined_fn (code);
  return commutative_binary_fn_p (associated_internal_fn (cfn, type));
}

/* Return true if CODE represents a ternary operation and if the first two
   operands are commutative when CODE is operating on TYPE.  */

bool
commutative_ternary_op_p (code_helper code, tree type)
{
  if (code.is_tree_code ())
    return commutative_ternary_tree_code (tree_code (code));
  auto cfn = combined_fn (code);
  return commutative_ternary_fn_p (associated_internal_fn (cfn, type));
}

/* If CODE is commutative in two consecutive operands, return the
   index of the first, otherwise return -1.  */

int
first_commutative_argument (code_helper code, tree type)
{
  if (code.is_tree_code ())
    {
      auto tcode = tree_code (code);
      if (commutative_tree_code (tcode)
	  || commutative_ternary_tree_code (tcode))
	return 0;
      return -1;
    }
  auto cfn = combined_fn (code);
  return first_commutative_argument (associated_internal_fn (cfn, type));
}

/* Return true if CODE is a binary operation that is associative when
   operating on type TYPE.  */

bool
associative_binary_op_p (code_helper code, tree type)
{
  if (code.is_tree_code ())
    return associative_tree_code (tree_code (code));
  auto cfn = combined_fn (code);
  return associative_binary_fn_p (associated_internal_fn (cfn, type));
}

/* Return true if the target directly supports operation CODE on type TYPE.
   QUERY_TYPE acts as for optab_for_tree_code.  */

bool
directly_supported_p (code_helper code, tree type, optab_subtype query_type)
{
  if (code.is_tree_code ())
    {
      direct_optab optab = optab_for_tree_code (tree_code (code), type,
						query_type);
      return (optab != unknown_optab
	      && optab_handler (optab, TYPE_MODE (type)) != CODE_FOR_nothing);
    }
  gcc_assert (query_type == optab_default
	      || (query_type == optab_vector && VECTOR_TYPE_P (type))
	      || (query_type == optab_scalar && !VECTOR_TYPE_P (type)));
  internal_fn ifn = associated_internal_fn (combined_fn (code), type);
  return (direct_internal_fn_p (ifn)
	  && direct_internal_fn_supported_p (ifn, type, OPTIMIZE_FOR_SPEED));
}

/* A wrapper around the internal-fn.cc versions of get_conditional_internal_fn
   for a code_helper CODE operating on type TYPE.  */

internal_fn
get_conditional_internal_fn (code_helper code, tree type)
{
  if (code.is_tree_code ())
    return get_conditional_internal_fn (tree_code (code));
  auto cfn = combined_fn (code);
  return get_conditional_internal_fn (associated_internal_fn (cfn, type));
}