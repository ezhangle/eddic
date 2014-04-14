//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef AST_FOREACH_H
#define AST_FOREACH_H

#include <vector>
#include <memory>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Deferred.hpp"

namespace eddic {

class Context;

namespace ast {

/*!
 * \class ASTForeach
 * \brief The AST node for a foreach loop. 
 * Should only be used from the Deferred version (eddic::ast::Foreach).
 */
struct ASTForeach {
    std::shared_ptr<Context> context;

    ast::Position position;
    ast::Type variableType;
    std::string variableName;
    int from;
    int to;
    std::vector<Instruction> instructions;

    mutable long references = 0;
};

/*!
 * \typedef Foreach
 * \brief The AST node for a foreach loop.
 */
typedef Deferred<ASTForeach> Foreach;

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::Foreach, 
    (eddic::ast::Position, Content->position)
    (eddic::ast::Type, Content->variableType)
    (std::string, Content->variableName)
    (int, Content->from)
    (int, Content->to)
    (std::vector<eddic::ast::Instruction>, Content->instructions)
)

#endif
