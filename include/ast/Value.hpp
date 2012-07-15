//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef AST_VALUE_H
#define AST_VALUE_H

#include <boost/variant/variant.hpp>

#include "ast/values_def.hpp"

#include "ast/Integer.hpp"
#include "ast/IntegerSuffix.hpp"
#include "ast/Float.hpp"
#include "ast/Litteral.hpp"
#include "ast/VariableValue.hpp"
#include "ast/DereferenceValue.hpp"
#include "ast/True.hpp"
#include "ast/False.hpp"
#include "ast/Null.hpp"
#include "ast/SuffixOperation.hpp"
#include "ast/PrefixOperation.hpp"

namespace eddic {

namespace ast {

typedef boost::variant<
            Integer, 
            IntegerSuffix, 
            Float,
            Litteral, 
            VariableValue,
            DereferenceValue,
            Expression,
            Minus,
            Plus,
            Null,
            True,
            False,
            ArrayValue,
            FunctionCall,
            Cast,
            BuiltinOperator,
            Assignment,
            Ternary,
            SuffixOperation,
            PrefixOperation
        > Value;

} //end of ast

} //end of eddic

#include "ast/Assignment.hpp"
#include "ast/Ternary.hpp"
#include "ast/Expression.hpp"
#include "ast/ArrayValue.hpp"
#include "ast/FunctionCall.hpp"
#include "ast/BuiltinOperator.hpp"
#include "ast/Minus.hpp"
#include "ast/Plus.hpp"
#include "ast/Cast.hpp"

#endif
