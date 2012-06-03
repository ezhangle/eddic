//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef LTAC_UTILS_H
#define LTAC_UTILS_H

#include <memory>
#include <boost/variant.hpp>

#include "mtac/Program.hpp"
#include "ltac/Program.hpp"
#include "ltac/RegisterManager.hpp"

namespace eddic {

namespace ltac {

bool is_float_operator(mtac::BinaryOperator op);

bool is_float_var(std::shared_ptr<Variable> variable);

bool is_int_var(std::shared_ptr<Variable> variable);

template<typename Variant>
bool is_variable(Variant& variant){
    return boost::get<std::shared_ptr<Variable>>(&variant);
}

template<typename Variant>
std::shared_ptr<Variable> get_variable(Variant& variant){
    return boost::get<std::shared_ptr<Variable>>(variant);
}

void add_instruction(std::shared_ptr<ltac::Function> function, ltac::Operator op);
void add_instruction(std::shared_ptr<ltac::Function> function, ltac::Operator op, ltac::Argument arg1);
void add_instruction(std::shared_ptr<ltac::Function> function, ltac::Operator op, ltac::Argument arg1, ltac::Argument arg2);
void add_instruction(std::shared_ptr<ltac::Function> function, ltac::Operator op, ltac::Argument arg1, ltac::Argument arg2, ltac::Argument arg3);

ltac::Register to_register(std::shared_ptr<Variable> var, ltac::RegisterManager& manager);
ltac::Argument to_arg(mtac::Argument argument, ltac::RegisterManager& manager);

} //end of ltac

} //end of eddic

#endif