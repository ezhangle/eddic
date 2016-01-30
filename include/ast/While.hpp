//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_WHILE_H
#define AST_WHILE_H

#include <vector>
#include <memory>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Deferred.hpp"
#include "ast/Value.hpp"

namespace eddic {

class Context;

namespace ast {

/*!
 * \class ASTWhile
 * \brief The AST node for a while loop. 
 * Should only be used from the Deferred version (eddic::ast::While).
 */
struct ASTWhile {
    std::shared_ptr<Context> context; 

    Value condition;
    std::vector<Instruction> instructions;

    mutable long references = 0;
};

/*!
 * \typedef While
 * \brief The AST node for a while loop.
 */
typedef Deferred<ASTWhile> While;

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::While, 
    (eddic::ast::Value, Content->condition)
    (std::vector<eddic::ast::Instruction>, Content->instructions)
)

#endif
