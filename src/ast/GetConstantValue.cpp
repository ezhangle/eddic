//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "variant.hpp"
#include "Variable.hpp"
#include "Type.hpp"

#include "ast/GetConstantValue.hpp"
#include "ast/Value.hpp"

using namespace eddic;

Val ast::GetConstantValue::operator()(const ast::Literal& literal) const {
    return make_pair(literal.value, literal.value.size() - 2);
}

Val ast::GetConstantValue::operator()(const ast::Integer& integer) const {
    return integer.value;
}

Val ast::GetConstantValue::operator()(const ast::False&) const {
    return 0;
}

Val ast::GetConstantValue::operator()(const ast::True&) const {
    return 1;
}

Val ast::GetConstantValue::operator()(const ast::IntegerSuffix& integer) const {
    return (double) integer.value;
}

Val ast::GetConstantValue::operator()(const ast::Float& float_) const {
    return float_.value;
}

Val ast::GetConstantValue::operator()(const ast::PrefixOperation& minus) const {
    if(minus.Content->op == ast::Operator::SUB){
        return -1 * boost::get<int>(boost::apply_visitor(*this, minus.Content->left_value));
    }

    eddic_unreachable("Not constant");
}

Val ast::GetConstantValue::operator()(const ast::VariableValue& value) const {
    auto var = value.variable();
    auto type = var->type();
    auto val = var->val();

    if(type == INT){
        return boost::get<int>(val);
    } else if(type == STRING){
        return boost::get<std::pair<std::string, int>>(val);
    }

    eddic_unreachable("This variable is of a type that cannot be constant");
}
