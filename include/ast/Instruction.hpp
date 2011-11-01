//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef AST_INSTRUCTION_H
#define AST_INSTRUCTION_H

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "ast/Swap.hpp"
#include "ast/FunctionCall.hpp"
#include "ast/Assignment.hpp"
#include "ast/Declaration.hpp"

namespace eddic {

struct ASTWhile;

typedef boost::variant<
            ASTFunctionCall, 
            ASTSwap, 
            ASTDeclaration,
            ASTAssignment, 
            boost::recursive_wrapper<ASTWhile>> 
        ASTInstruction;

} //end of eddic

#include "ast/While.hpp"

#endif
