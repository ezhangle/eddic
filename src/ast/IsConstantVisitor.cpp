//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "ast/IsConstantVisitor.hpp"
#include "ast/Value.hpp"

#include "Variable.hpp"
#include "VisitorUtils.hpp"

using namespace eddic;

ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::Litteral, true)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::Integer, true)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::True, true)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::False, true)

ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::ArrayValue, false)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::FunctionCall, false)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::SuffixOperation, false)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::PrefixOperation, false)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::BuiltinOperator, false)
ASSIGN_INSIDE_CONST(ast::IsConstantVisitor, ast::Assignment, false)

bool ast::IsConstantVisitor::operator()(ast::Minus& value) const {
    return visit(*this, value.Content->value);
}

bool ast::IsConstantVisitor::operator()(ast::Plus& value) const {
    return visit(*this, value.Content->value);
}

bool ast::IsConstantVisitor::operator()(ast::VariableValue& variable) const {
    return variable.Content->var->type().isConst();
}

bool ast::IsConstantVisitor::operator()(ast::ComposedValue& value) const {
    if(visit(*this, value.Content->first)){
        for(auto& op : value.Content->operations){
            if(!visit(*this, op.get<1>())){
                return false;
            }
        }

        return true;
    }

    return false;
}