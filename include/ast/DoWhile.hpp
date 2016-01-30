//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_DO_WHILE_H
#define AST_DO_WHILE_H

#include <vector>
#include <memory>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Deferred.hpp"
#include "ast/Value.hpp"

namespace eddic {

class Context;

namespace ast {

/*!
 * \class ASTDoWhile
 * \brief The AST node for a do while loop. 
 * Should only be used from the Deferred version (eddic::ast::DoWhile).
 */
struct ASTDoWhile {
    std::shared_ptr<Context> context; 

    Value condition;
    std::vector<Instruction> instructions;

    mutable long references = 0;
};

/*!
 * \typedef While
 * \brief The AST node for a while loop.
 */
typedef Deferred<ASTDoWhile> DoWhile;

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::DoWhile, 
    (std::vector<eddic::ast::Instruction>, Content->instructions)
    (eddic::ast::Value, Content->condition)
)

#endif
