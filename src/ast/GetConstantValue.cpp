//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <cassert>

#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include "ast/GetConstantValue.hpp"
#include "ast/Value.hpp"

#include "Variable.hpp"

using namespace eddic;

Val ast::GetConstantValue::operator()(const ast::Litteral& litteral) const {
    return make_pair(litteral.value, litteral.value.size() - 2);
}

Val ast::GetConstantValue::operator()(const ast::Integer& integer) const {
    return integer.value;
}

Val ast::GetConstantValue::operator()(const ast::Minus& minus) const {
    return -1 * boost::get<int>(boost::apply_visitor(*this, minus.Content->value));
}

Val ast::GetConstantValue::operator()(const ast::VariableValue& value) const {
    Type type = value.Content->var->type();
    assert(type.isConst());
        
    auto val = value.Content->var->val();

    if(type.base() == BaseType::INT){
        return boost::get<int>(val);
    } else if(type.base() == BaseType::STRING){
        return boost::get<std::pair<std::string, int>>(val);
    }

    //Type not managed
    assert(false);
}