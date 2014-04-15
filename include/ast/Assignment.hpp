//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include <memory>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Deferred.hpp"
#include "ast/Position.hpp"
#include "ast/Operator.hpp"

namespace eddic {

class Context;

namespace ast {

/*!
 * \class ASTAssignment
 * \brief The AST node for an assignement to a variable.  
 * Should only be used from the Deferred version (eddic::ast::Assignment).
 */
struct ASTAssignment {
    std::shared_ptr<Context> context;

    Position position;
    Value left_value;
    Value value;
    ast::Operator op = ast::Operator::ASSIGN; //If not specified, it is not a compound operator

    mutable long references = 0;
};

/*!
 * \typedef Assignment
 * \brief The AST node for an assignment to a variable. 
 */
typedef Deferred<ASTAssignment> Assignment;

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::Assignment, 
    (eddic::ast::Position, Content->position)
    (eddic::ast::Value, Content->left_value)
    (eddic::ast::Operator, Content->op)
    (eddic::ast::Value, Content->value)
)

#endif
